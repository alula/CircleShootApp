#ifndef __CREDITSSCREEN_H__
#define __CREDITSSCREEN_H__

#include <SexyAppFramework/Widget.h>
#include <SexyAppFramework/HyperlinkWidget.h>
#include <SexyAppFramework/ButtonListener.h>
#include <SexyAppFramework/CheckboxListener.h>

namespace Sexy
{
    class CircleCheckbox;
    class CircleButton;

    class CreditsScreen : public Widget, public ButtonListener
    {
    private:

    public:
        CreditsScreen(bool happyEnd);
        virtual ~CreditsScreen();

        virtual void Draw(Graphics *g);
        virtual void Update();
        virtual void AddedToManager(WidgetManager *theWidgetManager);
        virtual void RemovedFromManager(WidgetManager *theWidgetManager);
        virtual void ButtonDepress(int theId);
        virtual void KeyChar(char theChar);
    };

};

#endif