
#pragma once

#include <string>
#include <vector>
#include <osg/ref_ptr>
#include "archive.hpp"
#include "gas.hpp"

namespace ehb
{
    class MasterFileIndex
    {
        public:

            MasterFileIndex();

            bool readFile(const std::string & filename, siege::ByteArray & data);

            bool readGasFile(const std::string & filename, siege::Gas & gas);

            void forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recurse = false);

            static std::string concatPaths(const std::string & left, const std::string & right);

            // oy vey...
            static std::stringstream convertForGas(const siege::ByteArray & data);

        private:

            std::vector<osg::ref_ptr<Archive>> eachArchive;

            // TODO: caching?
            // std::unordered_map<std::string
    };
}
