
#pragma once

#include <map>
#include <string>
#include "IConfig.hpp"

namespace ehb
{
    class SimpleConfig final : public IConfig
    {
    public:

        SimpleConfig(int * argc, char * argv[]);

        virtual ~SimpleConfig() = default;

        virtual bool getBool(const std::string & key, bool defaultValue = false) override;
        virtual float getFloat(const std::string & key, float defaultValue = 0.0f) override;
        virtual int getInt(const std::string & key, int defaultValue = 0) override;
        virtual std::string getString(const std::string & key, const std::string & defaultValue = "") override;

        void setBool(const std::string & key, bool value);
        void setFloat(const std::string & key, float value);
        void setInt(const std::string & key, int value);
        void setString(const std::string & key, const std::string & value);

    private:

        std::map<std::string, bool> boolMap;
        std::map<std::string, float> floatMap;
        std::map<std::string, int> intMap;
        std::map<std::string, std::string> stringMap;
    };
}
