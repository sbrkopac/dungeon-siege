
#include <spdlog/spdlog.h>
#include <osgGA/GUIEventAdapter>
#include "FileSys.hpp"
#include "Shell.hpp"

#include <cstdlib>
#include <ctime>

namespace ehb
{
    bool fromString(const std::string & value, UI_CONTROL_TYPE & result)
    {
        if (value == "ui_type_window") result = ui_type_window;
        else if (value == "ui_type_button") result = ui_type_button;
        else if (value == "ui_type_checkbox") result = ui_type_checkbox;
        else if (value == "ui_type_slider") result = ui_type_slider;
        else if (value == "ui_type_listbox") result = ui_type_listbox;
        else if (value == "ui_type_radio_button") result = ui_type_radio_button;
        else if (value == "ui_type_button_multistage") result = ui_type_button_multistage;
        else if (value == "ui_type_text") result = ui_type_text;
        else if (value == "ui_type_cursor") result = ui_type_cursor;
        else if (value == "ui_type_dockbar") result = ui_type_dockbar;
        else if (value == "ui_type_gridbox") result = ui_type_gridbox;
        else if (value == "ui_type_popupmenu") result = ui_type_popupmenu;
        else if (value == "ui_type_item") result = ui_type_item;
        else if (value == "ui_type_itemslot") result = ui_type_itemslot;
        else if (value == "ui_type_infoslot") result = ui_type_infoslot;
        else if (value == "ui_type_status_bar") result = ui_type_status_bar;
        else if (value == "ui_type_edit_box") result = ui_type_edit_box;
        else if (value == "ui_type_combo_box") result = ui_type_combo_box;
        else if (value == "ui_type_listener") result = ui_type_listener;
        else if (value == "ui_type_listreport") result = ui_type_listreport;
        else if (value == "ui_type_chat_box") result = ui_type_chat_box;
        else if (value == "ui_type_text_box") result = ui_type_text_box;
        else if (value == "ui_type_dialog_box") result = ui_type_dialog_box;
        else if (value == "ui_type_tab") result = ui_type_tab;
        else return false;

        return true;
    }

    std::string toString(UI_CONTROL_TYPE value)
    {
        switch (value)
        {
            case ui_type_window: return "ui_type_window";
            case ui_type_button: return "ui_type_button";
            case ui_type_checkbox: return "ui_type_checkbox";
            case ui_type_slider: return "ui_type_slider";
            case ui_type_listbox: return "ui_type_listbox";
            case ui_type_radio_button: return "ui_type_radio_button";
            case ui_type_button_multistage: return "ui_type_button_multistage";
            case ui_type_text: return "ui_type_text";
            case ui_type_cursor: return "ui_type_cursor";
            case ui_type_dockbar: return "ui_type_dockbar";
            case ui_type_gridbox: return "ui_type_gridbox";
            case ui_type_popupmenu: return "ui_type_popupmenu";
            case ui_type_item: return "ui_type_item";
            case ui_type_itemslot: return "ui_type_itemslot";
            case ui_type_infoslot: return "ui_type_infoslot";
            case ui_type_status_bar: return "ui_type_status_bar";
            case ui_type_edit_box: return "ui_type_edit_box";
            case ui_type_combo_box: return "ui_type_combo_box";
            case ui_type_listener: return "ui_type_listener";
            case ui_type_listreport: return "ui_type_listreport";
            case ui_type_chat_box: return "ui_type_chat_box";
            case ui_type_text_box: return "ui_type_text_box";
            case ui_type_dialog_box: return "ui_type_dialog_box";
            case ui_type_tab: return "ui_type_tab";
        }

        return "";
    }
}

namespace ehb
{

    Shell::Shell(FileSys & fileSys, osgViewer::Viewer & viewer) : fileSys(fileSys), camera(new osg::Camera), group(new osg::Group)
    {
        // turn lighting off for the text and disable depth test to ensure it's always ontop.
        group->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

        srand(static_cast<unsigned>(time(0)));

        screenWidth = 640;
        screenHeight = 480;

        Gas doc;

        if (fileSys.readFile("/ui/interfaces/common/common_control_art.gas", doc))
        {
            if (const auto node = doc.child("common_control_art"))
            {
                for (const auto attr : node->eachAttribute())
                {
                    ctrlArt[attr.name] = attr.value;
                }
            }
        }

        for (const auto & entry : ctrlArt)
        {
            // spdlog::get("log")->info("common_control_art: {} = {}", entry.first, entry.second);
        }
    }

    void Shell::blah(osgViewer::Viewer & viewer)
    {
        // create a camera to set up the projection and model view matrices, and the subgraph to draw in the HUD

        // set the projection matrix
        camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 640, 0, 480));

        // set the view matrix
        camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        camera->setViewMatrix(osg::Matrix::identity());

        // only clear the depth buffer
        camera->setClearMask(GL_DEPTH_BUFFER_BIT);

        // draw subgraph after main camera view.
        camera->setRenderOrder(osg::Camera::POST_RENDER);

        // we don't want the camera to grab event focus from the viewers main camera(s).
        camera->setAllowEventFocus(false);

        camera->addChild(group);



        viewer.setSceneData(camera);
    }

    void Shell::activateInterface(const std::string & path, bool show)
    {
        auto log = spdlog::get("log");

        const auto lastColon = path.find_last_of(':');

        if (lastColon == std::string::npos) return;

        const std::string name = path.substr(lastColon + 1); // TODO: fix this so it doesn't crash (if : is the last character)

        // transform path into an actual file path
        std::string actualPath = path;
        std::replace(actualPath.begin(), actualPath.end(), ':', '/');
        actualPath += '/';
        actualPath += name;
        actualPath += ".gas";
        actualPath.insert(0, 1, '/');

        log->info("converting {} into {} and using {} as name", path, actualPath, name);

        Gas doc;

        if (fileSys.readFile(actualPath, doc))
        {
            log->info("successfully read {}", actualPath);

            // b interface = true; ?

            // UIShell.ActivateInterface( "ui:interfaces:backend:quick_bar", true);

            const int intendedResolutionWidth = std::stoi(doc.valueOf(name + ":intended_resolution_width", "-1"));
            const int intendedResolutionHeight = std::stoi(doc.valueOf(name + ":intended_resolution_height", "-1"));

            for (const auto node : doc.eachChildOf(name))
            {
                // if (node->name() != "data_bar") continue;
                if (Widget * widget = createDefaultWidgetOfType(node->type()))
                {
                    log->info("created {} widget with id {}", node->type(), node->name());
                    // widget->loadFrom(node);

                    /*
                     * TODO: process the following attributes:
                     *
                     * - dock_group
                     * - fixed_location
                     * - rollover_help
                     * - wrap_mode
                     */

                    Rect rect;

                    if (fromString(node->valueOf("rect"), rect))
                    {
                        log->info("{} becomes {}, {}, {}, {}", node->valueOf("rect"), rect.x1, rect.y1, rect.x2, rect.y2);
                        widget->setRect(rect.x1, rect.x2, rect.y1, rect.y2);
                    }

                    NormalizedRect uvcoords;

                    if (fromString(node->valueOf("uvcoords"), uvcoords))
                    {
                        widget->setUVRect(uvcoords.left, uvcoords.right, uvcoords.top, uvcoords.bottom);
                    }

                    widget->setDrawOrder(std::stoi(node->valueOf("draw_order", "0")));

                    widget->loadTexture(node->valueOf("texture") + ".raw", false);

#if 0
                    widget->alpha = std::stof(node->valueOf("alpha", "1"));
                    widget->drawOrder = std::stoi(node->valueOf("draw_order", "0")); // TODO: parse as a hex if the type is x...
                    widget->name = node->name();
                    widget->group = node->valueOf("group");
                    widget->interfaceParent = name;
                    fromString(node->valueOf("rect"), widget->originalRect);
                    fromString(node->valueOf("uvcoords"), widget->uvrect);
                    widget->backgroundColor = std::stoi(node->valueOf("background_color", "0")); // TODO: parse as a hex if the type is x...
                    widget->backgroundFill = node->valueOf("background_fill", "false") == "true";
                    widget->border = node->valueOf("border", "false") == "true";
                    widget->borderColor = std::stoi(node->valueOf("border_color", "0")); // TODO: parse as a hex if the type is x...
                    widget->visible = node->valueOf("pass_through", "false") == "true";
                    widget->visible = node->valueOf("visible", "true") == "true";
                    widget->centerX = node->valueOf("center_x", "false") == "true";
                    widget->centerY = node->valueOf("center_y", "false") == "true";
                    widget->stretchX = node->valueOf("stretch_x", "false") == "true";
                    widget->stretchY = node->valueOf("stretch_y", "false") == "true";
                    widget->isLeftAnchor = node->valueOf("is_left_anchor", "false") == "true";
                    widget->isRightAnchor = node->valueOf("is_right_anchor", "false") == "true";
                    widget->isTopAnchor = node->valueOf("is_top_anchor", "false") == "true";
                    widget->isBottomAnchor = node->valueOf("is_bottom_anchor", "false") == "true";
                    widget->leftAnchor = std::stoi(node->valueOf("left_anchor", "0"));
                    widget->rightAnchor = std::stoi(node->valueOf("right_anchor", "0"));
                    widget->topAnchor = std::stoi(node->valueOf("top_anchor", "0"));
                    widget->bottomAnchor = std::stoi(node->valueOf("bottom_anchor", "0"));
                    widget->maxWidth = std::stoi(node->valueOf("max_width", "-1"));
                    widget->maxHeight = std::stoi(node->valueOf("max_height", "-1"));
                    widget->texture = node->valueOf("texture");

                    widget->screenWidth = screenWidth;
                    widget->screenHeight = screenHeight;
                    widget->intendedResolutionWidth = intendedResolutionWidth;
                    widget->intendedResolutionHeight = intendedResolutionHeight;

                    if (widget->name == "foobar")
                    {
                        widget->drawOrder = 0;
                    }

                    widget->createOsgView(geode);
#endif

                    group->addChild(widget);

                    eachWidget.push_back(widget);
                }
                else
                {
                    // TODO: report error invalid gui node type
                    log->error("failed to create widget due to invalid type: {}", node->type());
                }
            }
        }
        else
        {
            log->error("failed to read {} from the file system", actualPath);
        }

        eachWidget.sort([](const Widget * lhs, const Widget * rhs)
        {
            return lhs->getDrawOrder() > rhs->getDrawOrder();
        });

        double i = 0;
        const double count = eachWidget.size();

        for (Widget * widget : eachWidget)
        {
            const double z = i++ / count;

            widget->setZ(-z - 1.f);
        }
    }

    void Shell::deactivateInterface(const std::string & name)
    {
        // TODO: remove from geode
        /*
        for (auto itr = eachWidget.begin(); itr != eachWidget.end(); ++itr)
        {
            if ((*itr)->getInterfaceParent() == name)
            {
                // (*itr)->removeFrom(geode);
                itr = eachWidget.erase(itr);
            }
        }
        */

        eachWidget.remove_if([name](const Widget * widget)
        {
            return widget->getInterfaceParent() == name;
        });
    }

    void Shell::showInterface(const std::string & name)
    {
        // TODO: figure out if this is the "UIWidget::Hidden" property

        for (Widget * widget : eachWidget)
        {
            if (widget->getInterfaceParent() == name)
            {
                widget->setHidden(false);
            }
        }
    }

    void Shell::hideInterface(const std::string & name)
    {
        // TODO: figure out if this is the "UIWidget::Hidden" property

        for (Widget * widget : eachWidget)
        {
            if (widget->getInterfaceParent() == name)
            {
                widget->setHidden(true);
            }
        }
    }

    bool Shell::doWidgetsOverlap(const Widget * widget1, const Widget * widget2) const
    {
        if (!widget1) return false;
        if (!widget2) return false;

        // TODO: can't recall coordinate system at the moment... swap y1 & y2?
        return widget1->getRect().x1 < widget2->getRect().x2 && widget1->getRect().x2 > widget2->getRect().x1 && widget1->getRect().y1 > widget2->getRect().y2 && widget1->getRect().y2 < widget2->getRect().y1;
    }

    Widget * Shell::findWidget(const std::string & name, const std::string & interface) const
    {
        for (Widget * widget : eachWidget)
        {
            if (widget->getName() == name && widget->getInterfaceParent() == interface)
            {
                return widget;
            }
        }

        return nullptr;
    }

    void Shell::shiftGroup(const std::string & interface, const std::string & group, int deltaX, int deltaY)
    {
        for (Widget * widget : eachWidget)
        {
            if (widget->getGroup() == group && widget->getInterfaceParent() == interface)
            {
                // TODO: widget->drag(deltaX, deltaY); ?
                // widget->shiftX += deltaX;
                // widget->shiftY += deltaY;
            }
        }
    }

    void Shell::shiftInterface(const std::string & name, int deltaX, int deltaY)
    {
        for (Widget * widget : eachWidget)
        {
            if (widget->getInterfaceParent() == name)
            {
                // TODO: widget->drag(deltaX, deltaY); ?
                // widget->shiftX += deltaX;
                // widget->shiftY += deltaY;
            }
        }
    }

    Widget * Shell::createDefaultWidgetOfType(const std::string & type)
    {
        if (type == "window") return new Widget;
        else if (type == "button") return new Widget;
        else if (type == "checkbox") return new Widget;
        else if (type == "slider") return new Widget;
        else if (type == "listbox") return new Widget;
        else if (type == "radio_button") return new Widget;
        else if (type == "button_multistage") return new Widget;
        else if (type == "text") return new Widget;
        else if (type == "cursor") return new Widget;
        else if (type == "dockbar") return new Widget;
        else if (type == "gridbox") return new Widget;
        else if (type == "popupmenu") return new Widget;
        else if (type == "item") return new Widget;
        else if (type == "itemslot") return new Widget;
        else if (type == "infoslot") return new Widget;
        else if (type == "status_bar") return new Widget;
        else if (type == "edit_box") return new Widget;
        else if (type == "combo_box") return new Widget;
        else if (type == "listener") return new Widget;
        else if (type == "listreport") return new Widget;
        else if (type == "chat_box") return new Widget;
        else if (type == "text_box") return new Widget;
        else if (type == "dialog_box") return new Widget;
        else if (type == "tab") return new Widget;

        return nullptr;
    }

    bool Shell::handle(const osgGA::GUIEventAdapter & event, osgGA::GUIActionAdapter & action)
    {
#if 0
        switch (event.getEventType())
        {
            case osgGA::GUIEventAdapter::NONE: std::cout << "NONE" << std::endl; break;
            case osgGA::GUIEventAdapter::PUSH: std::cout << "PUSH" << std::endl; break;
            case osgGA::GUIEventAdapter::RELEASE: std::cout << "RELEASE" << std::endl; break;
            case osgGA::GUIEventAdapter::DOUBLECLICK: std::cout << "DOUBLECLICK" << std::endl; break;
            case osgGA::GUIEventAdapter::DRAG: std::cout << "DRAG" << std::endl; break;
            case osgGA::GUIEventAdapter::MOVE: std::cout << "MOVE" << std::endl; break;
            case osgGA::GUIEventAdapter::KEYDOWN: std::cout << "KEYDOWN" << std::endl; break;
            case osgGA::GUIEventAdapter::KEYUP: std::cout << "KEYUP" << std::endl; break;
            // case osgGA::GUIEventAdapter::FRAME: std::cout << "FRAME" << std::endl; break;
            case osgGA::GUIEventAdapter::RESIZE: std::cout << "RESIZE" << std::endl; break;
            case osgGA::GUIEventAdapter::SCROLL: std::cout << "SCROLL" << std::endl; break;
            case osgGA::GUIEventAdapter::PEN_PRESSURE: std::cout << "PEN_PRESSURE" << std::endl; break;
            case osgGA::GUIEventAdapter::PEN_ORIENTATION: std::cout << "PEN_ORIENTATION" << std::endl; break;
            case osgGA::GUIEventAdapter::PEN_PROXIMITY_ENTER: std::cout << "PEN_PROXIMITY_ENTER" << std::endl; break;
            case osgGA::GUIEventAdapter::PEN_PROXIMITY_LEAVE: std::cout << "PEN_PROXIMITY_LEAVE" << std::endl; break;
            case osgGA::GUIEventAdapter::CLOSE_WINDOW: std::cout << "CLOSE_WINDOW" << std::endl; break;
            case osgGA::GUIEventAdapter::QUIT_APPLICATION: std::cout << "QUIT_APPLICATION" << std::endl; break;
            case osgGA::GUIEventAdapter::USER: std::cout << "USER" << std::endl; break;
            default: break;
        }
        // std::cout << "got event of type " << event.getEventType() << std::endl;
#endif

        auto log = spdlog::get("log");

        switch (event.getEventType())
        {
            case osgGA::GUIEventAdapter::PUSH:
            {
                for (Widget * widget : eachWidget)
                {
                    if (widget->isPassThrough() != true && event.getX() >= widget->getRect().x1 && event.getX() <= widget->getRect().x2 && event.getY() >= widget->getRect().y1 && event.getY() <= widget->getRect().y2)
                    {
                        log->info("PUSH on {} @ {}, {}", widget->getName(), event.getX(), event.getY());
                        break;
                    }
                }
            }
            break;

            case osgGA::GUIEventAdapter::RELEASE:
            {
                for (Widget * widget : eachWidget)
                {
                    if (widget->isPassThrough() != true && event.getX() >= widget->getRect().x1 && event.getX() <= widget->getRect().x2 && event.getY() >= widget->getRect().y1 && event.getY() <= widget->getRect().y2)
                    {
                        log->info("RELEASE on {} @ {}, {}", widget->getName(), event.getX(), event.getY());
                        break;
                    }
                }
            }
            break;

            case osgGA::GUIEventAdapter::DOUBLECLICK:
            {
            }
            break;

            case osgGA::GUIEventAdapter::DRAG:
            {
            }
            break;

            // case osgGA::GUIEventAdapter::MOVE:

            case osgGA::GUIEventAdapter::KEYDOWN:
            {
            }
            break;

            case osgGA::GUIEventAdapter::KEYUP:
            {
            }
            break;

            case osgGA::GUIEventAdapter::RESIZE:
            {
                const unsigned int lastScreenWidth = screenWidth, lastScreenHeight = screenHeight;

                screenWidth = event.getWindowWidth();
                screenHeight = event.getWindowHeight();

                // window resize vs window move
                if (screenWidth != lastScreenWidth || screenHeight != lastScreenHeight)
                {
                    // TODO: Uncomment the below to enable resizing of the gui in the resize event
                    //camera->setProjectionMatrix(osg::Matrix::ortho2D(0, screenWidth, 0, screenHeight));
                    //camera->setViewport(0, 0, screenWidth, screenHeight);

                    // let each widget know the screen has been resized
                    for (Widget * widget : eachWidget)
                    {
                        // widget->screenSizeChanged(screenWidth, screenHeight);
                    }
                }
                else
                {
                    // the window has been moved only
                }
            }
            break;

            case osgGA::GUIEventAdapter::SCROLL:
            {
                for (Widget * widget : eachWidget)
                {
                    if (widget->isPassThrough() != true && event.getX() >= widget->getRect().x1 && event.getX() <= widget->getRect().x2 && event.getY() >= widget->getRect().y1 && event.getY() <= widget->getRect().y2)
                    {
                        if (event.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP)
                        {
                            log->info("SCROLL_UP on {} @ {}, {}", widget->getName(), event.getX(), event.getY());
                        }

                        if (event.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_DOWN)
                        {
                            log->info("SCROLL_DOWN on {} @ {}, {}", widget->getName(), event.getX(), event.getY());
                        }

                        break;
                    }
                }
            }
            break;

            default:
                break;
        }

        return false;
    }
}
