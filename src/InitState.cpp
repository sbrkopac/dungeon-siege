
#include <osgGA/GUIEventAdapter>
#include "InitState.hpp"
#include "SimpleConfig.hpp"
#include "Logging.hpp"

#include <iostream>

namespace ehb
{
    void InitState::enter()
    {
        /*
         * generate a list of bits & tank files to load:
         *  - config.getString("data-dir") + "Bits"
         *  - config.getString("data-dir") + "Mods"
         *  - config.getString("ds-install-path") + "Resources"
         *  - config.getString("ds-install-path") + "Maps"
         *  - dsloa ? config.getString("ds-install-path") + "DSLOA"
         */

        Logging::init(config->getString("data-dir", "."));
        
        INFO_LOG("DS Install Path: {}", config->getString("ds-install-path"));
        INFO_LOG("Data Directory: {}", config->getString("data-dir"));
        INFO_LOG("Cache Directory: {}", config->getString("cache-dir"));
        INFO_LOG("Config Directory: {}", config->getString("config-dir"));
    }

    void InitState::leave()
    {
    }

    bool InitState::handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action)
    {
        /*
         * TODO:
         * - load each tank file
         * - process nnk files
         * - process /world/global/siege_nodes/
         * - process various .gas files which contain engine settings (/config/*.gas, etc...)
         * - draw dots
         * - gameStateMgr->request("IntroState");
         */

        return false;
    }
}
