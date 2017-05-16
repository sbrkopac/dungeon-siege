
#include <set>
#include <unordered_map>
#include <osg/ArgumentParser>
#include <osgDB/FileNameUtils>
#include "DirectoryArchive.hpp"
#include "FileSys.hpp"
#include "IConfig.hpp"
#include "TankArchive.hpp"
#define DSMOD
namespace ehb
{
    FileSys::FileSys(int * argc, char * argv[], IConfig * config)
    {
        osg::ArgumentParser args(argc, argv);

        std::set<std::string> uniqueSet;
        const std::unordered_multimap<std::string, std::string> defaultValues =
        {
#ifdef DSMOD
            { "--bits", osgDB::concatPaths(config->getString("data-dir"), "Bits") },
#endif
            { "--map_paths", osgDB::concatPaths(config->getString("ds-install-path"), "Maps") },
            { "--mod_paths", osgDB::concatPaths(config->getString("data-dir"), "Mods") },
            { "--res_paths", config->getString("ds-install-path") },
            { "--res_paths", osgDB::concatPaths(config->getString("ds-install-path"), "Resources") },
            { "--res_paths", osgDB::concatPaths(config->getString("ds-install-path"), "DSLOA") }
        };

        for (const std::string & key : { "--map_paths", "--mod_paths", "--res_paths" })
        {
            std::string arg;
            std::set<std::string> eachArg;

            const auto range = defaultValues.equal_range(key);

            for (auto itr = range.first; itr != range.second; ++itr)
            {
                eachArg.insert(itr->second);
            }

            if (args.find(key) != -1)
            {
                while (args.read(key, arg))
                {
                    const bool isNot = arg[0] == '!';

                    if (isNot)
                    {
                        eachArg.clear();
                        eachArg.insert(arg.substr(1));

                        break;
                    }
                    else
                    {
                        eachArg.insert(arg);
                    }
                }
            }

            for (const std::string & directory : eachArg)
            {
                for (const std::string & filename : osgDB::getDirectoryContents(directory))
                {
                    if (filename == "." || filename == "..") continue;

                    const std::string ext = osgDB::getLowerCaseFileExtension(filename);

                    /*
                     * TODO: clean this up, make a globally accessible function or some such
                     * ds -> demo extension for tanks
                     * dsm -> ???
                     */
                    if (ext == "ds" || ext == "dsm" || ext == "dsmap" || ext == "dsmod" || ext == "dsres" || ext == "dssave")
                    {
                        uniqueSet.insert(osgDB::concatPaths(directory, filename));
                    }
                }
            }
        }

        for (const std::string & filename : uniqueSet)
        {
            std::unique_ptr<IArchive> archive(new TankArchive);

            if (archive->open(filename))
            {
                eachArchive.push_back(std::move(archive));

                std::cout << "including " << filename << std::endl;
            }
            else
            {
                // TODO: proper logging
                std::cout << "failed to open tank file: " << filename << std::endl;
            }
        }

        std::sort(eachArchive.begin(), eachArchive.end(), [](const std::unique_ptr<IArchive> & lhs, const std::unique_ptr<IArchive> & rhs)
        {
            return static_cast<TankArchive & >(*lhs).getFileHeader().priority > static_cast<TankArchive &>(*rhs).getFileHeader().priority;
        });

#ifdef DSMOD
        // now take care of the bits
        std::set<std::string> eachBitsArg;
        std::string arg;

        while (args.read("--bits", arg))
        {
            const bool isNot = arg[0] == '!';

            if (isNot)
            {
                eachBitsArg.clear();
                eachBitsArg.insert(arg.substr(1));

                break;
            }
            else
            {
                eachBitsArg.insert(arg);
            }
        }

        for (const std::string & directory : eachBitsArg)
        {
            std::unique_ptr<IArchive> archive(new DirectoryArchive);

            if (archive->open(directory))
            {
                eachArchive.insert(eachArchive.begin(), std::move(archive));
            }
        }
#endif

        // TODO: proper logging
        /*
        for (const auto & archive : eachArchive)
        {
            std::cout << "including " << archive->getFileName() << std::endl;
        }
        */
    }

    std::string FileSys::concatPaths(const std::string & left, const std::string & right)
    {
        const char delimiterVirtual = '/';
        const char delimiterWindows = '\\';

        if (left.empty())
        {
            return right;
        }

        char lastChar = left[left.size() - 1];

        if (lastChar == delimiterVirtual)
        {
            return left + right;
        }
        else if (lastChar == delimiterWindows)
        {
            return left.substr(0, left.size() - 1) + delimiterVirtual + right;
        }
        else
        {
            return left + delimiterVirtual + right;
        }
    }

    void FileSys::forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recursive)
    {
        std::set<std::string> unique;

        auto func = [&unique, &callback](const std::string & filename, osgDB::FileType fileType)
        {
            if (unique.insert(filename).second)
            {
                callback(filename, fileType);
            }
        };

        for (const auto & archive : eachArchive)
        {
            archive->forEachFile(directory, func, recursive);
        }
    }

#if 0
    bool FileSys::exists(const std::string & filename)
    {
        return false;
    }

    bool FileSys::readGasFile(const std::string & filename, siege::Gas & gas)
    {
        return false;
    }
#endif
}
