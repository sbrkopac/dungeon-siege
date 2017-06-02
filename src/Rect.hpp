
#pragma once

#include <sstream>
#include <string>
#include <cstdio>

namespace ehb
{
    struct Rect
    {
        int x1, y1, x2, y2;
    };

    struct NormalizedRect
    {
        float left;
        float right;
        float top;
        float bottom;

    public:

        NormalizedRect() : left(0.f), right(1.f), top(0.f), bottom(1.f)
        {
        }
    };

    inline bool fromString(const std::string & value, Rect & result)
    {
        return std::sscanf(value.c_str(), "%d,%d,%d,%d", &result.x1, &result.y1, &result.x2, &result.y2) == 4;
    }

    inline std::string toString(const Rect & value)
    {
        std::stringstream stream;

        stream << value.x1 << "," << value.y1 << "," << value.x2 << "," << value.y2;

        return stream.str();
    }

    inline bool fromString(const std::string & value, NormalizedRect & result)
    {
        return std::sscanf(value.c_str(), "%f,%f,%f,%f", &result.left, &result.bottom, &result.right, &result.top) == 4;
    }

    inline std::string toString(const NormalizedRect & value)
    {
        std::stringstream stream;

        stream << value.left << "," << value.bottom << "," << value.right << "," << value.top;

        return stream.str();
    }
}
