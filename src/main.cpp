
#include <spdlog/spdlog.h>
#include <osgDB/FileNameUtils>
#include "Game.hpp"
#include "SimpleConfig.hpp"

int main(int argc, char * argv[])
{
    ehb::SimpleConfig config(&argc, argv);

    const std::string logsDir = osgDB::concatPaths(config.getString("data-dir"), "Logs");

    if (osgDB::makeDirectory(logsDir))
    {
        // log to both file and stdout
        std::vector<spdlog::sink_ptr> sinks =
        {
            std::make_shared<spdlog::sinks::stdout_sink_mt>(),
            std::make_shared<spdlog::sinks::simple_file_sink_mt>(osgDB::concatPaths(logsDir, "log.txt"))
        };

        spdlog::register_logger(std::make_shared<spdlog::logger>("log", sinks.begin(), sinks.end()));
    }
    else
    {
        // unable to log to file, fallback to stdout only
        spdlog::stdout_color_mt("log")->error("unable to create {} directory for logging", logsDir);
    }

    return ehb::Game(config).exec();
}
