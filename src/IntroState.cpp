
#include <spdlog/spdlog.h>
#include <osgGA/GUIEventAdapter>
#include "IntroState.hpp"
#include "Shell.hpp"

namespace ehb
{
    void IntroState::enter()
    {
        auto log = spdlog::get("log");

        log->info("IntroState::enter");

        // TODO: cache videos in config.getString("cache-dir")

        gui.activateInterface("ui:interfaces:backend:data_bar");
        // gui.activateInterface("ui:interfaces:backend:field_commands");
        // gui.activateInterface("ui:interfaces:backend:character_awp");
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
