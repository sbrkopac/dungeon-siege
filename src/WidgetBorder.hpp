
#pragma once

#include <osg/Geometry>
#include "IWidgetComponent.hpp"

namespace ehb
{
    class Widget;
    class WidgetBorder : public IWidgetComponent, public osg::Geometry
    {
    public:

        WidgetBorder(const Widget & widget);

        virtual ~WidgetBorder() = default;

        virtual void dragWindow(int deltaX, int deltaY) override;
        virtual void resize(unsigned int width, unsigned int height) override;
        virtual void setNormalizedRect(float left, float right, float top, float bottom) override;
        virtual void setRect(int left, int right, int top, int bottom) override;
        virtual void setBorder(bool value) override;
        virtual void setBorderColor(float r, float g, float b, float a) override;
        virtual void setBorderPadding(int value) override;
        virtual void setHidden(bool value) override;
        virtual void setNormalizeResize(bool value) override;
        virtual void setScale(float value) override;
        virtual void setTopmost(bool value) override;
        virtual void setVisible(bool value) override;
        virtual void setZ(float value) override;

    private:

        void updateVertexArray();

    private:

        const Widget & widget;
    };
}
