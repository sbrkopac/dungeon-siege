
#pragma once

#include <memory>
#include <vector>
#include "IArchive.hpp"

namespace ehb
{
    class FileSys
    {
    public:

        //! Helper function to concat paths
        static std::string concatPaths(const std::string & left, const std::string & right);

    public:

        FileSys();

        //! Reads a file into data
        bool readFile(const std::string & filename, siege::ByteArray & data);

        //! reads a file into a stringstream
        bool readFile(const std::string & filename, std::stringstream & stream);

        //! Iterate over each file in a specific directory with the option the recurse
        void forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recursive = false);

#if 0
        //! Check if a file exists
        bool exists(const std::string & filename);

        //! Reads a gas file into gas
        bool readGasFile(const std::string & filename, siege::Gas & gas);
#endif

    private:

        std::vector<std::unique_ptr<IArchive>> eachArchive;

    };

    inline bool FileSys::readFile(const std::string & filename, siege::ByteArray & data)
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

    inline bool FileSys::readFile(const std::string & filename, std::stringstream & stream)
    {
        for (const auto & archive : eachArchive)
        {
            if (archive->readFile(filename, stream))
            {
                return true;
            }
        }

        return false;
    }
}
