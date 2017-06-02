
#pragma once

#include <string>
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

        void dragWindow(int deltaX, int deltaY);

        void loadTexture(const std::string & filename, bool resize);

        // bool processAction(UI_ACTION, const string &);
        // bool receiveMessage(const UIMessage &);

        void resize(unsigned int width, unsigned int height);

        void setNormalizedRect(float left, float right, float top, float bottom);

        void setRect(int left, int right, int top, int bottom);

        void setUVRect(float left, float right, float top, float bottom);

        // widget properties
        float getAlpha() const;
        void setAlpha(float value);

        bool getBackgroundFill() const;
        void setBackgroundFill(bool value);

        // TODO: use the color class?
        void setBackgroundColor(float r, float g, float b, float a = 1.f);

        bool getBorder() const;
        void setBorder(bool value);

        // TODO: use the color class?
        void setBorderColor(float r, float g, float b, float a = 1.f);

        int getBorderPadding() const;
        void setBorderPadding(int value);

        bool isCommonControl() const;

        const std::string & getCommonTemplate() const;

        int getDrawOrder() const;
        void setDrawOrder(int value); // TODO: remove this method, don't want people changing this? or if they can, then has to make Shell recalculate z of everything

        const std::string & getGroup() const;
        void setGroup(const std::string & value); // TODO: don't have this here?

        bool getHidden() const;
        void setHidden(bool value);

        const std::string & getName() const;
        void setName(const std::string & value); // TODO: don't have this here

        const std::string & getInterfaceParent() const;
        void setInterfaceParent(const std::string & value); // TODO: definitely don't have this here

        const NormalizedRect & getNormalizedRect() const;

        bool getNormalizeResize() const;
        void setNormalizeResize(bool value);

        bool isPassThrough() const;
        void setPassThrough(bool value); // TODO: do something with this...

        const Rect & getRect() const;

        float getScale() const;
        void setScale(float value);

        bool getTopmost() const;
        void setTopmost(bool value);

        const NormalizedRect & getUVRect() const;

        bool getVisible() const;
        void setVisible(bool value);

        float getZ() const;
        void setZ(float value);

    private:

        // using nonstd::optional;

        float alpha = 1.f;
        bool backgroundFill = false;
        struct { float r, g, b, a; } backgroundColor;
        bool border = false;
        struct { float r, g, b, a; } borderColor;
        int borderPadding = 0;
        std::string commonTemplate;
        int drawOrder = 0;
        std::string group;
        bool hidden = false;
        std::string interfaceParent;
        std::string name;
        NormalizedRect normalizedRect;
        bool normalizeResize = false;
        bool passThrough = false;
        Rect rect;
        float scale = 1.f;
        bool topmost = false;
        NormalizedRect uvrect;
        bool visible = true;
        float z = -1.f;

        // TODO: store the original rect value from gas file

    };

    inline float Widget::getAlpha() const
    {
        return alpha;
    }

    inline bool Widget::getBackgroundFill() const
    {
        return backgroundFill;
    }

    inline bool Widget::getBorder() const
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

    inline bool Widget::getHidden() const
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
        return rect;
    }

    inline float Widget::getScale() const
    {
        return scale;
    }

    inline bool Widget::getTopmost() const
    {
        return topmost;
    }

    inline const NormalizedRect & Widget::getUVRect() const
    {
        return uvrect;
    }

    inline bool Widget::getVisible() const
    {
        return visible;
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
        // TODO: no thank you...
        friend class Shell;
    public:

        Widget() {}

        virtual ~Widget() = default;

        float getAlpha() const;
        // void setAlpha(float value);

        const std::string & getName() const;
        const std::string & getGroup() const;
        const std::string & getInterfaceParent() const;

        unsigned int getDrawOrder() const;

        bool isPassThrough() const;

        // TODO: figure out what ds does in the situation when stretchX or stretchY is set to true
        unsigned int getWidth() const;
        unsigned int getHeight() const;

        bool isCommonControl() const;
        bool isHidden() const;
        bool isVisible() const;

        // void setUVRect(float left, float right, float top, float bottom);

        virtual void createCommonCtrl(const std::string & sTemplate);

        // TODO: figure out what ds does in this situation when stretchX or stretchY is set to true
        // void resize(unsigned int width, unsigned int height);

        // TODO: figure out what ds does in this situation when centerX or centerY is set to true
        // void setRect(int left, int right, int top, int bottom /*, bool bAnimation */);

        void screenSizeChanged(unsigned int screenWidth, unsigned int screenHeight);

    private:

        // only to be called by Shell...?
        virtual void createOsgView(osg::Geode * geode);
        virtual void updateOsgView(float z);
        virtual void removeOsgView(osg::Geode * geode);

    protected:

        float alpha = 1.0f;
        unsigned int drawOrder = 0;
        std::string name;
        std::string group;
        std::string interfaceParent;
        Rect originalRect, rect;
        NormalizedRect uvrect;

        // std::optional<unsigned int> backgroundColor;
        unsigned int backgroundColor;
        bool backgroundFill = false;
        // std::optional<unsigned int> borderColor;
        bool border = false;
        unsigned int borderColor;

        bool passThrough = false;
        bool hidden = false;
        bool visible = true;

        // std::optional<std::string> commonTemplate;
        bool commonControl = false;
        std::string commonTemplate;

        std::string texture;

        bool centerX = false, centerY = false, stretchX = false, stretchY = false;
        // std::optional<int> leftAnchor, rightAnchor, topAnchor, bottomAnchor;
        bool isLeftAnchor = false, isRightAnchor = false, isTopAnchor = false, isBottomAnchor = false;
        int leftAnchor, rightAnchor, topAnchor, bottomAnchor;

        // std::optional<unsigned int> maxWidth, maxHeight;
        int maxWidth = -1, maxHeight = -1;
        int screenWidth = -1, screenHeight = -1;
        int intendedResolutionWidth = -1, intendedResolutionHeight = -1;

        int shiftX = 0, shiftY = 0;

        std::vector<osg::Geometry *> view;
    };

    inline float Widget::getAlpha() const
    {
        return alpha;
    }

    inline const std::string & Widget::getName() const
    {
        return name;
    }

    inline const std::string & Widget::getGroup() const
    {
        return group;
    }

    inline const std::string & Widget::getInterfaceParent() const
    {
        return interfaceParent;
    }

    inline unsigned int Widget::getDrawOrder() const
    {
        return drawOrder;
    }

    inline bool Widget::isPassThrough() const
    {
        return passThrough;
    }

    inline unsigned int Widget::getWidth() const
    {
        return rect.x2 > rect.x1 ? static_cast<unsigned int>(rect.x2 - rect.x1) : 0;
    }

    inline unsigned int Widget::getHeight() const
    {
        return rect.y2 > rect.y1 ? static_cast<unsigned int>(rect.y2 - rect.y1) : 0;
    }

    inline bool Widget::isCommonControl() const
    {
        return commonControl;
    }

    inline bool Widget::isHidden() const
    {
        return hidden;
    }

    inline bool Widget::isVisible() const
    {
        return visible;
    }
}
#endif
