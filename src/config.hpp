
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

            static bool getBool(const std::string & key, bool defaultValue);
            static float getFloat(const std::string & key, float defaultValue);
            static int getInt(const std::string & key, int defaultValue);
            static const std::string & getString(const std::string & key, const std::string & defaultValue);

            static void dump();
            static void dump(std::ostream & stream);
    };

    inline void Config::dump()
    {
        dump(std::cout);
    }
}
