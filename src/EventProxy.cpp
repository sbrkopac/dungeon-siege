
#include "EventProxy.hpp"
#include "GameStateMgr.hpp"

namespace ehb
{
    EventProxy::EventProxy(GameStateMgr * gameStateMgr) : gameStateMgr(gameStateMgr)
    {
    }

    bool EventProxy::handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action, osg::Object *, osg::NodeVisitor *)
    {
        return gameStateMgr->handle(event, action);
    }
}
