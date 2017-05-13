
#pragma once

#ifdef _DEBUG
    #define DEBUG_LOGGING
    #define SPDLOG_TRACE_ON
    #define SPDLOG_DEBUG_ON
#endif

#include <sstream>
#include <spdlog/spdlog.h>

namespace ehb
{
    static bool gLoggingReady = false;

    struct Logging
    {
        static void init(const std::string & path);

        static bool ready();
    };
}

// give the ability to disable all logging
#ifdef DISABLE_LOGGING
    #ifdef DEBUG_LOGGING
    #undef DEBUG_LOGGING
    #endif
    #define NO_LOGGING
#endif

// check if we should have no logging
#ifndef NO_LOGGING
    #define INFO_LOG(...) if (Logging::ready()) { spdlog::get("info")->info(__VA_ARGS__); } else { spdlog::get("fallback")->info(__VA_ARGS__); }
    #define WARN_LOG(...) if (Logging::ready()) { spdlog::get("warning")->warn(__VA_ARGS__); } else { spdlog::get("fallback")->info(__VA_ARGS__); }
    #define ERROR_LOG(...) if (Logging::ready()) { spdlog::get("error")->error(__VA_ARGS__); } else { spdlog::get("fallback")->info(__VA_ARGS__); }
#else
    #define INFO_LOG(...)
    #define WARN_LOG(...)
    #define ERROR_LOG(...)
#endif

// handle debug logging
#ifdef DEBUG_LOGGING
    #define DEBUG_LOG(...) if (Logging::ready()) { SPDLOG_DEBUG(spdlog::get("debug"), __VA_ARGS__); } else { spdlog::get("fallback")->info(__VA_ARGS__); }
    #define TRACE_LOG(...) if (Logging::ready()) { SPDLOG_TRACE(spdlog::get("trace"), __VA_ARGS__); } else { spdlog::get("fallback")->info(__VA_ARGS__); }
#else
    #define DEBUG_LOG(...)
    #define TRACE_LOG(...)
#endif
