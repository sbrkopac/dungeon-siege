
#include "Game.hpp"

#include "InitState.hpp"
#include "IntroState.hpp"
#include "LogoState.hpp"

namespace ehb
{
    Game::Game(int argc, char * argv[]) : config(argc, argv), gameStateMgr(this)
    {
        proxy = new EventProxy(&gameStateMgr);
    }

    Game::~Game()
    {
    }

    IGameState * Game::createGameState(const std::string & gameStateType, IGameStateMgr * gameStateMgr)
    {
        if (gameStateType == "InitState")
        {
            return new InitState(gameStateMgr);
        }
        else if (gameStateType == "IntroState")
        {
            return new IntroState(gameStateMgr);
        }
        else if (gameStateType == "LogoState")
        {
            return new LogoState(gameStateMgr);
        }

        return nullptr;
    }

    int Game::exec()
    {
        viewer.addEventHandler(proxy);

        viewer.setUpViewInWindow(config.getInt("x"), config.getInt("y"), config.getInt("width", 640), config.getInt("height", 480));
        // viewer.setUpViewInWindow(0, 0, 640, 480);
        viewer.setReleaseContextAtEndOfFrameHint(false);
        viewer.realize();
        viewer.init();

        gameStateMgr.request("InitState");

        while (!viewer.done())
        {
            viewer.advance();

            viewer.eventTraversal();
            viewer.updateTraversal();
            viewer.renderingTraversals();
        }

        // TODO: return a non zero if something bad happens blah blah
        return 0;
    }
}
