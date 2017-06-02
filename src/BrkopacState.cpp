
#include <spdlog/spdlog.h>
#include <osgGA/GUIEventAdapter>
#include <osgViewer/Viewer>
#include "BrkopacState.hpp"
#include "IGameStateMgr.hpp"
#include "IConfig.hpp"

#include <osg/Camera>
#include <osg/Image>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

namespace ehb
{
    void BrkopacState::enter()
    {
        auto log = spdlog::get("log");

        log->info("this is the BrkopacState");

        auto farmgirl = osgDB::readNodeFile("m_c_gah_fg_pos_a1.asp");
        farmgirl->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

        auto scene = new osg::Group;
        scene->addChild(farmgirl);

        viewer.setSceneData(scene);
    }

    void BrkopacState::leave()
    {
    }

    bool BrkopacState::handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action)
    {
        return false;
    }
}
