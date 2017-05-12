
#include "GameStateMgr.hpp"
#include "IGameState.hpp"
#include "IGameStateProvider.hpp"
#include <osgGA/GUIEventAdapter>

// TODO: some spdlog up in here
#include <iostream>

namespace ehb
{
    GameStateMgr::GameStateMgr(IGameStateProvider * provider) : provider(provider)
    {
    }

    GameStateMgr::~GameStateMgr()
    {
        if (currentState)
        {
            currentState->leave();
        }
    }

    void GameStateMgr::request(const std::string & gameStateType)
    {
        if (IGameState * state = provider->createGameState(gameStateType, this))
        {
            // TODO: log a warning if pendingState isn't empty

            pendingState.reset(state);
        }
        else
        {
            // TODO: log a warning that the requested state change can't happen
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

                if (pendingState)
                { std::cout << "acting on pendingState" << std::endl;
                    if (currentState)
                    {
                        std::cout << "==========================================================================================================================" << std::endl;
                        std::cout << "| BEGIN - WORLD STATE TRANSITION: " << "FROM" << " --> " << "TO" << std::endl;
                        std::cout << "|" << std::endl;

                        currentState->leave();
                    }
                    else
                    {
                        std::cout << "==========================================================================================================================" << std::endl;
                        std::cout << "| BEGIN - WORLD STATE TRANSITION: " << "TO" << std::endl;
                        std::cout << "|" << std::endl;
                    }

                    currentState = std::move(pendingState);

                    currentState->enter();

                    std::cout << "|" << std::endl;
                    std::cout << "| END - WORLD STATE TRANSITION. State = " << "TO""" << std::endl;
                    std::cout << "==========================================================================================================================" << std::endl;
                }
            }
            break;

            // TODO: when application is shutting down then call currentState->leave(); currentState.reset();

            case osgGA::GUIEventAdapter::USER:
                break;

            default:
                break;
        }

        return currentState->handle(event, action);
    }
}
