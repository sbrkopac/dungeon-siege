
#include <spdlog/spdlog.h>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/Registry>
#include <osgGA/GUIEventAdapter>
#include "IWidgetComponent.hpp"
#include "Widget.hpp"

#include "WidgetBody.hpp"
#include "WidgetBorder.hpp"

namespace ehb
{
    Widget::Widget()
    {
        addDrawable(new WidgetBorder(*this));
        addDrawable(new WidgetBody(*this));
    }

    void Widget::createCommonCtrl(const std::string & value)
    {
        commonTemplate = value;
    }

    void Widget::dragWindow(int deltaX, int deltaY)
    {
        // TODO: calculate the position factoring in center_x, etc...

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->dragWindow(deltaX, deltaY);
            }
        }
    }

    void Widget::loadTexture(const std::string & filename, bool resize)
    {
        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->loadTexture(filename, resize);
            }
        }
    }

    void Widget::resize(unsigned int width, unsigned int height)
    {
        // TODO: calculate the position factoring in center_x, etc...

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->resize(width, height);
            }
        }
    }

    void Widget::setNormalizedRect(float left, float right, float top, float bottom)
    {
        // TODO: calculate the position factoring in center_x, etc...
        normalizedRect.left = left;
        normalizedRect.right = right;
        normalizedRect.top = top;
        normalizedRect.bottom = bottom;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setNormalizedRect(left, right, top, bottom);
            }
        }
    }

    void Widget::setRect(int left, int right, int top, int bottom)
    {
        // TODO: calculate the position factoring in center_x, etc...
        // TODO: do i set rect or originalrect here?
        // NOTE: ds gui coords are left to right, top to bottom
        rect.x1 = left;
        rect.x2 = right;
        rect.y1 = top;
        rect.y2 = bottom;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setRect(left, right, top, bottom);
            }
        }
    }

    void Widget::setUVRect(float left, float right, float top, float bottom)
    {
        uvrect.left = left;
        uvrect.right = right;
        uvrect.top = top;
        uvrect.bottom = bottom;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setUVRect(left, right, top, bottom);
            }
        }
    }

    void Widget::setAlpha(float value)
    {
        alpha = value;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setAlpha(value);
            }
        }
    }

    void Widget::setBackgroundFill(bool value)
    {
        backgroundFill = value;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setBackgroundFill(value);
            }
        }
    }

    void Widget::setBackgroundColor(float r, float g, float b, float a)
    {
        backgroundColor.r = r;
        backgroundColor.g = g;
        backgroundColor.b = b;
        backgroundColor.a = a;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setBackgroundColor(r, g, b, a);
            }
        }
    }

    void Widget::setBorder(bool value)
    {
        border = value;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {

            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setBorder(value);
            }
        }
    }

    void Widget::setBorderColor(float r, float g, float b, float a)
    {
        borderColor.r = r;
        borderColor.g = g;
        borderColor.b = b;
        borderColor.a = a;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {

            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setBorderColor(r, g, b, a);
            }
        }
    }

    void Widget::setBorderPadding(int value)
    {
        borderPadding = value;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {

            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setBorderPadding(value);
            }
        }
    }

    void Widget::setHidden(bool value)
    {
        hidden = value;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {

            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setHidden(value);
            }
        }
    }

    void Widget::setNormalizeResize(bool value)
    {
        // TODO: figure what this is supposed to do and then do it
        normalizeResize = value;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {

            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setNormalizeResize(value);
            }
        }
    }

    void Widget::setScale(float value)
    {
        // TODO: figure what this is supposed to do and then do it
        scale = value;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {

            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setScale(value);
            }
        }
    }

    void Widget::setTopmost(bool value)
    {
        topmost = value;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {

            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setTopmost(value);
            }
        }
    }

    void Widget::setVisible(bool value)
    {
        visible = value;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {

            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setVisible(value);
            }
        }
    }

    void Widget::setZ(float value)
    {
        z = value;

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {

            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setZ(value);
            }
        }
    }
}

namespace ehb
{
#if 0
    void Widget::setAlpha(float value)
    {
        alpha = value;

        for (osg::Geometry * geometry : view)
        {
            auto array = static_cast<osg::Vec4Array *>(geometry->getColorArray());

            if (backgroundFill)
            {
                // TODO: set the color of the widget from backgroundColor
                array->at(0).set(1.f, 1.f, 1.f, alpha);
            }
            else
            {
                array->at(0).set(1.f, 1.f, 1.f, alpha);
            }
        }
    }

    void Widget::setUVRect(float left, float right, float top, float bottom)
    {
        uvrect.left = left;
        uvrect.right = right;
        uvrect.top = top;
        uvrect.bottom = bottom;

        for (osg::Geometry * geometry : view)
        {
            osg::Vec2Array * texcoords = static_cast<osg::Vec2Array *>(geometry->getTexCoordArray(0));

            texcoords->at(0).set(uvrect.left, uvrect.bottom); // lower left
            texcoords->at(1).set(uvrect.left, uvrect.top); // lower right
            texcoords->at(2).set(uvrect.right, uvrect.top); // upper right
            texcoords->at(3).set(uvrect.right, uvrect.bottom); // upper left
        }
    }


    void Widget::createCommonCtrl(const std::string & sTemplate)
    {
        auto log = spdlog::get("log");

        log->warn("can't create a common control of type Widget");
    }

    void Widget::screenSizeChanged(unsigned int newScreenWidth, unsigned int newScreenHeight)
    {
        auto log = spdlog::get("log");

        screenWidth = newScreenWidth;
        screenHeight = newScreenHeight;
    }

    void Widget::createOsgView(osg::Geode * geode)
    {
        // spdlog::get("log")->info("Widget::createOsgView called");

        {
            osg::Geometry * geometry = osg::createTexturedQuadGeometry(osg::Vec3(0.f, 0.f, 0.f), {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, uvrect.left, uvrect.top, uvrect.right, uvrect.bottom);

            geode->addDrawable(geometry);
            view.push_back(geometry);
        }
    }

    void Widget::updateOsgView(float z)
    {
        auto log = spdlog::get("log");

        // assert(screenWidth != -1 && screenHeight != -1)

        // TODO: determine if stretch works when you go below intended_resolution_*
        const double stretchFactorX = intendedResolutionWidth != -1 && screenWidth > intendedResolutionWidth ? static_cast<double>(screenWidth) / static_cast<double>(intendedResolutionWidth) : 1;
        const double stretchFactorY = intendedResolutionHeight != -1 && screenHeight > intendedResolutionHeight ? static_cast<double>(screenHeight) / static_cast<double>(intendedResolutionHeight) : 1;

        const unsigned int widgetWidth = originalRect.x2 > originalRect.x1 ? static_cast<unsigned int>(originalRect.x2 - originalRect.x1) : 0;
        const unsigned int widgetHeight = originalRect.y2 > originalRect.y1 ? static_cast<unsigned int>(originalRect.y2 - originalRect.y1) : 0;

        unsigned int newWidgetWidth = stretchX ? static_cast<double>(widgetWidth) * stretchFactorX : widgetWidth;
        unsigned int newWidgetHeight = stretchY ? static_cast<double>(widgetHeight) * stretchFactorY : widgetHeight;

        if (maxWidth != -1 && newWidgetWidth > maxWidth) newWidgetWidth = maxWidth;
        if (maxHeight != -1 && newWidgetHeight > maxHeight) newWidgetHeight = maxHeight;

        if (centerX) rect.x1 = screenWidth > newWidgetWidth ? (screenWidth - newWidgetWidth) / 2 : (newWidgetWidth - screenWidth) / 2; /* i guess... or zero */
        else if (isLeftAnchor) rect.x1 = leftAnchor;
        else if (isRightAnchor) rect.x1 = screenWidth - rightAnchor;
        else rect.x1 = originalRect.x1;

        rect.x1 += shiftX;
        rect.x2 = rect.x1 + newWidgetWidth;

        if (centerY) rect.y1 = screenHeight > newWidgetHeight ? (screenHeight - newWidgetHeight) / 2 : (newWidgetHeight - screenHeight) / 2; /* i guess... or zero */
        else if (isTopAnchor) rect.y1 = screenHeight - topAnchor;
        else if (isBottomAnchor) rect.y1 = bottomAnchor;
        else rect.y1 = originalRect.y1;

        rect.y1 += shiftY;
        rect.y1 -= newWidgetHeight; // account for yet another difference between ds and osg coordinate systems
        rect.y2 = rect.y1 + newWidgetHeight;

        // log->info("{} has rect {} vs original rect {}", name, toString(rect), toString(originalRect));

        assert(view.size() == 1);
        {
            osg::Geometry * geometry = view.at(0);

            osg::Vec3Array * vertices = static_cast<osg::Vec3Array *>(geometry->getVertexArray());
            osg::Vec4Array * colors = static_cast<osg::Vec4Array *>(geometry->getColorArray());
            osg::Vec2Array * texcoords = static_cast<osg::Vec2Array *>(geometry->getTexCoordArray(0));
            osg::StateSet * stateSet = geometry->getOrCreateStateSet();

            vertices->at(0).set(rect.x1, rect.y2, z);
            vertices->at(1).set(rect.x1, rect.y1, z);
            vertices->at(2).set(rect.x2, rect.y1, z);
            vertices->at(3).set(rect.x2, rect.y2, z);

            if (backgroundFill)
            {
                // TODO: set the color of the widget from backgroundColor
                colors->at(0).set(1.f, 1.f, 1.f, alpha);
            }
            else
            {
                colors->at(0).set(1.f, 1.f, 1.f, alpha);
            }

            // this looks right on data_bar.gas... crossing fingers
            texcoords->at(0).set(uvrect.left, uvrect.bottom); // lower left
            texcoords->at(1).set(uvrect.left, uvrect.top); // lower right
            texcoords->at(2).set(uvrect.right, uvrect.top); // upper right
            texcoords->at(3).set(uvrect.right, uvrect.bottom); // upper left

            if (!texture.empty())
            {
                auto rc = osgDB::Registry::instance()->readImage(this->texture + ".raw", nullptr);

                // log->info("attempting to load: {}.raw", this->texture);

                if (rc.validImage())
                {
                    osg::Texture2D * texture = new osg::Texture2D;
                    texture->setImage(rc.takeImage());
                    // log->info("found texture: {}", (this->texture + ".raw"));

                    stateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
                }
                else
                {
                    log->error("error loading image: {}", rc.message());
                }
            }

            stateSet->setMode(GL_BLEND,osg::StateAttribute::ON);
            stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        }
    }

    void Widget::removeOsgView(osg::Geode * geode)
    {
        for (auto geometry : view)
        {
            geode->removeDrawable(geometry);
        }

        view.clear();
    }
#endif
}
