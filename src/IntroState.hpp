
#pragma once

#include "IGameState.hpp"

namespace ehb
{
    class Shell;
    class IntroState final : public IGameState
    {
    public:

        IntroState(IGameStateMgr & gameStateMgr, Shell & gui);

        virtual ~IntroState() = default;

        virtual void enter() override;
        virtual void leave() override;
        virtual bool handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action) override;

    private:

        IGameStateMgr & gameStateMgr;
        Shell & gui;
    };

    inline IntroState::IntroState(IGameStateMgr & gameStateMgr, Shell & gui) : gameStateMgr(gameStateMgr), gui(gui)
    {
    }
}

