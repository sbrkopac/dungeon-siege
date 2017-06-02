
#pragma once

#include <string>
#include <unordered_map>
#include <osgDB/Callbacks>

// TODO: move nnk stuff out of this class??
namespace ehb
{
    class FileSys;
    class ReadFileProxy : public osgDB::ReadFileCallback
    {
        public:

            ReadFileProxy(FileSys & fileSys);

            virtual ~ReadFileProxy() = default;

            virtual osgDB::ReaderWriter::ReadResult readImage(const std::string & filename, const osgDB::Options * options) override;
            virtual osgDB::ReaderWriter::ReadResult readNode(const std::string & filename, const osgDB::Options * options) override;

        private:

            std::string prepareFilePath(const std::string &filename);

            FileSys & fileSys;
            std::unordered_map<std::string, std::string> namingKeyMap;
    };
}
