
#pragma once

#include <functional>

#include <osgDB/FileUtils>

namespace siege
{
    class Gas;
    class ByteArray;
}

namespace ehb
{
    class FileSystem
    {
        public:

            FileSystem & getInstance();

            //! Check if a file exists
            bool exists(const std::string & filename);

            //! Reads a file into data
            bool readFile(const std::string & filename, siege::ByteArray & data);

            //! Reads a gas file into gas
            bool readGasFile(const std::string & filename, siege::Gas & gas);

            //! Iterate over each file in a specific directory with the option the recurse
            void forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recurse = false);

            //! Helper function to concat paths
            static std::string concatPaths(const std::string & left, const std::string & right);

        private:

            FileSystem();

            //! Helper function to convert a siege::ByteArray into a std::stringstream
            std::stringstream FileSystem::convert(const siege::ByteArray & data);

    };

    inline FileSystem & FileSystem::getInstance()
    {
        static FileSystem instance;
        return instance;
    }
}