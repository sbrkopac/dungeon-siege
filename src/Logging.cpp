
#include "Logging.hpp"

#include <osgDB/FileUtils>

namespace ehb
{    
    std::shared_ptr <spdlog::logger> gFallbackLog = spdlog::basic_logger_mt("fallback", "fallback.log");

    void Logging::init(const std::string & path)
    {
        // sanity check for logs folder
        if (!osgDB::fileExists(path + "/Logs"))
        {
            osgDB::makeDirectory(path + "/Logs");
        }

        // setup all relevant loggers based on defines

    #ifndef NO_LOGGING
        std::shared_ptr<spdlog::logger> info = spdlog::basic_logger_mt("info", path + "/Logs/info.log");
        std::shared_ptr<spdlog::logger> warn = spdlog::basic_logger_mt("warning", path + "/Logs/warning.log");
        std::shared_ptr<spdlog::logger> error = spdlog::basic_logger_mt("error", path + "/Logs/error.log");
    #endif

    #ifdef DEBUG_LOGGING
        spdlog::set_level(spdlog::level::trace);

        std::shared_ptr<spdlog::logger> debug = spdlog::basic_logger_mt("debug", path + "/Logs/debug.log");
        std::shared_ptr<spdlog::logger> trace = spdlog::basic_logger_mt("trace", path + "/Logs/trace.log");
    #endif

        // set the global pattern... dungeon siege style
        spdlog::set_pattern("+%H:%M:%S:%e - %v");

        gLoggingReady = true;
    }

    bool Logging::ready()
    {
        return gLoggingReady;
    }
}