
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

    void Widget::dragWidget(int deltaX, int deltaY)
    {
        // TODO: confirm if this is correct
        shiftX += deltaX;
        shiftY += deltaY;

        updateCache();

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->dragWidget(deltaX, deltaY);
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
        // TODO: confirm if this is correct
        rect.x2 = rect.x1 + width;
        rect.y2 = rect.y1 + height;

        updateCache();

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
        // TODO: figure out what to do here
        normalizedRect.left = left;
        normalizedRect.right = right;
        normalizedRect.top = top;
        normalizedRect.bottom = bottom;

        updateCache();

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
        // TODO: do i set rectCache or rect here?
        // NOTE: ds gui coords are left to right, top to bottom
        rect.x1 = left;
        rect.x2 = right;
        rect.y1 = top;
        rect.y2 = bottom;

        updateCache();

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                component->setRect(left, right, top, bottom);
            }
        }
    }

    void Widget::setScreenSize(unsigned int width, unsigned int height)
    {
        // TODO: recalculate the widget position
        screenWidth = width;
        screenHeight = height;

        updateCache();

        for (unsigned int i = 0; i < getNumDrawables(); i++)
        {
            if (IWidgetComponent * component = dynamic_cast<IWidgetComponent *>(getDrawable(i)))
            {
                // TODO: figure out what to call here
                // component->set...();
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

    void Widget::updateCache()
    {
        // TODO: figure out how to use scale, normalizedRect, and normalizeResize

        // TODO: determine if stretch works when you go below intended_resolution_*
        const double stretchFactorX = intendedResolutionWidth && screenWidth > *intendedResolutionWidth ? static_cast<double>(screenWidth) / static_cast<double>(*intendedResolutionWidth) : 1;
        const double stretchFactorY = intendedResolutionHeight && screenHeight > *intendedResolutionHeight ? static_cast<double>(screenHeight) / static_cast<double>(*intendedResolutionHeight) : 1;

        const unsigned int widgetWidth = rect.x2 > rect.x1 ? static_cast<unsigned int>(rect.x2 - rect.x1) : 0;
        const unsigned int widgetHeight = rect.y2 > rect.y1 ? static_cast<unsigned int>(rect.y2 - rect.y1) : 0;

        unsigned int newWidgetWidth = stretchX ? static_cast<double>(widgetWidth) * stretchFactorX : widgetWidth;
        unsigned int newWidgetHeight = stretchY ? static_cast<double>(widgetHeight) * stretchFactorY : widgetHeight;

        if (maxWidth && newWidgetWidth > maxWidth) newWidgetWidth = *maxWidth;
        if (maxHeight && newWidgetHeight > maxHeight) newWidgetHeight = *maxHeight;

        if (centerX) rectCache.x1 = screenWidth > newWidgetWidth ? (screenWidth - newWidgetWidth) / 2 : (newWidgetWidth - screenWidth) / 2; /* i guess... or zero */
        else if (leftAnchor) rectCache.x1 = *leftAnchor;
        else if (rightAnchor) rectCache.x1 = screenWidth - *rightAnchor; // FIXME: what if rightAnchor is negative
        else rectCache.x1 = rect.x1;

        rectCache.x1 += shiftX;
        rectCache.x2 = rectCache.x1 + newWidgetWidth;

        if (centerY) rectCache.y1 = screenHeight > newWidgetHeight ? (screenHeight - newWidgetHeight) / 2 : (newWidgetHeight - screenHeight) / 2; /* i guess... or zero */
        else if (topAnchor) rectCache.y1 = screenHeight - *topAnchor; // FIXME: what if topAnchor is negative
        else if (bottomAnchor) rectCache.y1 = *bottomAnchor;
        else rectCache.y1 = rect.y1;

        rectCache.y1 += shiftY;
        // rectCache.y1 -= newWidgetHeight; // account for yet another difference between ds and osg coordinate systems
        rectCache.y2 = rectCache.y1 + newWidgetHeight;

        // log->info("{} has rectCache {} vs rect {}", name, toString(rectCache), toString(rect));
    }
}
