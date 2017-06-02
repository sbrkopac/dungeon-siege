
#include "EventProxy.hpp"
#include "GameStateMgr.hpp"
#include "Shell.hpp"

namespace ehb
{
    EventProxy::EventProxy(GameStateMgr & gameStateMgr, Shell & gui) : gameStateMgr(gameStateMgr), gui(gui)
    {
    }

    bool EventProxy::handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action, osg::Object *, osg::NodeVisitor *)
    {
        return gui.handle(event, action) || gameStateMgr.handle(event, action);
    }
}
