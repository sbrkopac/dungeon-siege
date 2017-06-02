
#pragma once

#include <osgGA/GUIEventAdapter>
#include <iostream>
#include <algorithm>

namespace ehb
{
    enum eActionType
    {
        at_none,
        at_some_action,
        at_another_action,
        at_busy_action,

        party_heal_body_with_potions,
        party_heal_magic_with_potions,

        attack,
        cast,
        guard,
        move,
        patrol,
        stop,

        get_group_1,
        get_group_2,
        get_group_3,
        get_group_4,
        get_group_5,
        get_group_6,
        get_group_7,
        get_group_8,
        set_group_1,
        set_group_2,
        set_group_3,
        set_group_4,
        set_group_5,
        set_group_6,
        set_group_7,
        set_group_8,

        set_awp_01,
        set_awp_02,
        set_awp_03,
        set_awp_04,
        set_awp_05,
        set_awp_06,
        set_awp_07,
        set_awp_08,
        set_awp_09,
        set_awp_10,

        get_awp_01,
        get_awp_02,
        get_awp_03,
        get_awp_04,
        get_awp_05,
        get_awp_06,
        get_awp_07,
        get_awp_08,
        get_awp_09,
        get_awp_10,

        move_order_free,
        move_order_limited,
        move_order_never,

        fight_order_always,
        fight_order_back_only,
        fight_order_never,

        target_closest,
        target_weakest,
        target_strongest,

        select_all_party_members, // x2
        select_next_player,
        select_last_player,
        select_lead_character,

        rotate_selected_slots,
        rotate_primary_spell_slot,
        rotate_secondary_spell_slot,

        formation_increase_spacing,
        formation_decrease_spacing,
        cycle_formations
    };

    struct Binding
    {
        int type;
        int button;
        int mask;

        eActionType action;

    public:

        Binding(int type, int button, int mask, eActionType action) : type(type), button(button), mask(mask), action(action)
        {
        }

    public:

        bool operator < (const Binding & other) const
        {
            if (button < other.button) return true;
            if (button > other.button) return false;

            if (mask < other.mask) return true;
            if (mask > other.mask) return false;

            return false;
        }
    };

    class InputBinder
    {
    public:

        InputBinder()
        {
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_H, 0, party_heal_body_with_potions);

            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_M, 0, party_heal_magic_with_potions);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_A, 0, attack);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_C, 0, cast);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_G, 0, guard);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_O, 0, patrol);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_S, 0, stop);

            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F1, 0, get_group_1);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F2, 0, get_group_2);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F3, 0, get_group_3);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F4, 0, get_group_4);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F5, 0, get_group_5);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F6, 0, get_group_6);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F7, 0, get_group_7);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F8, 0, get_group_8);

            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F1, osgGA::GUIEventAdapter::MODKEY_CTRL, set_group_1);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F2, osgGA::GUIEventAdapter::MODKEY_CTRL, set_group_2);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F3, osgGA::GUIEventAdapter::MODKEY_CTRL, set_group_3);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F4, osgGA::GUIEventAdapter::MODKEY_CTRL, set_group_4);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F5, osgGA::GUIEventAdapter::MODKEY_CTRL, set_group_5);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F6, osgGA::GUIEventAdapter::MODKEY_CTRL, set_group_6);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F7, osgGA::GUIEventAdapter::MODKEY_CTRL, set_group_7);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_F8, osgGA::GUIEventAdapter::MODKEY_CTRL, set_group_8);

            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_1, osgGA::GUIEventAdapter::MODKEY_CTRL, set_awp_01);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_2, osgGA::GUIEventAdapter::MODKEY_CTRL, set_awp_02);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_3, osgGA::GUIEventAdapter::MODKEY_CTRL, set_awp_03);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_4, osgGA::GUIEventAdapter::MODKEY_CTRL, set_awp_04);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_5, osgGA::GUIEventAdapter::MODKEY_CTRL, set_awp_05);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_6, osgGA::GUIEventAdapter::MODKEY_CTRL, set_awp_06);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_7, osgGA::GUIEventAdapter::MODKEY_CTRL, set_awp_07);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_8, osgGA::GUIEventAdapter::MODKEY_CTRL, set_awp_08);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_9, osgGA::GUIEventAdapter::MODKEY_CTRL, set_awp_09);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_0, osgGA::GUIEventAdapter::MODKEY_CTRL, set_awp_10);

            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_1, 0, get_awp_01);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_2, 0, get_awp_02);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_3, 0, get_awp_03);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_4, 0, get_awp_04);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_5, 0, get_awp_05);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_6, 0, get_awp_06);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_7, 0, get_awp_07);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_8, 0, get_awp_08);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_9, 0, get_awp_09);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_0, 0, get_awp_10);

            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_Q, osgGA::GUIEventAdapter::MODKEY_ALT, move_order_free);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_W, osgGA::GUIEventAdapter::MODKEY_ALT, move_order_limited);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_E, osgGA::GUIEventAdapter::MODKEY_ALT, move_order_never);

            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_A, osgGA::GUIEventAdapter::MODKEY_ALT, fight_order_always);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_S, osgGA::GUIEventAdapter::MODKEY_ALT, fight_order_back_only);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_D, osgGA::GUIEventAdapter::MODKEY_ALT, fight_order_never);

            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_Z, osgGA::GUIEventAdapter::MODKEY_ALT, target_closest);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_X, osgGA::GUIEventAdapter::MODKEY_ALT, target_weakest);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_C, osgGA::GUIEventAdapter::MODKEY_ALT, target_strongest);

            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_A, osgGA::GUIEventAdapter::MODKEY_CTRL, select_all_party_members);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_E, 0, select_all_party_members);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_Period, 0, select_next_player);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_Comma, 0, select_last_player);
            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_Slash, 0, select_lead_character);

            bindMap.emplace_back(osgGA::GUIEventAdapter::KEYDOWN, osgGA::GUIEventAdapter::KEY_Q, 0, rotate_selected_slots);

            std::sort(bindMap.begin(), bindMap.end(), [](const Binding & lhs, const Binding & rhs) { return rhs < lhs; });
        }

        eActionType action(const osgGA::GUIEventAdapter & event) const
        {
            for (const auto & b : bindMap)
            {
                if (b.type & event.getEventType())
                {
                    int button;
                    int mask;

                    switch (b.type)
                    {
                        case osgGA::GUIEventAdapter::PUSH:
                        case osgGA::GUIEventAdapter::RELEASE:
                            button = event.getButton();
                            mask = event.getButtonMask();
                            break;

                        case osgGA::GUIEventAdapter::KEYDOWN:
                        case osgGA::GUIEventAdapter::KEYUP:
                            button = event.getKey();
                            mask = event.getModKeyMask();
                            break;

                        default: // assert false
                            button = 0;
                            mask = 0;
                            break;
                    }

                    if (b.button == button)
                    {
                        if ((b.mask != 0 && (b.mask & mask) != 0) || b.mask == 0)
                        {
#if 0
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_LEFT_SHIFT) std::cout << "MODKEY_LEFT_SHIFT" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_RIGHT_SHIFT) std::cout << "MODKEY_RIGHT_SHIFT" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL) std::cout << "MODKEY_LEFT_CTRL" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_RIGHT_CTRL) std::cout << "MODKEY_RIGHT_CTRL" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_LEFT_ALT) std::cout << "MODKEY_LEFT_ALT" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_RIGHT_ALT) std::cout << "MODKEY_RIGHT_ALT" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_LEFT_META) std::cout << "MODKEY_LEFT_META" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_RIGHT_META) std::cout << "MODKEY_RIGHT_META" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_LEFT_SUPER) std::cout << "MODKEY_LEFT_SUPER" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_RIGHT_SUPER) std::cout << "MODKEY_RIGHT_SUPER" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_LEFT_HYPER) std::cout << "MODKEY_LEFT_HYPER" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_RIGHT_HYPER) std::cout << "MODKEY_RIGHT_HYPER" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_NUM_LOCK) std::cout << "MODKEY_NUM_LOCK" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CAPS_LOCK) std::cout << "MODKEY_CAPS_LOCK" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL) std::cout << "MODKEY_CTRL" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_SHIFT) std::cout << "MODKEY_SHIFT" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_ALT) std::cout << "MODKEY_ALT" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_META) std::cout << "MODKEY_META" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_SUPER) std::cout << "MODKEY_SUPER" << std::endl;
                            if (event.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_HYPER) std::cout << "MODKEY_HYPER" << std::endl;
#endif

                            return b.action;
                        }
                    }
                }
            }

            return at_none;
        }

    private:

        std::vector<Binding> bindMap;
    };

    static
    bool fromString(const std::string & value, osgGA::GUIEventAdapter::KeySymbol & result)
    {
        if (value == "key_space") result = osgGA::GUIEventAdapter::KEY_Space;

        else if (value == "key_0") result = osgGA::GUIEventAdapter::KEY_0;
        else if (value == "key_1") result = osgGA::GUIEventAdapter::KEY_1;
        else if (value == "key_2") result = osgGA::GUIEventAdapter::KEY_2;
        else if (value == "key_3") result = osgGA::GUIEventAdapter::KEY_3;
        else if (value == "key_4") result = osgGA::GUIEventAdapter::KEY_4;
        else if (value == "key_5") result = osgGA::GUIEventAdapter::KEY_5;
        else if (value == "key_6") result = osgGA::GUIEventAdapter::KEY_6;
        else if (value == "key_7") result = osgGA::GUIEventAdapter::KEY_7;
        else if (value == "key_8") result = osgGA::GUIEventAdapter::KEY_8;
        else if (value == "key_9") result = osgGA::GUIEventAdapter::KEY_9;
        else if (value == "key_a") result = osgGA::GUIEventAdapter::KEY_A;
        else if (value == "key_b") result = osgGA::GUIEventAdapter::KEY_B;
        else if (value == "key_c") result = osgGA::GUIEventAdapter::KEY_C;
        else if (value == "key_d") result = osgGA::GUIEventAdapter::KEY_D;
        else if (value == "key_e") result = osgGA::GUIEventAdapter::KEY_E;
        else if (value == "key_f") result = osgGA::GUIEventAdapter::KEY_F;
        else if (value == "key_g") result = osgGA::GUIEventAdapter::KEY_G;
        else if (value == "key_h") result = osgGA::GUIEventAdapter::KEY_H;
        else if (value == "key_i") result = osgGA::GUIEventAdapter::KEY_I;
        else if (value == "key_j") result = osgGA::GUIEventAdapter::KEY_J;
        else if (value == "key_k") result = osgGA::GUIEventAdapter::KEY_K;
        else if (value == "key_l") result = osgGA::GUIEventAdapter::KEY_L;
        else if (value == "key_m") result = osgGA::GUIEventAdapter::KEY_M;
        else if (value == "key_n") result = osgGA::GUIEventAdapter::KEY_N;
        else if (value == "key_o") result = osgGA::GUIEventAdapter::KEY_O;
        else if (value == "key_p") result = osgGA::GUIEventAdapter::KEY_P;
        else if (value == "key_q") result = osgGA::GUIEventAdapter::KEY_Q;
        else if (value == "key_r") result = osgGA::GUIEventAdapter::KEY_R;
        else if (value == "key_s") result = osgGA::GUIEventAdapter::KEY_S;
        else if (value == "key_t") result = osgGA::GUIEventAdapter::KEY_T;
        else if (value == "key_u") result = osgGA::GUIEventAdapter::KEY_U;
        else if (value == "key_v") result = osgGA::GUIEventAdapter::KEY_V;
        else if (value == "key_w") result = osgGA::GUIEventAdapter::KEY_W;
        else if (value == "key_x") result = osgGA::GUIEventAdapter::KEY_X;
        else if (value == "key_y") result = osgGA::GUIEventAdapter::KEY_Y;
        else if (value == "key_z") result = osgGA::GUIEventAdapter::KEY_Z;

        else if (value == "key_apostrophe") result = osgGA::GUIEventAdapter::KEY_Quotedbl;
        else if (value == "key_comma") result = osgGA::GUIEventAdapter::KEY_Comma;
        else if (value == "key_minus") result = osgGA::GUIEventAdapter::KEY_Minus;
        else if (value == "key_period") result = osgGA::GUIEventAdapter::KEY_Period;
        else if (value == "key_slash") result = osgGA::GUIEventAdapter::KEY_Slash;
        else if (value == "key_semicolon") result = osgGA::GUIEventAdapter::KEY_Semicolon;
        else if (value == "key_equals") result = osgGA::GUIEventAdapter::KEY_Equals;
        else if (value == "key_lbracket") result = osgGA::GUIEventAdapter::KEY_Leftbracket;
        else if (value == "key_backslash") result = osgGA::GUIEventAdapter::KEY_Backslash;
        else if (value == "key_rbracket") result = osgGA::GUIEventAdapter::KEY_Rightbracket;
        else if (value == "key_grave") result = osgGA::GUIEventAdapter::KEY_Backquote;

        else if (value == "key_back") result = osgGA::GUIEventAdapter::KEY_BackSpace;
        else if (value == "key_tab") result = osgGA::GUIEventAdapter::KEY_Tab;
        else if (value == "key_clear") result = osgGA::GUIEventAdapter::KEY_Clear;
        else if (value == "key_return") result = osgGA::GUIEventAdapter::KEY_Return;
        else if (value == "key_pause") result = osgGA::GUIEventAdapter::KEY_Pause;
        else if (value == "key_scrolllock") result = osgGA::GUIEventAdapter::KEY_Scroll_Lock;
        else if (value == "key_escape") result = osgGA::GUIEventAdapter::KEY_Escape;
        else if (value == "key_delete") result = osgGA::GUIEventAdapter::KEY_Delete;

        else if (value == "key_home") result = osgGA::GUIEventAdapter::KEY_Home;
        else if (value == "key_left") result = osgGA::GUIEventAdapter::KEY_Left;
        else if (value == "key_up") result = osgGA::GUIEventAdapter::KEY_Up;
        else if (value == "key_right") result = osgGA::GUIEventAdapter::KEY_Right;
        else if (value == "key_down") result = osgGA::GUIEventAdapter::KEY_Down;
        else if (value == "key_pageup") result = osgGA::GUIEventAdapter::KEY_Page_Up;
        else if (value == "key_pagedown") result = osgGA::GUIEventAdapter::KEY_Page_Down;
        else if (value == "key_end") result = osgGA::GUIEventAdapter::KEY_End;

        else if (value == "key_select") result = osgGA::GUIEventAdapter::KEY_Select;
        else if (value == "key_insert") result = osgGA::GUIEventAdapter::KEY_Insert;
        else if (value == "key_help") result = osgGA::GUIEventAdapter::KEY_Help;
        else if (value == "key_break") result = osgGA::GUIEventAdapter::KEY_Break;
        else if (value == "key_numlock") result = osgGA::GUIEventAdapter::KEY_Num_Lock;

        else if (value == "key_multiply") result = osgGA::GUIEventAdapter::KEY_KP_Multiply;
        else if (value == "key_add") result = osgGA::GUIEventAdapter::KEY_KP_Add;
        else if (value == "key_separator") result = osgGA::GUIEventAdapter::KEY_KP_Separator;
        else if (value == "key_subtract") result = osgGA::GUIEventAdapter::KEY_KP_Subtract;
        else if (value == "key_decimal") result = osgGA::GUIEventAdapter::KEY_KP_Decimal;
        else if (value == "key_divide") result = osgGA::GUIEventAdapter::KEY_KP_Divide;

        else if (value == "key_numpad0") result = osgGA::GUIEventAdapter::KEY_KP_0;
        else if (value == "key_numpad1") result = osgGA::GUIEventAdapter::KEY_KP_1;
        else if (value == "key_numpad2") result = osgGA::GUIEventAdapter::KEY_KP_2;
        else if (value == "key_numpad3") result = osgGA::GUIEventAdapter::KEY_KP_3;
        else if (value == "key_numpad4") result = osgGA::GUIEventAdapter::KEY_KP_4;
        else if (value == "key_numpad5") result = osgGA::GUIEventAdapter::KEY_KP_5;
        else if (value == "key_numpad6") result = osgGA::GUIEventAdapter::KEY_KP_6;
        else if (value == "key_numpad7") result = osgGA::GUIEventAdapter::KEY_KP_7;
        else if (value == "key_numpad8") result = osgGA::GUIEventAdapter::KEY_KP_8;
        else if (value == "key_numpad9") result = osgGA::GUIEventAdapter::KEY_KP_9;

        else if (value == "key_f1") result = osgGA::GUIEventAdapter::KEY_F1;
        else if (value == "key_f2") result = osgGA::GUIEventAdapter::KEY_F2;
        else if (value == "key_f3") result = osgGA::GUIEventAdapter::KEY_F3;
        else if (value == "key_f4") result = osgGA::GUIEventAdapter::KEY_F4;
        else if (value == "key_f5") result = osgGA::GUIEventAdapter::KEY_F5;
        else if (value == "key_f6") result = osgGA::GUIEventAdapter::KEY_F6;
        else if (value == "key_f7") result = osgGA::GUIEventAdapter::KEY_F7;
        else if (value == "key_f8") result = osgGA::GUIEventAdapter::KEY_F8;
        else if (value == "key_f9") result = osgGA::GUIEventAdapter::KEY_F9;
        else if (value == "key_f10") result = osgGA::GUIEventAdapter::KEY_F10;
        else if (value == "key_f11") result = osgGA::GUIEventAdapter::KEY_F11;
        else if (value == "key_f12") result = osgGA::GUIEventAdapter::KEY_F12;
        else if (value == "key_f13") result = osgGA::GUIEventAdapter::KEY_F13;
        else if (value == "key_f14") result = osgGA::GUIEventAdapter::KEY_F14;
        else if (value == "key_f15") result = osgGA::GUIEventAdapter::KEY_F15;
        else if (value == "key_f16") result = osgGA::GUIEventAdapter::KEY_F16;
        else if (value == "key_f17") result = osgGA::GUIEventAdapter::KEY_F17;
        else if (value == "key_f18") result = osgGA::GUIEventAdapter::KEY_F18;
        else if (value == "key_f19") result = osgGA::GUIEventAdapter::KEY_F19;
        else if (value == "key_f20") result = osgGA::GUIEventAdapter::KEY_F20;
        else if (value == "key_f21") result = osgGA::GUIEventAdapter::KEY_F21;
        else if (value == "key_f22") result = osgGA::GUIEventAdapter::KEY_F22;
        else if (value == "key_f23") result = osgGA::GUIEventAdapter::KEY_F23;
        else if (value == "key_f24") result = osgGA::GUIEventAdapter::KEY_F24;

        else if (value == "key_capslock") result = osgGA::GUIEventAdapter::KEY_Caps_Lock;

        // else if (value == "key_none") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_lbutton") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_rbutton") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_mbutton") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_xbutton1") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_xbutton2") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_shift") result = osgGA::GUIEventAdapter::KEY_Shift_L | osgGA::GUIEventAdapter::KEY_Shift_R;
        // else if (value == "key_control") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_alt") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_browser_back") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_browser_forward") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_browser_refresh") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_browser_stop") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_browser_search") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_browser_favorites") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_browser_home") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_volume_mute") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_volume_down") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_volume_up") result = osgGA::GUIEventAdapter::KEY_;
        // ??? else if (value == "key_lwin") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_media_next_track") result = osgGA::GUIEventAdapter::KEY_;
        // ??? else if (value == "key_rwin") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_media_prev_track") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_apps") result = osgGA::GUIEventAdapter::KEY_;
        // ??? else if (value == "key_print") result = osgGA::GUIEventAdapter::KEY_Print;
        // else if (value == "key_media_stop") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_sleep") result = osgGA::GUIEventAdapter::KEY_;
        // ??? else if (value == "key_printscreen") result = osgGA::GUIEventAdapter::KEY_Print;
        // else if (value == "key_media_play_pause") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_launch_mail") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_launch_media_select") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_launch_app1") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_launch_app2") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_kana") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_junja") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_final") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_hanja") result = osgGA::GUIEventAdapter::KEY_;
        // else if (value == "key_kanji") result = osgGA::GUIEventAdapter::KEY_;

        else return false;

        return true;
    }
}
