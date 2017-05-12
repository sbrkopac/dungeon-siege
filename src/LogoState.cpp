
#include <osgGA/GUIEventAdapter>
#include "LogoState.hpp"

namespace ehb
{
    void LogoState::enter()
    {
    }

    void LogoState::leave()
    {
    }

    bool LogoState::handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action)
    {
        /*
         * TODO: play ASP animation of Dungeon Siege text and gameStateMgr->request("MainMenuState");
         */

        return false;
    }
}
