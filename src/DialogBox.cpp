
#include <spdlog/spdlog.h>
#include <osg/Geode>
#include <osg/Geometry>
#include "DialogBox.hpp"

namespace ehb
{
    void DialogBox::createCommonCtrl(const std::string & commonTemplate)
    {
        auto log = spdlog::get("log");

        if (isCommonControl())
        {
            log->error("{} is already a common control, can't createCommonCtrl again", getName());
            return;
        }

        /*
         * cpbox_bottom_left_corner	= b_gui_cmn_cpbox_ll;
         * cpbox_bottom_right_corner	= b_gui_cmn_cpbox_lr;
         * cpbox_bottom_side			= b_gui_cmn_cpbox_bot;
         * cpbox_left_side				= b_gui_cmn_cpbox_l;
         * cpbox_right_side			= b_gui_cmn_cpbox_r;
         * cpbox_top_left_corner		= b_gui_cmn_cpbox_ul;
         * cpbox_top_right_corner		= b_gui_cmn_cpbox_ur;
         * cpbox_top_side				= b_gui_cmn_cpbox_top;
         * cpbox_fill					= b_gui_cmn_box_alpha_154;
         */

        const auto pos = commonTemplate.find_last_of('_');

        log->info("creating dialog box common control {}", commonTemplate);
        log->info(" - {}_bottom_left_corner", commonTemplate);
        log->info(" - {}_bottom_right_corner", commonTemplate);
        log->info(" - {}_bottom_side", commonTemplate);
        log->info(" - {}_left_side", commonTemplate);
        log->info(" - {}_right_side", commonTemplate);
        log->info(" - {}_top_left_corner", commonTemplate);
        log->info(" - {}_top_right_corner", commonTemplate);
        log->info(" - {}_top_side", commonTemplate);
        log->info(" - {}_fill", commonTemplate);
        if (pos != std::string::npos) log->info(" - {}_fill", commonTemplate.substr(0, pos));

        /*
        addDrawable(new TopLeftCorner(*this));
        addDrawable(new TopRightCorner(*this));
        addDrawable(new BottomLeftCorner(*this));
        addDrawable(new BottomRightCorner(*this));
        addDrawable(new LeftSide(*this));
        addDrawable(new TopSide(*this));
        addDrawable(new RightSide(*this));
        addDrawable(new BottomSide(*this));
        addDrawable(new Middle(*this));
        */

        // Widget::createCommonCtrl(commonTemplate);
    }
}
