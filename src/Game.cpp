
#include <osg/Timer>
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
            return new InitState(gameStateMgr, &config, &fileSys);
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

        if (config.getBool("fullscreen"))
        {
            viewer.setUpViewOnSingleScreen();
        }
        else
        {
            int defaultX = 0, defaultY = 0;

            int actualWidth = config.getInt("width", 640);
            int actualHeight = config.getInt("height", 480);

            // calculate the coordinates which would put the window at the center of the screen
            if (osg::GraphicsContext::WindowingSystemInterface * wsi = osg::GraphicsContext::getWindowingSystemInterface())
            {
                osg::GraphicsContext::ScreenSettings resolution;

                // TODO: consider not hard coding to a specific monitor
                wsi->getScreenSettings(0, resolution);

                defaultX = (resolution.width - actualWidth) / 2;
                defaultY = (resolution.height - actualHeight) / 2;
            }

            viewer.setUpViewInWindow(config.getInt("x", defaultX), config.getInt("y", defaultY), actualWidth, actualHeight);
        }

        viewer.setReleaseContextAtEndOfFrameHint(false);
        viewer.realize();
        viewer.init();

        gameStateMgr.request("InitState");

        osg::Timer fps;

        const float maxfps = static_cast<float>(config.getInt("maxfps", -1));

        while (!viewer.done())
        {
            fps.setStartTick();

            viewer.advance();

            viewer.eventTraversal();
            viewer.updateTraversal();
            viewer.renderingTraversals();

            // obey maxfps
            if (maxfps > 0.f)
            {
                const double thisFrameTime = fps.time_s();
                const double minFrameTime = 1.0 / maxfps;

                if (thisFrameTime < minFrameTime)
                {
                    OpenThreads::Thread::microSleep(1000 * 1000 * (minFrameTime - thisFrameTime));
                }
            }
        }

        // TODO: return a non zero if something bad happens blah blah
        return 0;
    }
}
