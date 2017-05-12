
#pragma once

#ifdef _DEBUG
    #define DEBUG_LOGGING
    #define SPDLOG_TRACE_ON
    #define SPDLOG_DEBUG_ON
#endif

#include <spdlog/spdlog.h>

// give the ability to disable all logging
#ifdef DISABLE_LOGGING
#ifdef DEBUG_LOGGING
#undef DEBUG_LOGGING
#endif
#define NO_LOGGING
#endif

// check if we should have no logging
#ifndef NO_LOGGING
    #define INFO_LOG(...) spdlog::get("info")->info(__VA_ARGS__);
    #define WARN_LOG(...) spdlog::get("warning")->warn(__VA_ARGS__);
    #define ERROR_LOG(...) spdlog::get("error")->error(__VA_ARGS__);
#else
    #define INFO_LOG(...)
    #define WARN_LOG(...)
    #define ERROR_LOG(...)
#endif

// handle debug logging
#ifdef DEBUG_LOGGING

    #define DEBUG_LOG(...) SPDLOG_DEBUG(spdlog::get("debug"), __VA_ARGS__)
    #define TRACE_LOG(...) SPDLOG_TRACE(spdlog::get("trace"), __VA_ARGS__)
#else
    #define DEBUG_LOG(...)
    #define TRACE_LOG(...)
#endif

namespace ehb
{
    struct Logging
    {
        static void init(const std::string & path);
    };
}
