
#include "GameStateMgr.hpp"
#include "IGameState.hpp"
#include "IGameStateProvider.hpp"
#include <osgGA/GUIEventAdapter>

#include "Logging.hpp"

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
            if (pendingState != nullptr)
            {
                WARN_LOG("The GameStateMgr still has a pending state while transitioning to a new state");
            }

            pendingState.reset(state);
        }
        else
        {
            WARN_LOG("GameState [{}] is unavailable", gameStateType);
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
                {
                    if (currentState)
                    {
                        std::stringstream ss;

                        ss << "==========================================================================================================================" << std::endl;
                        ss << "| BEGIN - WORLD STATE TRANSITION: " << "FROM" << " --> " << "TO" << std::endl;
                        ss << "|" << std::endl;

                        INFO_LOG(ss.str());

                        currentState->leave();
                    }
                    else
                    {
                        std::stringstream ss;

                        ss << "==========================================================================================================================" << std::endl;
                        ss << "| BEGIN - WORLD STATE TRANSITION: " << "TO" << std::endl;
                        ss << "|" << std::endl;

                        INFO_LOG(ss.str());
                    }

                    currentState = std::move(pendingState);

                    currentState->enter();

                    std::stringstream ss;

                    ss << "|" << std::endl;
                    ss << "| END - WORLD STATE TRANSITION. State = " << "TO""" << std::endl;
                    ss << "==========================================================================================================================" << std::endl;

                    INFO_LOG(ss.str());
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
