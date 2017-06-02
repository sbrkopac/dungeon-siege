
#pragma once

#include <osg/ref_ptr>
#include <osgViewer/Viewer>
#include "EventProxy.hpp"
#include "FileSys.hpp"
#include "GameStateMgr.hpp"
#include "IGameStateProvider.hpp"
#include "Shell.hpp"

namespace ehb
{
    class EventProxy;
    class Game final : public IGameStateProvider
    {
    public:

        Game(IConfig & config);

        virtual ~Game() = default;

        virtual IGameState * createGameState(const std::string & gameStateType, IGameStateMgr & gameStateMgr) override;

        int exec();

    private:

        // SimpleConfig config;
        IConfig & config;

        osg::ref_ptr<EventProxy> proxy;
        GameStateMgr gameStateMgr;
        osgViewer::Viewer viewer;
        FileSys fileSys;
        osg::ref_ptr<Shell> gui;
    };
}
