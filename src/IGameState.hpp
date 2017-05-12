
#pragma once

namespace osgGA
{
    class GUIActionAdapter;
    class GUIEventAdapter;
}

namespace ehb
{
    class IGameStateMgr;
    class IGameState
    {
    public:

        virtual ~IGameState() = default;

        virtual void enter() = 0;
        virtual void leave() = 0;
        virtual bool handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action) = 0;
    };
}
