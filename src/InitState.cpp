
#include <spdlog/spdlog.h>
#include <osgGA/GUIEventAdapter>
#include "IGameStateMgr.hpp"
#include "InitState.hpp"
#include "SimpleConfig.hpp"

#include "InputBinder.hpp"

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

        auto log = spdlog::get("log");

        log->info("ds-install-path: {}", config.getString("ds-install-path"));
        log->info("cache-dir: {}", config.getString("cache-dir"));
        log->info("config-dir: {}", config.getString("config-dir"));
        log->info("data-dir: {}", config.getString("data-dir"));
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

        InputBinder binder;

        switch (binder.action(event))
        {
            case party_heal_body_with_potions: std::cout << "Drink Health Potion" << std::endl; break;
            case party_heal_magic_with_potions: std::cout << "Drink Mana Potion" << std::endl; break;

            case attack: std::cout << "Force Attack" << std::endl; break;
            case cast: std::cout << "Force Cast Spell" << std::endl; break;
            case guard: std::cout << "Guard Character" << std::endl; break;
            case move: std::cout << "Move" << std::endl; break;
            case patrol: std::cout << "Patrol" << std::endl; break;
            case stop: std::cout << "Stop" << std::endl; break;

            case get_group_1: std::cout << "Recall Party Group 1" << std::endl; break;
            case get_group_2: std::cout << "Recall Party Group 2" << std::endl; break;
            case get_group_3: std::cout << "Recall Party Group 3" << std::endl; break;
            case get_group_4: std::cout << "Recall Party Group 4" << std::endl; break;
            case get_group_5: std::cout << "Recall Party Group 5" << std::endl; break;
            case get_group_6: std::cout << "Recall Party Group 6" << std::endl; break;
            case get_group_7: std::cout << "Recall Party Group 7" << std::endl; break;
            case get_group_8: std::cout << "Recall Party Group 8" << std::endl; break;
            case set_group_1: std::cout << "Save Party Group 1" << std::endl; break;
            case set_group_2: std::cout << "Save Party Group 2" << std::endl; break;
            case set_group_3: std::cout << "Save Party Group 3" << std::endl; break;
            case set_group_4: std::cout << "Save Party Group 4" << std::endl; break;
            case set_group_5: std::cout << "Save Party Group 5" << std::endl; break;
            case set_group_6: std::cout << "Save Party Group 6" << std::endl; break;
            case set_group_7: std::cout << "Save Party Group 7" << std::endl; break;
            case set_group_8: std::cout << "Save Party Group 8" << std::endl; break;

            case set_awp_01: std::cout << "Save Weapon Config. 1" << std::endl; break;
            case set_awp_02: std::cout << "Save Weapon Config. 2" << std::endl; break;
            case set_awp_03: std::cout << "Save Weapon Config. 3" << std::endl; break;
            case set_awp_04: std::cout << "Save Weapon Config. 4" << std::endl; break;
            case set_awp_05: std::cout << "Save Weapon Config. 5" << std::endl; break;
            case set_awp_06: std::cout << "Save Weapon Config. 6" << std::endl; break;
            case set_awp_07: std::cout << "Save Weapon Config. 7" << std::endl; break;
            case set_awp_08: std::cout << "Save Weapon Config. 8" << std::endl; break;
            case set_awp_09: std::cout << "Save Weapon Config. 9" << std::endl; break;
            case set_awp_10: std::cout << "Save Weapon Config. 0" << std::endl; break;

            case get_awp_01: std::cout << "Recall Weapon Config. 1" << std::endl; break;
            case get_awp_02: std::cout << "Recall Weapon Config. 2" << std::endl; break;
            case get_awp_03: std::cout << "Recall Weapon Config. 3" << std::endl; break;
            case get_awp_04: std::cout << "Recall Weapon Config. 4" << std::endl; break;
            case get_awp_05: std::cout << "Recall Weapon Config. 5" << std::endl; break;
            case get_awp_06: std::cout << "Recall Weapon Config. 6" << std::endl; break;
            case get_awp_07: std::cout << "Recall Weapon Config. 7" << std::endl; break;
            case get_awp_08: std::cout << "Recall Weapon Config. 8" << std::endl; break;
            case get_awp_09: std::cout << "Recall Weapon Config. 9" << std::endl; break;
            case get_awp_10: std::cout << "Recall Weapon Config. 0" << std::endl; break;

            case move_order_free: std::cout << "Movement: Move Freely" << std::endl; break;
            case move_order_limited: std::cout << "Movement: Move Limited" << std::endl; break;
            case move_order_never: std::cout << "Movement: Move Never" << std::endl; break;

            case fight_order_always: std::cout << "Attack: Fight Freely" << std::endl; break;
            case fight_order_back_only: std::cout << "Attack: Defend" << std::endl; break;
            case fight_order_never: std::cout << "Attack: Hold Fire" << std::endl; break;

            case target_closest: std::cout << "Targeting: Target Closest" << std::endl; break;
            case target_weakest: std::cout << "Targeting: Target Weakest" << std::endl; break;
            case target_strongest: std::cout << "Targeting: Target Strongest" << std::endl; break;

            case select_all_party_members: std::cout << "Select All Party Members" << std::endl; break;
            case select_next_player: std::cout << "Select Next Party Member" << std::endl; break;
            case select_last_player: std::cout << "Select Last Party Member" << std::endl; break;
            case select_lead_character: std::cout << "Select Lead Party Member" << std::endl; break;

            default:
                break;
        }

        if (event.getEventType() == osgGA::GUIEventAdapter::FRAME)
        {
            gameStateMgr.request("BrkopacState");

            /*
            if (config->getBool("intro"))
            {
                gameStateMgr->request("IntroState");
            }
            else
            {
                gameStateMgr->request("LogoState");
            }
            */
        }

        return false;
    }
}
