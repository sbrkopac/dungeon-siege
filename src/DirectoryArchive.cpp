
#include <osgDB/fstream>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include "DirectoryArchive.hpp"

#include <experimental/filesystem>

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

    void DirectoryArchive::forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recursive)
    {
        const std::string fullpath = osgDB::concatPaths(root, !directory.empty() && directory[0] == '/' ? directory.substr(1) : directory);

        if (recursive)
        {
            namespace fs = std::experimental::filesystem;

            try
            {
                const auto size = fullpath.size();

                for (auto & p: fs::recursive_directory_iterator(fullpath))
                {
                    // FIXME: this won't work properly on windows with unicode file names...
                    const std::string filename = p.path().string();

                    callback(osgDB::convertFileNameToUnixStyle(filename.substr(size + 1)), osgDB::fileType(filename));
                }
            }
            catch (std::exception & e)
            {
                // TODO: log a warning
            }
        }
        else
        {
            for (const std::string & filename : osgDB::getDirectoryContents(fullpath))
            {
                if (filename == "." || filename == "..") continue;

                callback(osgDB::convertFileNameToUnixStyle(filename), osgDB::fileType(osgDB::concatPaths(fullpath, filename)));
            }
        }
    }
}
