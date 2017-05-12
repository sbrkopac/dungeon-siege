
#pragma once

#include "IGameState.hpp"

namespace ehb
{
    class IntroState final : public IGameState
    {
    public:

        IntroState(IGameStateMgr * gameStateMgr);

        virtual ~IntroState() = default;

        virtual void enter() override;
        virtual void leave() override;
        virtual bool handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action) override;

    private:

        IGameStateMgr * gameStateMgr;
    };

    inline IntroState::IntroState(IGameStateMgr * gameStateMgr) : gameStateMgr(gameStateMgr)
    {
    }
}

