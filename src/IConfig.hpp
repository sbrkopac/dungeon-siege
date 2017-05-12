
#pragma once

#include <string>

namespace ehb
{
    class IConfig
    {
    public:

        virtual ~IConfig() = default;

        virtual bool getBool(const std::string & key, bool defaultValue = false) = 0;
        virtual float getFloat(const std::string & key, float defaultValue = 0.0f) = 0;
        virtual int getInt(const std::string & key, int defaultValue = 0) = 0;
        virtual std::string getString(const std::string & key, const std::string & defaultValue = "") = 0;
    };
}
