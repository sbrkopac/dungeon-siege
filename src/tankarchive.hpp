
#pragma once

#include "archive.hpp"

namespace ehb
{
    class TankArchive : public Archive
    {
        // TODO: caching???

        public:

            virtual ~TankArchive();

            virtual bool open(const std::string & filename) override;

            virtual void close() override;

            virtual const std::string & getFileName() const override;

            virtual bool readFile(const std::string & filename, siege::ByteArray & data) override;

            virtual void forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recurse = false) override;

            const siege::TankFile::Header & getFileHeader() const;

        private:

            siege::TankFile tankFile;
            siege::TankFile::Reader reader;
    };

    inline const std::string & TankArchive::getFileName() const
    {
        return tankFile.getFileName();
    }

    inline const siege::TankFile::Header & TankArchive::getFileHeader() const
    {
        return tankFile.getFileHeader();
    }
}
