
#pragma once

#include <string>

namespace ehb
{
    class IWidgetComponent
    {
    public:

        virtual ~IWidgetComponent() = default;

        // widget methods
        virtual void createCommonCtrl(const std::string & commonTemplate) {}
        virtual void dragWindow(int deltaX, int deltaY) {}
        virtual void loadTexture(const std::string & filename, bool resize) {}
        // virtual bool processAction(UI_ACTION, const string &) {}
        // virtual bool receiveMessage(const UIMessage &) {}
        virtual void resize(unsigned int width, unsigned int height) {}
        virtual void setNormalizedRect(float left, float right, float top, float bottom) {}
        virtual void setRect(int left, int right, int top, int bottom) {}
        virtual void setUVRect(float left, float right, float top, float bottom) {}

        // widget properties
        virtual void setAlpha(float value) {}
        virtual void setBackgroundColor(float r, float g, float b, float a) {}
        virtual void setBackgroundFill(bool value) {}
        virtual void setBorder(bool value) {}
        virtual void setBorderColor(float r, float g, float b, float a) {}
        virtual void setBorderPadding(int value) {}
        virtual void setHidden(bool value) {}
        virtual void setNormalizeResize(bool value) {}
        virtual void setScale(float value) {}
        virtual void setTopmost(bool value) {}
        virtual void setVisible(bool value) {}
        virtual void setZ(float value) {}
    };
}
