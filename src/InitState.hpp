
#pragma once

#include "IGameState.hpp"

namespace ehb
{
    class InitState final : public IGameState
    {
    public:

        InitState(IGameStateMgr * gameStateMgr);

        virtual ~InitState() = default;

        virtual void enter() override;
        virtual void leave() override;
        virtual bool handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action) override;

    private:

        IGameStateMgr * gameStateMgr;
    };

    inline InitState::InitState(IGameStateMgr * gameStateMgr) : gameStateMgr(gameStateMgr)
    {
    }
}
