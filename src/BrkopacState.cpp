
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
    #if 0
    namespace gui
    {
        class Drawable : public osg::Drawable
        {
            public:

                Drawable()
                {
                    setSupportsDisplayList(false);
                }

                class FrameUpdate : public osg::Drawable::UpdateCallback
                {

                };

                class CollectDrawCalls : public osg::Drawable::CullCallback
                {

                };
        };

        class Gui : public osg::Camera
        {
            public:

                Gui()
                {
                    drawable = new Drawable;

                    setReferenceFrame(osg::Transform::ABSOLUTE_RF);
                    setProjectionResizePolicy(osg::Camera::FIXED);
                    setProjectionMatrix(osg::Matrix::ortho2D(0, 640, 0, 480));
                    setViewMatrix(osg::Matrix::identity());
                    setRenderOrder(osg::Camera::POST_RENDER);
                    setClearMask(GL_NONE);

                    addChild(drawable);
                }

            private:

                osg::ref_ptr<Drawable> drawable;
        };
    }
    #endif

    void BrkopacState::enter()
    {
        #if 0
        auto log = spdlog::get("log");

        log->info("this is the BrkopacState");

        osgViewer::Viewer::Windows windows;
        viewer.getWindows(windows);

        osgViewer::GraphicsWindow * window = windows[0];

        gui::Gui *gui = new gui::Gui;

        gui->setGraphicsContext(windows[0]);
        gui->setViewport(0, 0, window->getTraits()->width, window->getTraits()->height);

        viewer.addSlave(gui, false);
        #endif
    }

    void BrkopacState::leave()
    {
    }

    bool BrkopacState::handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action)
    {
        return false;
    }
}
