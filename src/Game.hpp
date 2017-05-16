
#pragma once

#include <osg/ref_ptr>
#include <osgViewer/Viewer>
#include "EventProxy.hpp"
#include "FileSys.hpp"
#include "GameStateMgr.hpp"
#include "IGameStateProvider.hpp"
#include "SimpleConfig.hpp"

namespace ehb
{
    class EventProxy;
    class Game final : public IGameStateProvider
    {
    public:

        Game(int * argc, char * argv[]);

        virtual ~Game();

        virtual IGameState * createGameState(const std::string & gameStateType, IGameStateMgr * gameStateMgr) override;

        int exec();

    private:

        SimpleConfig config;

        osg::ref_ptr<EventProxy> proxy;
        GameStateMgr gameStateMgr;
        osgViewer::Viewer viewer;
        FileSys fileSys;
    };
}
