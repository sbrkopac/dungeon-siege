
#include <spdlog/spdlog.h>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include "Rect.hpp"
#include "Widget.hpp"
#include "WidgetBody.hpp"

namespace ehb
{
    WidgetBody::WidgetBody(const Widget & widget) : widget(widget)
    {
        if (osg::Vec3Array * data = new osg::Vec3Array(4))
        {
            const Rect & rect = widget.getRect();
            const float z = widget.getZ();

            spdlog::get("log")->info("initializing WidgetBody with rect {}, {}, {}, {}", rect.x1, rect.y1, rect.x2, rect.y2);

            (*data)[0].set(rect.x1, rect.y2, z);
            (*data)[1].set(rect.x1, rect.y1, z);
            (*data)[2].set(rect.x2, rect.y1, z);
            (*data)[3].set(rect.x2, rect.y2, z);

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
            const osg::Color & color = widget.getBackgroundColor();

            (*data)[0].set(color.r, color.g, color.b, color.a);
            */

            (*data)[0].set(1.f, 1.f, 1.f, 1.f);

            setColorArray(data, osg::Array::BIND_OVERALL);
        }

        if (osg::Vec2Array * data = new osg::Vec2Array(4))
        {
            const NormalizedRect & rect = widget.getUVRect();

            (*data)[0].set(rect.left, rect.bottom);
            (*data)[1].set(rect.left, rect.top);
            (*data)[2].set(rect.right, rect.top);
            (*data)[3].set(rect.right, rect.bottom);

            setTexCoordArray(0, data);
        }

        addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

        getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
        getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    }

    void WidgetBody::dragWindow(int deltaX, int deltaY)
    {
        updateVertexArray();
    }

    void WidgetBody::loadTexture(const std::string & filename, bool resize)
    {
        // TODO: use the resize argument
        if (osg::ref_ptr<osg::Image> image = osgDB::readImageFile(filename))
        {
            osg::Texture2D * texture = new osg::Texture2D;
            texture->setImage(image);

            getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
        }
    }

    void WidgetBody::resize(unsigned int, unsigned int)
    {
        updateVertexArray();
    }

    void WidgetBody::setNormalizedRect(float left, float right, float top, float bottom)
    {
    }

    void WidgetBody::setRect(int, int, int, int)
    {
        updateVertexArray();
    }

    void WidgetBody::setUVRect(float left, float right, float top, float bottom)
    {
        if (osg::Vec2Array * data = dynamic_cast<osg::Vec2Array *>(getTexCoordArray(0)))
        {
            (*data)[0].set(left, bottom);
            (*data)[1].set(left, top);
            (*data)[2].set(right, top);
            (*data)[3].set(right, bottom);
        }
    }

    void WidgetBody::setBackgroundColor(float r, float g, float b, float a)
    {
        if (osg::Vec4Array * data = dynamic_cast<osg::Vec4Array *>(getColorArray()))
        {
            (*data)[0].set(r, g, b, a);
        }
    }

    void WidgetBody::setBorder(bool value)
    {
    }

    void WidgetBody::setBorderPadding(int value)
    {
        updateVertexArray();
    }

    void WidgetBody::setHidden(bool value)
    {
    }

    void WidgetBody::setNormalizeResize(bool value)
    {
    }

    void WidgetBody::setScale(float value)
    {
    }

    void WidgetBody::setTopmost(bool)
    {
        updateVertexArray();
    }

    void WidgetBody::setVisible(bool value)
    {
    }

    void WidgetBody::setZ(float)
    {
        updateVertexArray();
    }

    void WidgetBody::updateVertexArray()
    {
        if (osg::Vec3Array * data = dynamic_cast<osg::Vec3Array *>(getVertexArray()))
        {
            const Rect & rect = widget.getRect();
            const float z = widget.getZ();

            (*data)[0].set(rect.x1, rect.y2, z);
            (*data)[1].set(rect.x1, rect.y1, z);
            (*data)[2].set(rect.x2, rect.y1, z);
            (*data)[3].set(rect.x2, rect.y2, z);
        }
    }
}
