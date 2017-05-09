
#include <iostream>
#include <sstream>
#include <string>
#include "../gas.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace siege;

TEST_CASE("GasTest: Doing some gas testing...")
{
    std::stringstream stream;

    { // setup a mock gas file

        stream << "[t:template,n:generic_test]" << std::endl;
        stream << "{" << std::endl;
        stream << "    specializes = trigger;" << std::endl;
        stream << "    doc = \"generic_test\";" << std::endl;
        stream << "    " << std::endl;
        stream << "    extra_doc = \"Generic trigger which does nothing except trigger stuff.\";" << std::endl;
        stream << "    " << std::endl;
        stream << "    category_name = \"trigger\";" << std::endl;
        stream << "    " << std::endl;
        stream << "    [gizmo] { diffuse_color=1.000000,1.000000,1.000000; texture= ; }" << std::endl;
        stream << "    " << std::endl;
        stream << "    aspect:model = m_a_shd_10;" << std::endl;
        stream << "    aspect:textures:0 = b_a_shd_014;" << std::endl;
        stream << "    " << std::endl;
        stream << "    common:        screen_name = \"dev_shield, rusted buckler\";" << std::endl;
        stream << "    common:is_pcontent_allowed = false;" << std::endl;
        stream << "    " << std::endl;
        stream << "    gui:        inventory_icon = b_gui_ig_i_a_shd_014;" << std::endl;
        stream << "    gui:        inventory_width = 2;" << std::endl;
        stream << "    gui:        inventory_height = 2;" << std::endl;
        stream << "    " << std::endl;
        stream << "    defend:        defense = 5;" << std::endl;
        stream << "}" << std::endl;
    }

    Gas doc;

    // check for valid parsing
    REQUIRE(doc.load(stream) != false);

    SECTION("GasTest: Attribute testing using pointer to the child")
    {
        // get the only child in the file
        const Node *generic_test = doc.child("generic_test");
        REQUIRE(generic_test != nullptr);

        // do checks for the basic attributes for the first child
        CHECK(generic_test->type() == "template");
        CHECK(generic_test->name() == "generic_test");
        CHECK(generic_test->valueOf("specializes") == "trigger");
        CHECK(generic_test->valueOf("doc") == "\"generic_test\"");
        CHECK(generic_test->valueOf("extra_doc") == "\"Generic trigger which does nothing except trigger stuff.\"");
        CHECK(generic_test->valueOf("category_name") == "\"trigger\"");
        
        // get our next child (gizmo)
        const Node *gizmo = generic_test->child(generic_test->child("gizmo")->name());
        REQUIRE(gizmo != nullptr);

        CHECK(gizmo->valueOf("diffuse_color") == "1.000000,1.000000,1.000000");
        CHECK(gizmo->valueOf("texture") == "");

        // get our next child (aspect)
        const Node *aspect = generic_test->child(generic_test->child("aspect")->name());
        REQUIRE(aspect != nullptr);

        CHECK(aspect->valueOf("model") == "m_a_shd_10");
        CHECK(aspect->valueOf("textures:0") == "b_a_shd_014");
    }

    SECTION("GasTest: Attribute testing using valueOf without pointer to the child")
    {
        // generic_test section
        CHECK(doc.valueOf("generic_test:specializes") == "trigger");
        CHECK(doc.valueOf("generic_test:doc") == "\"generic_test\"");
        CHECK(doc.valueOf("generic_test:extra_doc") == "\"Generic trigger which does nothing except trigger stuff.\"");
        CHECK(doc.valueOf("generic_test:category_name") == "\"trigger\"");

        // generic_test:gizmo section
        CHECK(doc.valueOf("generic_test:gizmo:diffuse_color") == "1.000000,1.000000,1.000000");
        CHECK(doc.valueOf("generic_test:gizmo:texture") == "");
    }

}