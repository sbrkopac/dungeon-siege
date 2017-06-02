
#pragma once

#include "IGameState.hpp"

namespace ehb
{
    class FileSys;
    class IConfig;
    class InitState final : public IGameState
    {
    public:

        InitState(IGameStateMgr & gameStateMgr, IConfig & config, FileSys & fileSys);

        virtual ~InitState() = default;

        virtual void enter() override;
        virtual void leave() override;
        virtual bool handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action) override;

    private:

        IGameStateMgr & gameStateMgr;
        IConfig & config;
        FileSys & fileSys;
    };

    inline InitState::InitState(IGameStateMgr & gameStateMgr, IConfig & config, FileSys & fileSys) : gameStateMgr(gameStateMgr), config(config), fileSys(fileSys)
    {
    }
}
