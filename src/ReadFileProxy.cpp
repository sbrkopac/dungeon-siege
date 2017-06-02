
#include <list>
#include <set>
#include <spdlog/spdlog.h>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include "FileSys.hpp"
#include "ReadFileProxy.hpp"

namespace ehb
{
    // TODO: replace this with the osgDB::Registry cache
    static std::map<std::string, osgDB::ReaderWriter::ReadResult> cache;

    // TODO: make a real nnk parser
    static std::string trim(const std::string & str)
    {
        if (!str.size()) return str;
        std::string::size_type first = str.find_first_not_of( " \t" );
        std::string::size_type last = str.find_last_not_of( "  \t\r\n" );
        if ((first==str.npos) || (last==str.npos)) return std::string( "" );
        return str.substr( first, last-first+1 );
    }

    static void parseTree(std::unordered_map<std::string, std::string> & namingKeyMap, std::istream & stream)
    {
        for (std::string line; std::getline(stream, line);)
        {
            line = trim(line);

            if (line.empty())
            {
                continue;
            }

            if (line.front() != '#')
            {
                std::stringstream lineStream(line);

                std::string type, key, value, description;

                std::getline(lineStream, type, '=');
                std::getline(lineStream, key, ',');
                std::getline(lineStream, value, ',');
                std::getline(lineStream, description, ',');

                type = trim(type);
                key = trim(key);
                value = trim(value);

                if (type == "TREE")
                {
                    std::replace(value.begin(), value.end(), '\\', '/');

                    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
                    std::transform(value.begin(), value.end(), value.begin(), ::tolower);

                    auto index = key.find_last_of('_');

                    std::string fullFileName;

                    if (index != std::string::npos)
                    {
                        auto itr = namingKeyMap.find(key.substr(0, index));

                        if (itr != namingKeyMap.end()) {

                            fullFileName += itr->second;
                            fullFileName += value;
                            fullFileName += '/';

                            namingKeyMap.emplace(key, fullFileName);

                            // std::cout << "storing '" << key << " as '" << fullFileName << "'" << std::endl;

                            continue;
                        }
                    }

                    fullFileName += value;

                    if (!fullFileName.empty())
                    {
                        fullFileName += '/';
                    }

                    namingKeyMap.emplace(key, fullFileName);

                    // std::cout << "storing '" << key << " as '" << fullFileName << "'" << std::endl;
                }
            }
        }
    }

    std::string ReadFileProxy::prepareFilePath(const std::string &filename)
    {
        std::string actualFileName = filename;

        if (filename.find_first_of('/') == std::string::npos)
        {
            std::string resolvedFileName;

            for (std::string::size_type index = filename.rfind('_'); index != std::string::npos && index != 0; index = filename.rfind('_', index - 1))
            {
                auto itr = namingKeyMap.find(filename.substr(0, index));

                if (itr != namingKeyMap.end())
                {
                    resolvedFileName = itr->second + filename;
                    break;
                }
            }

            if (!resolvedFileName.empty())
            {
                const std::string prefix = filename.substr(0, 2);

                if (prefix == "a_" || prefix == "b_" || prefix == "m_" || prefix == "t_")
                {
                    actualFileName.clear();

                    actualFileName += "/art/";
                    actualFileName += resolvedFileName;
                }
            }
        }

        return actualFileName;
    }

    ReadFileProxy::ReadFileProxy(FileSys & fileSys) : fileSys(fileSys)
    {
        std::set<std::string> std, ext;

        fileSys.forEachFile("/art", [&std, &ext](const std::string & filename, osgDB::FileType fileType)
        {
            auto log = spdlog::get("log");

            if (osgDB::getLowerCaseFileExtension(filename) == "nnk")
            {
                const std::string lowerCaseFileName = osgDB::convertToLowerCase(filename);

                if (lowerCaseFileName.compare(0, 9, "namingkey") == 0)
                {
                    std.emplace(filename);

                    log->info("added standard naming key file {}", filename);
                }
                else
                {
                    ext.emplace(filename);

                    log->info("added extension naming key file {}", filename);
                }
            }
        });

        std::list<std::string> eachFileName;

        for (auto itr = std.rbegin(); itr != std.rend(); ++itr)
        {
            std::string filename;

            filename += "/art/";
            filename += *itr;

            eachFileName.push_back(filename);

            break;
        }

        for (auto itr = ext.begin(); itr != ext.end(); ++itr)
        {
            std::string filename;

            filename += "/art/";
            filename += *itr;

            eachFileName.push_back(filename);
        }

        for (const std::string & filename : eachFileName)
        {
            std::stringstream stream;

            if (fileSys.readFile(filename, stream))
            {
                // process it...
                parseTree(namingKeyMap, stream);
            }
        }
    }

    osgDB::ReaderWriter::ReadResult ReadFileProxy::readImage(const std::string & filename, const osgDB::Options * options)
    {
        std::string actualFileName = prepareFilePath(filename);

        //std::cout << "ACTUALFILENAME = " << actualFileName << std::endl;

        osgDB::ReaderWriter::ReadResult result = osgDB::ReaderWriter::ReadResult::FILE_NOT_FOUND;

        auto cached = cache.find(filename);

        if (cached == cache.end())
        {
            if (osgDB::ReaderWriter * rw = osgDB::Registry::instance()->getReaderWriterForExtension(osgDB::getFileExtension(filename)))
            {
                std::stringstream stream;

                if (fileSys.readFile(actualFileName, stream))
                {
                    result = rw->readImage(stream, options);

                    cache.emplace(filename, result);
                }
                else
                {
                    std::cout << "ReadFileProxy failed to read file " << actualFileName << std::endl;
                }
            }
            else { std::cout << "FAILED TO FIND READERWRITER FOR FILE " << filename << std::endl; }
        }
        else // found result
        {
            result = cached->second;
        }

        return result;
    }

    osgDB::ReaderWriter::ReadResult ReadFileProxy::readNode(const std::string & filename, const osgDB::Options * options)
    {
        std::string actualFileName = prepareFilePath(filename);

        // std::cout << "ACTUALFILENAME = " << actualFileName << std::endl;

        osgDB::ReaderWriter::ReadResult result = osgDB::ReaderWriter::ReadResult::FILE_NOT_FOUND;

        auto cached = cache.find(filename);

        if (cached == cache.end())
        {
            if (osgDB::ReaderWriter * rw = osgDB::Registry::instance()->getReaderWriterForExtension(osgDB::getFileExtension(filename)))
            {
                // std::cout << "FOUND READERWRITER!: " << rw->getCompoundClassName() << std::endl;
                std::stringstream stream;

                if (fileSys.readFile(actualFileName, stream))
                {
                    result = rw->readNode(stream, options);

                    // TODO: caching
                }
                else
                {
                    // std::cout << "ReadFileProxy failed to read file " << actualFileName << std::endl;
                }
            }
        }
        else // found result in cache
        {
            result = cached->second;
        }

        return result;
    }
}
