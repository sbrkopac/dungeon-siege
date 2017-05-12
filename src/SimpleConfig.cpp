
#include "SimpleConfig.hpp"

namespace ehb
{
    SimpleConfig::SimpleConfig(int argc, char * argv[])
    {
    }

    bool SimpleConfig::getBool(const std::string & key, bool defaultValue)
    {
        const auto itr = boolMap.find(key);

        return itr != boolMap.end() ? itr->second : defaultValue;
    }

    float SimpleConfig::getFloat(const std::string & key, float defaultValue)
    {
        const auto itr = floatMap.find(key);

        return itr != floatMap.end() ? itr->second : defaultValue;
    }

    int SimpleConfig::getInt(const std::string & key, int defaultValue)
    {
        const auto itr = intMap.find(key);

        return itr != intMap.end() ? itr->second : defaultValue;
    }

    std::string SimpleConfig::getString(const std::string & key, const std::string & defaultValue)
    {
        const auto itr = stringMap.find(key);

        return itr != stringMap.end() ? itr->second : defaultValue;
    }

    void SimpleConfig::setBool(const std::string & key, bool value)
    {
        boolMap[key] = value;
    }

    void SimpleConfig::setFloat(const std::string & key, float value)
    {
        floatMap[key] = value;
    }

    void SimpleConfig::setInt(const std::string & key, int value)
    {
        intMap[key] = value;
    }

    void SimpleConfig::setString(const std::string & key, const std::string & value)
    {
        stringMap[key] = value;
    }
}
