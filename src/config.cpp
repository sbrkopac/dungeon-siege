
#include "config.hpp"

#include <minIni.h>
#include <osg/ArgumentParser>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>

#ifdef WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <basedir.h>
#endif

namespace ehb
{
    // These were moved here so we don't have to make Config a singleton
    static std::unordered_map<std::string, bool> boolMap;
    static std::unordered_map<std::string, float> floatMap;
    static std::unordered_map<std::string, int> intMap;
    static std::unordered_map<std::string, std::string> stringMap;

    constexpr const char * registryKeyVec[] =
    {
        "Software\\Wow6432Node\\Microsoft\\Microsoft Games\\Dungeon Siege Legends of Aranna\\1.0",
        "Software\\Wow6432Node\\Microsoft\\Microsoft Games\\DungeonSiege\\1.0",
        "Software\\Wow6432Node\\Microsoft\\Microsoft Games\\DungeonSiegeDemo\\1.0",
        "Software\\Microsoft\\Microsoft Games\\Dungeon Siege Legends of Aranna\\1.0",
        "Software\\Microsoft\\Microsoft Games\\DungeonSiege\\1.0",
        "Software\\Microsoft\\Microsoft Games\\DungeonSiegeDemo\\1.0"
    };

    /*
     * highest preference to command line options, ini, then registry (though for many variables there is only 1 source)
     */
    Config::Config(int * argc, char * argv[])
    {
        osg::ArgumentParser args(argc, argv);

#ifdef WIN32
        { // grab the win32 specific folders to use for cache, config, and user data
            CHAR path[MAX_PATH];

            if (SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path) == S_OK)
            {
                const std::string value = osgDB::concatPaths(path, "OpenSiege");

                if (osgDB::makeDirectory(value)) stringMap["cache-dir"] = value;
            }

            if (SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path) == S_OK)
            {
                const std::string value = osgDB::concatPaths(path, "OpenSiege");

                if (osgDB::makeDirectory(value)) stringMap["config-dir"] = value;
                if (osgDB::makeDirectory(value)) stringMap["data-dir"] = value;
            }
        }
#else
        { // grab the freedesktop.org specific folders to use for cache, config, and user data
            xdgHandle xdg;

            xdgInitHandle(&xdg);

            if (const char * path = xdgCacheHome(&xdg))
            {
                const std::string value = osgDB::concatPaths(path, "OpenSiege");

                if (osgDB::makeDirectory(value)) stringMap["cache-dir"] = value;
            }

            if (const char * path = xdgConfigHome(&xdg))
            {
                const std::string value = osgDB::concatPaths(path, "OpenSiege");

                if (osgDB::makeDirectory(value)) stringMap["config-dir"] = value;
            }

            if (const char * path = xdgDataHome(&xdg))
            {
                const std::string value = osgDB::concatPaths(path, "OpenSiege");

                if (osgDB::makeDirectory(value)) stringMap["data-dir"] = value;
            }

            xdgWipeHandle(&xdg);
        }
#endif

        { // parse all boolean values from the command line
            bool value;

            if (args.read("--fullscreen", value)) boolMap["fullscreen"] = value;
            if (args.read("--intro", value)) boolMap["intro"] = value;
            if (args.read("--sound", value)) boolMap["sound"] = value;
        }
        { // parse all integer values from the command line
            int value;

            if (args.read("--bpp", value)) intMap["bpp"] = value;
            if (args.read("--height", value)) intMap["height"] = value;
            if (args.read("--maxfps", value)) intMap["maxfps"] = value;
            if (args.read("--width", value)) intMap["width"] = value;
        }
        { // either read in the ds install path specified by the user or attempt to grab it from the windows/wine registry
            std::string value;

            if (args.read("--ds-install-path", value))
            {
                stringMap["ds-install-path"] = value;
            }
            else
            {
                // search the windows registry
#ifdef WIN32
                for (const char * key : registryKeyVec)
                {
                    // http://stackoverflow.com/questions/34065/how-to-read-a-value-from-the-windows-registry
                    HKEY hKey;

                    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, key, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
                    {
                        CHAR szBuffer[MAX_PATH];
                        DWORD dwBufferSize = sizeof(szBuffer);

                        if (RegQueryValueEx(hKey, "EXE Path", 0, NULL, (LPBYTE) szBuffer, &dwBufferSize) == ERROR_SUCCESS)
                        {
                            stringMap["ds-install-path"] = szBuffer;

                            break;
                        }
                    }
                }
#else
                if (const char * WINEPREFIX = std::getenv("WINEPREFIX"))
                {
                    const std::string system = osgDB::concatPaths(WINEPREFIX, "system.reg");
                    const std::string devices = osgDB::concatPaths(WINEPREFIX, "dosdevices");

                    if (osgDB::fileType(system) == osgDB::REGULAR_FILE)
                    {
                        minIni ini(system);

                        for (std::string section : registryKeyVec)
                        {
                            std::cout << "looking for section: " << section << std::endl;

                            { // replace all \ with \\... because wine is like that
                                std::string::size_type index = 0;

                                while ((index = section.find("\\", index)) != std::string::npos)
                                {
                                    section.insert(index, "\\");

                                    index += 2;
                                }
                            }

                            std::string value = ini.gets(section, "\"EXE Path\"");

                            if (value.empty())
                            {
                                // couldn't find windows registry key in wine... no problem, app just not installed
                            }
                            else
                            {
                                { // replace all \\ with \... because wine is like that
                                    std::string::size_type index = 0;

                                    while ((index = value.find("\\\\", index)) != std::string::npos)
                                    {
                                        value.replace(index, 2, "/");
                                    }
                                }

                                // wine uses lowercase drive letters under $WINEPREFIX/dosdevices/
                                value[0] = std::tolower(value[0]);

                                stringMap["ds-install-path"] = osgDB::concatPaths(devices, value);

                                break;
                            }
                        }
                    }
                    else
                    {
                        // TODO: print a warning out... this is weird... why no system.reg??
                    }
                }
#endif
            }
        }

        const std::string iniFileName = osgDB::concatPaths(getString("config-dir", "."), "OpenSiege.ini");

        if (osgDB::fileExists(iniFileName))
        {
            minIni ini(iniFileName);
            { // parse all integer values from user configuration file
                int value;

                if ((value = ini.geti("", "x", -1)) != -1) intMap["x"] = value;
                if ((value = ini.geti("", "y", -1)) != -1) intMap["y"] = value;
                if ((value = ini.geti("", "width", -1)) != -1) intMap["width"] = value;
                if ((value = ini.geti("", "height", -1)) != -1) intMap["height"] = value;
                if ((value = ini.geti("", "bpp", -1)) != -1) intMap["bpp"] = value;
            }
        }
    }

    Config::~Config()
    {
    }

    bool Config::getBool(const std::string & key, bool defaultValue)
    {
        const auto itr = boolMap.find(key);

        return (itr != boolMap.end()) ? itr->second : defaultValue;
    }

    float Config::getFloat(const std::string & key, float defaultValue)
    {
        const auto itr = floatMap.find(key);

        return (itr != floatMap.end()) ? itr->second : defaultValue;
    }

    int Config::getInt(const std::string & key, int defaultValue)
    {
        const auto itr = intMap.find(key);

        return (itr != intMap.end()) ? itr->second : defaultValue;
    }

    const std::string & Config::getString(const std::string & key, const std::string & defaultValue)
    {
        const auto itr = stringMap.find(key);

        return (itr != stringMap.end()) ? itr->second : defaultValue;
    }


    void Config::dump(std::ostream & stream)
    {
        for (const auto & entry : boolMap)
        {
            stream << entry.first << " = " << (entry.second ? "true" : "false") << std::endl;
        }

        for (const auto & entry : floatMap)
        {
            stream << entry.first << " = " << entry.second << std::endl;
        }

        for (const auto & entry : intMap)
        {
            stream << entry.first << " = " << entry.second << std::endl;
        }

        for (const auto & entry : stringMap)
        {
            stream << entry.first << " = " << entry.second << std::endl;
        }
    }
}
