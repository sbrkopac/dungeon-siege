
#include "tankarchive.hpp"

#include <iostream>

namespace ehb
{
    TankArchive::~TankArchive()
    {
        close();
    }

    bool TankArchive::open(const std::string & filename)
    {
        try
        {
            tankFile.openForReading(filename);
            reader.indexFile(tankFile);

            archiveFileName = filename;

            return true;
        }
        catch (siege::TankFile::Error & e)
        {
            std::cout << "TankArchive::open(" << filename << "): " << e.what() << std::endl;
        }

        return false;
    }

    void TankArchive::close()
    {
        try
        {
            tankFile.close();
        }
        catch (siege::TankFile::Error & e)
        {
        }
    }

    bool TankArchive::readFile(const std::string & filename, siege::ByteArray & data)
    {
        // TODO: cleanup filename

        try
        {
            data = reader.extractResourceToMemory(tankFile, filename, false);

            // TODO: is this needed?
            data.push_back(0);

            return true;
        }
        catch (siege::TankFile::Error & e)
        {
            // std::cout << "TankArchive::readFile(" << filename << "): " << e.what() << std::endl;
        }

        return false;
    }

    // FIXME: if "directory" has a trailing / this falls apart
    void TankArchive::forEachFile(const std::string & directory, std::function<void(const std::string &, osgDB::FileType)> callback, bool recurse)
    {
        if (recurse)
        {
            for (std::string filename : reader.getDirectoryList())
            {
                if (filename.back() == '/')
                {
                    filename.pop_back();
                }

                if (filename.size() > directory.size() && filename.compare(0, directory.size(), directory) == 0)
                {
                    callback(filename.substr(directory.size() + 1), osgDB::DIRECTORY);
                }
            }

            for (const std::string & filename : reader.getFileList())
            {
                if (filename.size() > directory.size() && filename.compare(0, directory.size(), directory) == 0)
                {
                    callback(filename.substr(directory.size() + 1), osgDB::REGULAR_FILE);
                }
            }
        }
        else
        {
            for (std::string filename : reader.getDirectoryList())
            {
                if (filename.back() == '/')
                {
                    filename.pop_back();
                }

                const auto pos = filename.find_last_of('/');

                if (pos == 0)
                {
                    if (directory == "/")
                    {
                        callback(filename.substr(1), osgDB::DIRECTORY);
                    }
                }
                else if (pos != std::string::npos)
                {
                    // if (directory == filename.substr(0, pos))
                    if (filename.compare(0, pos, directory) == 0)
                    {
                        callback(filename.substr(pos + 1), osgDB::DIRECTORY);
                    }
                }
            }

            for (const std::string & filename : reader.getFileList())
            {
                const auto pos = filename.find_last_of('/');

                if (pos == 0)
                {
                    if (directory == "/")
                    {
                        callback(filename.substr(1), osgDB::REGULAR_FILE);
                    }
                }
                else if (pos != std::string::npos)
                {
                    // if (filename.substr(0, pos) == directory)
                    if (filename.compare(0, pos, directory) == 0)
                    {
                        callback(filename.substr(pos + 1), osgDB::REGULAR_FILE);
                    }
                }
            }
        }
    }
}
