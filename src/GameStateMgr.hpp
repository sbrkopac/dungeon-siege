
#pragma once

#include <memory>
#include "IGameState.hpp"
#include "IGameStateMgr.hpp"

namespace osgGA
{
    class GUIActionAdapter;
    class GUIEventAdapter;
}

namespace ehb
{
    class IGameStateProvider;
    class GameStateMgr final : public IGameStateMgr
    {
    public:

        GameStateMgr(IGameStateProvider * provider);

        virtual ~GameStateMgr();

        virtual void request(const std::string & gameStateType) override;

        bool handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action);

    private:

        IGameStateProvider * provider;

        std::unique_ptr<IGameState> currentState;
        std::unique_ptr<IGameState> pendingState;
    };
}
