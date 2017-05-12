
#pragma once

#include "IGameState.hpp"

namespace ehb
{
    class LogoState final : public IGameState
    {
    public:

        LogoState(IGameStateMgr * gameStateMgr);

        virtual ~LogoState() = default;

        virtual void enter() override;
        virtual void leave() override;
        virtual bool handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action) override;

    private:

        IGameStateMgr * gameStateMgr;
    };

    inline LogoState::LogoState(IGameStateMgr * gameStateMgr) : gameStateMgr(gameStateMgr)
    {
    }
}

