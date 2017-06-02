
#pragma once

#include <string>

namespace ehb
{
    class IGameStateMgr
    {
    public:

        virtual ~IGameStateMgr() = default;

        virtual void request(const std::string & gameStateType) = 0;
    };
}
