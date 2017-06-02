
#pragma once

#include <string>
#include <optional.hpp>
#include <osg/Geode>
#include "Rect.hpp"

namespace ehb
{
    class Widget : public osg::Geode
    {
    public:

        Widget();

        virtual ~Widget() = default;

        // widget methods
        virtual void createCommonCtrl(const std::string & commonTemplate);

        void dragWidget(int deltaX, int deltaY);

        void loadTexture(const std::string & filename, bool resize);

        // bool processAction(UI_ACTION, const string &);
        // bool receiveMessage(const UIMessage &);

        // TODO: figure out what ds does in this situation when stretchX or stretchY is set to true
        void resize(unsigned int width, unsigned int height);

        void setNormalizedRect(float left, float right, float top, float bottom);

        // TODO: figure out what ds does in this situation when centerX or centerY is set to true
        void setRect(int left, int right, int top, int bottom);

        void setScreenSize(unsigned int width, unsigned int height); // TODO: this should only be callable by Shell...?

        void setUVRect(float left, float right, float top, float bottom);

        // widget properties
        float getAlpha() const;
        void setAlpha(float value);

        bool hasBackgroundFill() const;
        void setBackgroundFill(bool value);

        // TODO: use the color class?
        void setBackgroundColor(float r, float g, float b, float a = 1.f);

        bool hasBorder() const;
        void setBorder(bool value);

        // TODO: use the color class?
        void setBorderColor(float r, float g, float b, float a = 1.f);

        int getBorderPadding() const;
        void setBorderPadding(int value);

        bool isCommonControl() const;

        const std::string & getCommonTemplate() const;

        int getDrawOrder() const;
        void setDrawOrder(int value); // TODO: this property should only be set via .gas template loading

        const std::string & getGroup() const;
        void setGroup(const std::string & value); // TODO: this property should only be set via .gas template loading...?

        // TODO: figure out what ds does in the situation when stretchY is set to true
        unsigned int getHeight() const;

        bool isHidden() const;
        void setHidden(bool value);

        const std::string & getInterfaceParent() const;
        void setInterfaceParent(const std::string & value); // TODO: this property should only be set via .gas template loading

        const std::string & getName() const;
        void setName(const std::string & value); // TODO: this property should only be set via .gas template loading...?

        const NormalizedRect & getNormalizedRect() const;

        bool getNormalizeResize() const;
        void setNormalizeResize(bool value);

        bool isPassThrough() const;
        void setPassThrough(bool value); // TODO: this property should only be set via .gas template loading...?

        // TODO: figure out if ds returns rect or rectCache
        const Rect & getRect() const;

        float getScale() const;
        void setScale(float value);

        bool isTopmost() const;
        void setTopmost(bool value);

        const NormalizedRect & getUVRect() const;

        bool isVisible() const;
        void setVisible(bool value);

        // TODO: figure out what ds does in the situation when stretchX is set to true
        unsigned int getWidth() const;

        float getZ() const;
        void setZ(float value); // TODO: this property should only be set by Shell

    private:

        void updateCache();

    private:

        float alpha = 1.f;
        bool backgroundFill = false;
        struct { float r, g, b, a; } backgroundColor;
        bool border = false;
        struct { float r, g, b, a; } borderColor;
        int borderPadding = 0;
        nonstd::optional<int> bottomAnchor;
        bool centerX = false, centerY = false;
        std::string commonTemplate;
        int drawOrder = 0;
        std::string group;
        bool hidden = false;
        nonstd::optional<unsigned int> intendedResolutionHeight, intendedResolutionWidth;
        std::string interfaceParent;
        nonstd::optional<int> leftAnchor;
        nonstd::optional<unsigned int> maxHeight, maxWidth;
        std::string name;
        NormalizedRect normalizedRect;
        bool normalizeResize = false;
        bool passThrough = false;
        Rect rect; // rect of the widget
        Rect rectCache; // calculated rect of the widget factoring in *everything* (anchors, centering, intended interface resolution, max sizes, normalization, scaling, screen size, shifting, and stretching)
        nonstd::optional<int> rightAnchor;
        float scale = 1.f;
        unsigned int screenHeight = 0, screenWidth = 0;
        int shiftX = 0, shiftY = 0;
        bool stretchX = false, stretchY = false;
        bool topmost = false;
        nonstd::optional<int> topAnchor;
        NormalizedRect uvrect;
        bool visible = true;
        float z = -1.f;

    };

    inline float Widget::getAlpha() const
    {
        return alpha;
    }

    inline bool Widget::hasBackgroundFill() const
    {
        return backgroundFill;
    }

    inline bool Widget::hasBorder() const
    {
        return border;
    }

    inline int Widget::getBorderPadding() const
    {
        return borderPadding;
    }

    inline bool Widget::isCommonControl() const
    {
        return commonTemplate.empty() != true;
    }

    inline const std::string & Widget::getCommonTemplate() const
    {
        return commonTemplate;
    }

    inline int Widget::getDrawOrder() const
    {
        return drawOrder;
    }

    inline void Widget::setDrawOrder(int value)
    {
        drawOrder = value;
    }

    inline const std::string & Widget::getGroup() const
    {
        return group;
    }

    inline void Widget::setGroup(const std::string & value)
    {
        group = value;
    }

    inline const std::string & Widget::getName() const
    {
        return name;
    }

    inline void Widget::setName(const std::string & value)
    {
        name = value;
    }

    inline unsigned int Widget::getHeight() const
    {
        return rectCache.y2 > rectCache.y1 ? static_cast<unsigned int>(rectCache.y2 - rectCache.y1) : 0;
    }

    inline bool Widget::isHidden() const
    {
        return hidden;
    }

    inline const std::string & Widget::getInterfaceParent() const
    {
        return interfaceParent;
    }

    inline void Widget::setInterfaceParent(const std::string & value)
    {
        interfaceParent = value;
    }

    inline const NormalizedRect & Widget::getNormalizedRect() const
    {
        return normalizedRect;
    }

    inline bool Widget::getNormalizeResize() const
    {
        return normalizeResize;
    }

    inline bool Widget::isPassThrough() const
    {
        return passThrough;
    }

    inline void Widget::setPassThrough(bool value)
    {
        passThrough = value;
    }

    inline const Rect & Widget::getRect() const
    {
        return rectCache;
    }

    inline float Widget::getScale() const
    {
        return scale;
    }

    inline bool Widget::isTopmost() const
    {
        return topmost;
    }

    inline const NormalizedRect & Widget::getUVRect() const
    {
        return uvrect;
    }

    inline bool Widget::isVisible() const
    {
        return visible;
    }

    inline unsigned int Widget::getWidth() const
    {
        return rectCache.x2 > rectCache.x1 ? static_cast<unsigned int>(rectCache.x2 - rectCache.x1) : 0;
    }

    inline float Widget::getZ() const
    {
        return topmost ? -1.f : z;
    }
}

#if 0
namespace ehb
{
    class Widget
    {
    protected:

        int shiftX = 0, shiftY = 0;
    };
}
#endif
