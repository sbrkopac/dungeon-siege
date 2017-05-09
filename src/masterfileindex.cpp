
#include <algorithm>
#include <list>
#include <set>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include "directoryarchive.hpp"
#include "masterfileindex.hpp"
#include "tankarchive.hpp"
#include "config.hpp"

#include <iostream>

namespace ehb
{
    MasterFileIndex::MasterFileIndex()
    {
        std::set<std::string> uniqueSet;
        const std::unordered_multimap<std::string, std::string> defaultValues =
        {
#ifdef DSMOD
            { "--bits", osgDB::concatPaths(DungeonSiege::userPath(), "Bits") },
#endif
            { "--map_paths", osgDB::concatPaths(Config::getString("ds-install-path"), "Maps") },
            { "--mod_paths", osgDB::concatPaths(Config::getString("data-dir"), "Mods") },
            { "--res_paths", osgDB::concatPaths(Config::getString("ds-install-path"), "Resources") },
            { "--res_paths", osgDB::concatPaths(Config::getString("ds-install-path"), "DSLOA") }
        };

        // TODO: Fix this, it is legacy from a previous implementation
        for (const std::string & key : { "--map_paths", "--mod_paths", "--res_paths" })
        {
            std::string arg;
            std::set<std::string> eachArg;

            const auto range = defaultValues.equal_range(key);

            for (auto itr = range.first; itr != range.second; ++itr)
            {
                eachArg.insert(itr->second);
            }

            for (const std::string & directory : eachArg)
            {
                for (const std::string & filename : osgDB::getDirectoryContents(directory))
                {
                    if (filename == "." || filename == "..") continue;

                    const std::string ext = osgDB::getLowerCaseFileExtension(filename);

                    // TODO: clean this up, make a globally accessible function or some such
                    if (ext == "dsm" || ext == "dsmap" || ext == "dsmod" || ext == "dsres" || ext == "dssave")
                    {
                        uniqueSet.insert(osgDB::concatPaths(directory, filename));
                    }
                }
            }
        }

        for (const std::string & filename : uniqueSet)
        {
            osg::ref_ptr<Archive> archive = new TankArchive;

            if (archive->open(filename))
            {
                eachArchive.push_back(archive);

                // std::cout << "including " << filename << std::endl;
            }
            else
            {
                // TODO: proper logging
                std::cout << "failed to open tank file: " << filename << std::endl;
            }
        }

        std::sort(eachArchive.begin(), eachArchive.end(), [](const osg::ref_ptr<Archive> & lhs, const osg::ref_ptr<Archive> & rhs)
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
            osg::ref_ptr<Archive> archive = new DirectoryArchive;

            if (archive->open(directory))
            {
                eachArchive.insert(eachArchive.begin(), archive);
            }
        }
#endif

        // TODO: proper logging
        for (const auto & archive : eachArchive)
        {
            std::cout << "including " << archive->getFileName() << std::endl;
        }
    }

    bool MasterFileIndex::readFile(const std::string & filename, siege::ByteArray & data)
    {
        for (const auto & archive : eachArchive)
        {
            if (archive->readFile(filename, data))
            {
                return true;
            }
        }

        return false;
    }

    bool MasterFileIndex::readGasFile(const std::string & filename, siege::Gas &gas)
    {
        siege::ByteArray data;

        for (const auto & archive : eachArchive)
        {
            if (archive->readFile(filename, data))
            {
                if (gas.load(convertForGas(data)))
                {
                    return true;
                }
            }
        }

        return false;
    }

    void MasterFileIndex::forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recurse)
    {
        std::set<std::string> uniqueSet;

        auto func = [&uniqueSet, &callback](const std::string & filename, osgDB::FileType fileType)
        {
            //
            if (uniqueSet.insert(filename).second)
            {
                callback(filename, fileType);
            }
        };

        for (const auto & archive : eachArchive)
        {
            archive->forEachFile(directory, func, recurse);
        }
    }

    std::string MasterFileIndex::concatPaths(const std::string &left, const std::string &right)
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

    std::stringstream MasterFileIndex::convertForGas(const siege::ByteArray &data)
    {
        std::stringstream stream;

        stream.write(reinterpret_cast<const char *>(data.data()), data.size());

        return stream;
    }
}
