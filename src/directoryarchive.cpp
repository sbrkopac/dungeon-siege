
#include <osgDB/fstream>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include "directoryarchive.hpp"

namespace ehb
{
    DirectoryArchive::~DirectoryArchive()
    {
        close();
    }

    bool DirectoryArchive::open(const std::string & filename)
    {
        if (osgDB::fileType(filename) == osgDB::DIRECTORY)
        {
            root = filename;
            archiveFileName = filename;

            return true;
        }

        return false;
    }

    void DirectoryArchive::close()
    {
        root.clear();
    }

    bool DirectoryArchive::readFile(const std::string & filename, siege::ByteArray & data)
    {
        const std::string fullpath = osgDB::concatPaths(root, filename);

        osgDB::ifstream stream(fullpath.c_str(), std::ios::binary | std::ios::ate);

        if (stream.good())
        {
            data.resize(stream.tellg());

            stream.seekg(std::ios::beg);

            if (stream.read(reinterpret_cast<char *>(data.data()), data.size()))
            {
                return true;
            }
        }

        return false;
    }

    void forEachFileRecursive(const std::string & basepath, const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recurse)
    {
        const std::string fullpath = osgDB::concatPaths(basepath, directory);

        for (const std::string & filename : osgDB::getDirectoryContents(fullpath))
        {
            if (filename == "." || filename == "..") continue;

            const osgDB::FileType fileType = osgDB::fileType(filename);

            callback(filename, fileType);

            if (fileType == osgDB::DIRECTORY)
            {
                forEachFileRecursive(basepath, fullpath, callback, recurse);
            }
        }
    }

    void DirectoryArchive::forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recurse)
    {
        const std::string fullpath = osgDB::concatPaths(root, directory);

        forEachFileRecursive(fullpath, "", callback, recurse);

#if 0
        for (const std::string & filename : osgDB::getDirectoryContents(fullpath))
        {
            if (filename == "." || filename == "..") continue;

            const osgDB::FileType fileType = osgDB::fileType(filename);

            callback(filename, fileType);
        }
#endif
    }
}
