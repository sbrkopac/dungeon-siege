
#include <osgDB/fstream>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include "DirectoryArchive.hpp"

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
            // TODO: store archive file name
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

    static
    void forEachFileRecursive(const std::string & basepath, const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recursive)
    {
        const std::string fullpath = osgDB::concatPaths(basepath, directory);

        for (const std::string & filename : osgDB::getDirectoryContents(fullpath))
        {
            if (filename == "." || filename == "..") continue;

            const osgDB::FileType fileType = osgDB::fileType(filename);

            callback(filename, fileType);

            if (fileType == osgDB::DIRECTORY)
            {
                forEachFileRecursive(basepath, fullpath, callback, recursive);
            }
        }
    }

    void DirectoryArchive::forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recursive)
    {
        const std::string fullpath = osgDB::concatPaths(root, directory);

        forEachFileRecursive(fullpath, "", callback, recursive);
    }
}
