
#include <set>
#include <unordered_map>
#include <spdlog/spdlog.h>
#include <osg/ArgumentParser>
#include <osgDB/FileNameUtils>
#include "DirectoryArchive.hpp"
#include "FileSys.hpp"
#include "IConfig.hpp"
#include "TankArchive.hpp"
#define DSMOD
namespace ehb
{
    FileSys::FileSys(IConfig & config)
    {
        auto log = spdlog::get("log");

        std::set<std::string> unique;

        for (const std::string & directory : {
            osgDB::concatPaths(config.getString("data-dir"), "Maps"),
            osgDB::concatPaths(config.getString("data-dir"), "Mods"),
            osgDB::concatPaths(config.getString("ds-install-path"), "Maps"),
            osgDB::concatPaths(config.getString("ds-install-path"), "Mods"),
            osgDB::concatPaths(config.getString("ds-install-path"), "Resources"),
            // osgDB::concatPaths(config.getString("ds-install-path"), "DSLOA"),
            config.getString("ds-install-path")
        }) {
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
                    unique.insert(osgDB::concatPaths(directory, filename));
                }
            }
        }

        for (const std::string & filename : unique)
        {
            std::unique_ptr<IArchive> archive(new TankArchive);

            if (archive->open(filename))
            {
                eachArchive.push_back(std::move(archive));

                log->info("including {}", filename);
            }
            else
            {
                log->error("failed to open tank file: {}", filename);
            }
        }

        std::sort(eachArchive.begin(), eachArchive.end(), [](const std::unique_ptr<IArchive> & lhs, const std::unique_ptr<IArchive> & rhs)
        {
            return static_cast<TankArchive & >(*lhs).getFileHeader().priority > static_cast<TankArchive &>(*rhs).getFileHeader().priority;
        });

        /* TODO: file name isn't sufficient to mark something as unique
        std::unique(eachArchive.begin(), eachArchive.end(), [](const std::unique_ptr<IArchive> & lhs, const std::unique_ptr<IArchive> & rhs)
        {
            const TankArchive & lhsAsTank = static_cast<TankArchive & >(*lhs);
            const TankArchive & rhsAsTank = static_cast<TankArchive & >(*rhs);

            return lhsAsTank.getFileHeader().indexCrc32 == rhsAsTank.getFileHeader().indexCrc32 && lhsAsTank.getFileHeader().dataCrc32 == rhsAsTank.getFileHeader().dataCrc32;
        });
        */

#ifdef DSMOD
        {
            std::unique_ptr<IArchive> archive(new DirectoryArchive);

            if (archive->open(osgDB::concatPaths(config.getString("data-dir"), "Bits")))
            {
                eachArchive.insert(eachArchive.begin(), std::move(archive));
            }
        }

#endif

        // TODO: proper logging
        for (const auto & archive : eachArchive)
        {
            // std::cout << "including " << archive->getFileName() << std::endl;
        }
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
