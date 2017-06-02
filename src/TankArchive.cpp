
#include <spdlog/spdlog.h>
#include "TankArchive.hpp"

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

            directoryCache.clear();
            directoryCache.reserve(reader.getDirectoryCount());

            fileCache.clear();
            fileCache.reserve(reader.getFileCount());

            for (std::string filename : reader.getDirectoryList())
            {
                if (filename.front() != '/') filename.insert(0, 1, '/'); // this is always true
                if (filename.back() == '/') filename.pop_back();

                directoryCache.push_back(filename);
            }

            for (const std::string & filename : reader.getFileList())
            {
                fileCache.push_back(filename);
            }

            return true;
        }
        catch (siege::TankFile::Error & e)
        {
            spdlog::get("log")->error("TankArchive::open({}): {}", filename, e.what());
        }

        return false;
    }

    void TankArchive::close()
    {
        directoryCache.clear();
        fileCache.clear();

        try
        {
            tankFile.close();
        }
        catch (siege::TankFile::Error & e)
        {
            spdlog::get("log")->warn("TankArchive::close(): {}", e.what());
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
            // NOTE: don't log anything here... this'll happen *all* the time
            // spdlog::get("log")->error("TankArchive::readFile({}): {}", filename, e.what());
        }

        return false;
    }

    void TankArchive::forEachFile(const std::string & directory_, std::function<void(const std::string &, osgDB::FileType)> callback, bool recursive)
    {
        std::string directory = directory_;

        // make the directory conform to what i'm expecting
        if (directory.front() != '/') directory.insert(0, 1, '/');
        if (directory.back() != '/') directory.push_back('/');

        for (std::string filename : directoryCache)
        {
            if (filename.size() > directory.size())
            {
                if (directory == filename.substr(0, directory.size()))
                {
                    const auto pos = filename.find('/', directory.size());

                    // NOTE: don't change the order of this
                    if (recursive || pos == std::string::npos)
                    {
                        callback(filename.substr(directory.size()), osgDB::DIRECTORY);
                    }
                }
            }
        }

        for (const std::string & filename : fileCache)
        {
            if (filename.size() > directory.size())
            {
                if (directory == filename.substr(0, directory.size()))
                {
                    const auto pos = filename.find('/', directory.size());

                    // NOTE: don't change the order of this
                    if (recursive || pos == std::string::npos)
                    {
                        callback(filename.substr(directory.size()), osgDB::REGULAR_FILE);
                    }
                }
            }
        }
    }
}
