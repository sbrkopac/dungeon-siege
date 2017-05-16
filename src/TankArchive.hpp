
#pragma once

#include <siege/tank_file.hpp>
#include "IArchive.hpp"

namespace ehb
{
    class TankArchive : public IArchive
    {
    public:

        virtual ~TankArchive();

        virtual bool open(const std::string & filename) override;

        virtual void close() override;

        virtual bool readFile(const std::string & filename, siege::ByteArray & data) override;

        virtual void forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recursive = false) override;

        const siege::TankFile::Header & getFileHeader() const;

    private:

        siege::TankFile tankFile;
        siege::TankFile::Reader reader;
    };

    inline const siege::TankFile::Header & TankArchive::getFileHeader() const
    {
        return tankFile.getFileHeader();
    }
}
