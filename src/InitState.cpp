
#include <osgGA/GUIEventAdapter>
#include "InitState.hpp"

namespace ehb
{
    void InitState::enter()
    {
    }

    void InitState::leave()
    {
    }

    bool InitState::handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action)
    {
        /*
         * TODO: load tanks and then gameStateMgr->request("InitState");
         */

        if (event.getEventType() == osgGA::GUIEventAdapter::PUSH)
        {
            return true;
        }

        return false;
    }
}
