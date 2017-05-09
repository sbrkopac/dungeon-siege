
#include <osg/GraphicsContext>
#include <osgViewer/config/SingleScreen>
#include <osgViewer/config/SingleWindow>
#include <osgViewer/Viewer>
#include "config.hpp"
#include "masterfileindex.hpp"

int main(int argc, char * argv[])
{
    // read command line args, win32 registry, ini, and hard coded defaults to init the Config class
    ehb::Config config(&argc, argv);

    config.dump();

    // create a window using data from the Config class
    osgViewer::Viewer viewer;

#ifdef _DEBUG
    if (config.getBool("fullscreen", false))
#else
    if (config.getBool("fullscreen", true))
#endif
    {
        viewer.apply(new osgViewer::SingleScreen());
    }
    else
    {
        int defaultX = 0, defaultY = 0;

        int actualWidth = config.getInt("width", 640);
        int actualHeight = config.getInt("height", 480);

        // calculate the coordinates which would put the window at the center of the screen
        if (osg::GraphicsContext::WindowingSystemInterface * wsi = osg::GraphicsContext::getWindowingSystemInterface())
        {
            osg::GraphicsContext::ScreenSettings resolution;

            // TODO: consider not hard coding to a specific monitor
            wsi->getScreenSettings(0, resolution);

            defaultX = (resolution.width - actualWidth) / 2;
            defaultY = (resolution.height - actualHeight) / 2;
        }

        viewer.apply(new osgViewer::SingleWindow(config.getInt("x", defaultX), config.getInt("y", defaultY), actualWidth, actualHeight));
    }

    // so much work just to set the window title...
    osgViewer::Viewer::Windows eachWindow;
    {
        viewer.getWindows(eachWindow);

        for (osgViewer::GraphicsWindow * window : eachWindow)
        {
            window->setWindowName("Open Siege");
        }
    }

    /*

     * This relies on the config class being init properly
     * Maybe it is worth passing the config class to the ctor for clarity?
     * TODO: xShould this be inside a state?
     */
    ehb::MasterFileIndex masterFileIndex;

    {
        // validate we can find the startup_verify.gpg file and if we can't we should die

        siege::ByteArray data;

        if (!masterFileIndex.readFile("/config/startup_verify.gpg", data))
        {
            throw std::runtime_error("Startup_verify.gpg is not present. Goodbye.");
        }

        // TODO: Check the startup_verify entries and validate the proper folders are in-place
    }

    // fire up the first world state

    osg::Timer fps;

    viewer.setReleaseContextAtEndOfFrameHint(false);
    viewer.realize();
    viewer.init();

    const float maxfps = static_cast<float>(config.getInt("maxfps", -1));

    while (!viewer.done())
    {
        fps.setStartTick();

        viewer.advance();

        viewer.eventTraversal();
        viewer.updateTraversal();
        viewer.renderingTraversals();

        if (maxfps > 0.f)
        {
            const double thisFrameTime = fps.time_s();
            const double minFrameTime = 1.0 / maxfps;

            if (thisFrameTime < minFrameTime)
            {
                std::cout << "sleeping for " << (minFrameTime - thisFrameTime) << std::endl;
                OpenThreads::Thread::microSleep(1000 * 1000 * (minFrameTime - thisFrameTime));
            }
        }
    }

    // write out ini file using values from ... wherever they are

    return 0;
}
