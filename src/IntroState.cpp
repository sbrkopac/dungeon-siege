
#include <osgGA/GUIEventAdapter>
#include "IntroState.hpp"

namespace ehb
{
    void IntroState::enter()
    {
    }

    void IntroState::leave()
    {
    }

    bool IntroState::handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action)
    {
        /*
         * TODO: show some movies and then gameStateMgr->request("LogoState");
         */

        return false;
    }
}
