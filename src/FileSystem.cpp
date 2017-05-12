
#include "FileSystem.hpp"

namespace ehb
{
    FileSystem::FileSystem()
    {
    }

    bool FileSystem::exists(const std::string & filename)
    {
        return false;
    }

    bool FileSystem::readFile(const std::string & filename, siege::ByteArray & data)
    {
        return false;
    }

    bool FileSystem::readGasFile(const std::string & filename, siege::Gas & gas)
    {
        return false;
    }

    void FileSystem::forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recurse)
    {
    }

    std::string FileSystem::concatPaths(const std::string & left, const std::string & right)
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

    std::stringstream FileSystem::convert(const siege::ByteArray & data)
    {
        std::stringstream stream;

        return stream;
    }
}