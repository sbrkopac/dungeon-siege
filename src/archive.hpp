
#pragma once

#include <functional>
#include <string>
#include <osg/Referenced>
#include <osgDB/FileUtils>
#include <siege/tank_file.hpp>

namespace ehb
{
    class Archive : public osg::Referenced
    {
        public:

            // TODO: make this a member variable or something...
            std::string archiveFileName;

        public:

            virtual ~Archive()
            {
            }

            virtual bool open(const std::string & filename) = 0;

            virtual void close() = 0;

            virtual const std::string & getFileName() const = 0;

            virtual bool readFile(const std::string & filename, siege::ByteArray & data) = 0;

            virtual void forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recurse = false) = 0;
    };
}
