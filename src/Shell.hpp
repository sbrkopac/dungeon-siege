
#pragma once

#include <unordered_map>
#include <osg/Group>
#include "Widget.hpp"

namespace osgGA
{
    class GUIActionAdapter;
    class GUIEventAdapter;
}

namespace ehb
{
    enum UI_CONTROL_TYPE
    {
        ui_type_window = 0,
        ui_type_button = 1,
        ui_type_checkbox = 2,
        ui_type_slider = 3,
        ui_type_listbox = 4,
        ui_type_radio_button = 5,
        ui_type_button_multistage = 6,
        ui_type_text = 7,
        ui_type_cursor = 8,
        ui_type_dockbar = 9,
        ui_type_gridbox = 10,
        ui_type_popupmenu = 11,
        ui_type_item = 12,
        ui_type_itemslot = 13,
        ui_type_infoslot = 14,
        ui_type_status_bar = 15,
        ui_type_edit_box = 16,
        ui_type_combo_box = 17,
        ui_type_listener = 18,
        ui_type_listreport = 19,
        ui_type_chat_box = 20,
        ui_type_text_box = 21,
        ui_type_dialog_box = 22,
        ui_type_tab = 23
    };

    bool fromString(const std::string & value, UI_CONTROL_TYPE & result);

    std::string toString(UI_CONTROL_TYPE value);
}

namespace ehb
{
    class FileSys;
    class Shell : public osg::Group
    {
    public:

        Shell(FileSys & fileSys);

        void activateInterface(const std::string & path, bool show = true);
        void deactivateInterface(const std::string & name);

        void showInterface(const std::string & name);
        void hideInterface(const std::string & name);

        bool doWidgetsOverlap(const Widget * widget1, const Widget * widget2);
        bool doWidgetsOverlap(const std::string & widget1, const std::string & interface1, const std::string & widget2, const std::string & interface2);

        Widget * findWidget(const std::string & name, const std::string & interface);

        void shiftGroup(const std::string & interface, const std::string & group, int deltaX, int deltaY);
        void shiftInterface(const std::string & name, int deltaX, int deltaY);

        Widget * createDefaultWidgetOfType(const std::string & type);

        bool handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action);

    private:

        FileSys & fileSys;

        unsigned int screenWidth = 0, screenHeight = 0;

    public:
        std::unordered_map<std::string, std::string> ctrlArt;
    };

    inline bool Shell::doWidgetsOverlap(const std::string & widget1, const std::string & interface1, const std::string & widget2, const std::string & interface2)
    {
        return doWidgetsOverlap(findWidget(widget1, interface1), findWidget(widget2, interface2));
    }
}
