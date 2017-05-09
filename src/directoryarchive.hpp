
#pragma once

#include "archive.hpp"

namespace ehb
{
    class DirectoryArchive : public Archive
    {
        public:

            virtual ~DirectoryArchive();

            virtual bool open(const std::string & filename) override;

            virtual void close() override;

            virtual const std::string & getFileName() const override;

            virtual bool readFile(const std::string & filename, siege::ByteArray & data) override;

            virtual void forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recurse = false) override;

        private:

            std::string root;
    };

    inline const std::string & DirectoryArchive::getFileName() const
    {
        return root;
    }
}
