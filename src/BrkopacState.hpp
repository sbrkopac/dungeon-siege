
#pragma once

#include "IGameState.hpp"

namespace osgViewer
{
    class Viewer;
}

namespace ehb
{
    class FileSys;
    class IConfig;
    class BrkopacState : public IGameState
    {
    public:

        BrkopacState(IGameStateMgr & gameStateMgr, IConfig & config, FileSys & fileSys, osgViewer::Viewer & viewer);

        virtual ~BrkopacState() = default;

        virtual void enter() override;
        virtual void leave() override;
        virtual bool handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action) override;

    private:

        IGameStateMgr & gameStateMgr;
        IConfig & config;
        FileSys & fileSys;
        osgViewer::Viewer & viewer;
    };

    inline BrkopacState::BrkopacState(IGameStateMgr & gameStateMgr, IConfig & config, FileSys & fileSys, osgViewer::Viewer & viewer) : gameStateMgr(gameStateMgr), config(config), fileSys(fileSys), viewer(viewer)
    {
    }
}
