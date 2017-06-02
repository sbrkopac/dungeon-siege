
#pragma once

#include <osgGA/GUIEventHandler>

namespace ehb
{
    class GameStateMgr;
    class Shell;
    class EventProxy final : public osgGA::GUIEventHandler
    {
    public:

        EventProxy(GameStateMgr & gameStateMgr, Shell & gui);

        virtual ~EventProxy() = default;

        virtual bool handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action, osg::Object *, osg::NodeVisitor *) override;

    private:
        GameStateMgr & gameStateMgr;
        Shell & gui;
    };
}
