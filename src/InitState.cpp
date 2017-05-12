
#include <osgGA/GUIEventAdapter>
#include "InitState.hpp"
#include "SimpleConfig.hpp"
#include "Logging.hpp"

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
