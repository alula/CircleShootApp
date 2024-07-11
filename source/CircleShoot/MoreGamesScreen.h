#ifndef __MOREGAMESSCREEN_H__
#define __MOREGAMESSCREEN_H__

#include <SexyAppFramework/Widget.h>
#include <SexyAppFramework/HyperlinkWidget.h>
#include <SexyAppFramework/ButtonListener.h>
#include <SexyAppFramework/CheckboxListener.h>

namespace Sexy
{
    class CircleCheckbox;
    class CircleButton;

    class MoreGamesScreen : public Widget, public ButtonListener, public CheckboxListener
    {
    private:

    public:
        MoreGamesScreen();
        virtual ~MoreGamesScreen();

        // virtual void Resize(int theX, int theY, int theWidth, int theHeight);
        // virtual void Draw(Graphics *g);
        // virtual void Update();
        // virtual void AddedToManager(WidgetManager *theWidgetManager);
        // virtual void RemovedFromManager(WidgetManager *theWidgetManager);
        // virtual void ButtonDepress(int theId);
    };

};

#endif