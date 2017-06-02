
#pragma once

#include <memory>
#include <vector>
#include "gas.hpp"
#include "IArchive.hpp"

namespace ehb
{
    class IConfig;
    class FileSys
    {
    public:

        //! Helper function to concat paths
        static std::string concatPaths(const std::string & left, const std::string & right);

    public:

        FileSys(IConfig & config);

        //! Reads a file into data
        bool readFile(const std::string & filename, siege::ByteArray & data);

        //! reads a file into a stringstream
        bool readFile(const std::string & filename, std::stringstream & stream);

        //! reads a gas file
        bool readFile(const std::string & filename, Gas & doc);

        //! Iterate over each file in a specific directory with the option the recurse
        void forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recursive = false);

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

    inline bool FileSys::readFile(const std::string & filename, Gas & doc)
    {
        for (const auto & archive : eachArchive)
        {
            std::stringstream stream;

            if (archive->readFile(filename, stream))
            {
                return doc.load(stream);
            }
        }

        return false;
    }
}
