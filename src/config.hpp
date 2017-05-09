
#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

namespace ehb
{
    class Config
    {
        public:

            Config(int * argc, char * argv[]);
           ~Config();

            bool getBool(const std::string & key, bool defaultValue) const;
            float getFloat(const std::string & key, float defaultValue) const;
            int getInt(const std::string & key, int defaultValue) const;
            const std::string & getString(const std::string & key, const std::string & defaultValue) const;

            void dump();
            void dump(std::ostream & stream);

        private:

            std::unordered_map<std::string, bool> boolMap;
            std::unordered_map<std::string, float> floatMap;
            std::unordered_map<std::string, int> intMap;
            std::unordered_map<std::string, std::string> stringMap;
    };

    inline bool Config::getBool(const std::string & key, bool defaultValue) const
    {
        const auto itr = boolMap.find(key);

        return (itr != boolMap.end()) ? itr->second : defaultValue;
    }

    inline float Config::getFloat(const std::string & key, float defaultValue) const
    {
        const auto itr = floatMap.find(key);

        return (itr != floatMap.end()) ? itr->second : defaultValue;
    }

    inline int Config::getInt(const std::string & key, int defaultValue) const
    {
        const auto itr = intMap.find(key);

        return (itr != intMap.end()) ? itr->second : defaultValue;
    }

    inline const std::string & Config::getString(const std::string & key, const std::string & defaultValue) const
    {
        const auto itr = stringMap.find(key);

        return (itr != stringMap.end()) ? itr->second : defaultValue;
    }

    inline void Config::dump()
    {
        dump(std::cout);
    }
}
