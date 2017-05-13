
#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <siege/common.hpp>
#include <osgDB/FileUtils>

namespace ehb
{
    class IArchive
    {
    public:

        virtual ~IArchive() = default;

        virtual bool open(const std::string & filename) = 0;

        virtual void close() = 0;

        virtual bool readFile(const std::string & filename, siege::ByteArray & data) = 0;

        virtual void forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recursive = false) = 0;

        bool readFile(const std::string & filename, std::stringstream & stream);
    };

    inline bool IArchive::readFile(const std::string & filename, std::stringstream & stream)
    {
        siege::ByteArray data;

        if (readFile(filename, data))
        {
            stream.write(reinterpret_cast<const char *>(data.data()), data.size());

            return true;
        }

        return false;
    }
}
