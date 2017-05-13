
#include <set>
#include "FileSys.hpp"

namespace ehb
{
    FileSys::FileSys()
    {
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
