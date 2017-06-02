
#include <spdlog/spdlog.h>
#include "Rect.hpp"
#include "Widget.hpp"
#include "WidgetBorder.hpp"

namespace ehb
{
    WidgetBorder::WidgetBorder(const Widget & widget) : widget(widget)
    {
        if (osg::Vec3Array * data = new osg::Vec3Array(4))
        {
            const Rect & rect = widget.getRect();
            const int padding = widget.getBorderPadding();
            const float z = widget.getZ();

            spdlog::get("log")->info("initializing WidgetBorder with rect {}, {}, {}, {}", rect.x1, rect.y1, rect.x2, rect.y2);

            (*data)[0].set(rect.x1 - padding, rect.y2 + padding, z);
            (*data)[1].set(rect.x1 - padding, rect.y1 - padding, z);
            (*data)[2].set(rect.x2 + padding, rect.y1 - padding, z);
            (*data)[3].set(rect.x2 + padding, rect.y2 + padding, z);

            setVertexArray(data);
        }

        if (osg::Vec3Array * data = new osg::Vec3Array(1))
        {
            (*data)[0].set(0.f, 0.f, 1.f);

            setNormalArray(data, osg::Array::BIND_OVERALL);
        }

        if (osg::Vec4Array * data = new osg::Vec4Array(1))
        {
            /*
            const osg::Color & color = widget.getBorderColor();

            (*data)[0].set(widget.getBorderColor().r, widget.getBorderColor().g, widget.getBorderColor().b, widget.getBorderColor().a);
            */

            (*data)[0].set(1.f, 1.f, 1.f, 1.f);

            setColorArray(data, osg::Array::BIND_OVERALL);
        }

        addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4));
        // TODO: getOrCreateStateSet()->setAttribute(new osg::LineWidth(widget.getBorderPadding()));
    }

    void WidgetBorder::dragWindow(int deltaX, int deltaY)
    {
        updateVertexArray();
    }

    void WidgetBorder::resize(unsigned int, unsigned int)
    {
        updateVertexArray();
    }

    void WidgetBorder::setNormalizedRect(float left, float right, float top, float bottom)
    {
    }

    void WidgetBorder::setRect(int, int, int, int)
    {
        updateVertexArray();
    }

    void WidgetBorder::setBorder(bool value)
    {
    }

    void WidgetBorder::setBorderColor(float r, float g, float b, float a)
    {
        if (osg::Vec4Array * data = dynamic_cast<osg::Vec4Array *>(getColorArray()))
        {
            (*data)[0].set(r, g, b, a);
        }
    }

    void WidgetBorder::setBorderPadding(int value)
    {
        // TODO: getOrconst Widget & widgetCreateStateSet()->setAttribute(new osg::LineWidth(value));
        updateVertexArray();
    }

    void WidgetBorder::setHidden(bool value)
    {
    }

    void WidgetBorder::setNormalizeResize(bool value)
    {
    }

    void WidgetBorder::setScale(float value)
    {
    }

    void WidgetBorder::setTopmost(bool)
    {
        updateVertexArray();
    }

    void WidgetBorder::setVisible(bool value)
    {
    }

    void WidgetBorder::setZ(float)
    {
        updateVertexArray();
    }

    void WidgetBorder::updateVertexArray()
    {
        if (osg::Vec3Array * data = dynamic_cast<osg::Vec3Array *>(getVertexArray()))
        {
            const Rect & rect = widget.getRect();
            const int padding = widget.getBorderPadding();
            const float z = widget.getZ();

            (*data)[0].set(rect.x1 - padding, rect.y2 + padding, z);
            (*data)[1].set(rect.x1 - padding, rect.y1 - padding, z);
            (*data)[2].set(rect.x2 + padding, rect.y1 - padding, z);
            (*data)[3].set(rect.x2 + padding, rect.y2 + padding, z);
        }
    }
}
