#ifndef __HELPSCREEN_H__
#define __HELPSCREEN_H__

#include <SexyAppFramework/Widget.h>
#include <SexyAppFramework/HyperlinkWidget.h>
#include <SexyAppFramework/ButtonListener.h>
#include <SexyAppFramework/CheckboxListener.h>

namespace Sexy
{
    class CircleCheckbox;
    class CircleButton;

    class HelpScreen : public Widget, public ButtonListener, public CheckboxListener
    {
    private:
        Widget *mHelpWidget1;
        Widget *mHelpWidget2;
        Widget *mHelpWidget3;
        Widget *mHelpBack;
        CircleCheckbox *mShowHelpCheckbox;
        CircleButton *mOKButton;

    public:
        HelpScreen();
        virtual ~HelpScreen();

        virtual void Resize(int theX, int theY, int theWidth, int theHeight);
        virtual void Draw(Graphics *g);
        virtual void AddedToManager(WidgetManager *theWidgetManager);
        virtual void RemovedFromManager(WidgetManager *theWidgetManager);
        virtual void ButtonDepress(int theId);
        virtual void CheckboxChecked(int theId, bool checked);
        virtual void KeyChar(char theChar);
    };

};

#endif