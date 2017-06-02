
#include "GameStateMgr.hpp"
#include "IGameState.hpp"
#include "IGameStateProvider.hpp"
#include <spdlog/spdlog.h>
#include <osgGA/GUIEventAdapter>

namespace ehb
{
    GameStateMgr::GameStateMgr(IGameStateProvider * provider) : provider(provider)
    {
    }

    GameStateMgr::~GameStateMgr()
    {
        if (currentState.second)
        {
            currentState.second->leave();
        }
    }

    void GameStateMgr::request(const std::string & gameStateType)
    {
        if (IGameState * state = provider->createGameState(gameStateType, *this))
        {
            if (pendingState.second)
            {
                spdlog::get("log")->warn("GameStateMgr still has a pending state {} while requesting a new state ({})", pendingState.first, gameStateType);
            }

            pendingState.first = gameStateType;
            pendingState.second.reset(state);
        }
        else
        {
            spdlog::get("log")->warn("GameState [{}] is unavailable", gameStateType);
        }
    }

    bool GameStateMgr::handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action)
    {
        // assert(currentState != nullptr);

        switch (event.getEventType())
        {
            case osgGA::GUIEventAdapter::FRAME:
            {
                // std::cout << "FRAME" << std::endl;

                if (pendingState.second)
                {
                    auto log = spdlog::get("log");

                    if (currentState.second)
                    {
                        log->info("==========================================================================================================================");
                        log->info("| BEGIN - WORLD STATE TRANSITION: {} --> {}", currentState.first, pendingState.first);
                        log->info("|");

                        currentState.second->leave();
                    }
                    else
                    {
                        log->info("==========================================================================================================================");
                        log->info("| BEGIN - WORLD STATE TRANSITION: {}", pendingState.first);
                        log->info("|");
                    }

                    currentState.first = pendingState.first;
                    currentState.second = std::move(pendingState.second);

                    currentState.second->enter();

                    log->info("|");
                    log->info("| END - WORLD STATE TRANSITION. State = {}", pendingState.first);
                    log->info("==========================================================================================================================");
                }
            }
            break;

            // TODO: when application is shutting down then call currentState->leave(); currentState.reset();

            case osgGA::GUIEventAdapter::USER:
                break;

            default:
                break;
        }

        return currentState.second->handle(event, action);
    }
}
