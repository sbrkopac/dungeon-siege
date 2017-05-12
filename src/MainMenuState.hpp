
#pragma once

#include "IGameState.hpp"

namespace ehb
{
    class MainMenuState final : public IGameState
    {
    public:

        MainMenuState(IGameStateMgr * gameStateMgr)

        virtual ~MainMenuState() = default;

        virtual void enter() override;
        virtual void leave() override;
        virtual bool handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action) override;

    private:

        IGameStateMgr * gameStateMgr;
    };

    inline MainMenuState::MainMenuState(IGameStateMgr * gameStateMgr) : gameStateMgr(gameStateMgr)
    {
    }
}

