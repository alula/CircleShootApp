#ifndef __CIRCLEBUTTON_H__
#define __CIRCLEBUTTON_H__

#include <SexyAppFramework/DialogButton.h>

namespace Sexy
{

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    class CircleButton : public Sexy::DialogButton
    {
    public:
        enum
        {
            CB_ClickSound = 1,
            CB_ChoralSound = 2,
        };

        int mClickSound;
        int mHoverSound;
        double mPitch;
        int unkF8;
        bool unkFC;
        bool unkFD;

        CircleButton(Image *theComponentImage, int theId, ButtonListener *theListener);
        virtual ~CircleButton();

        virtual bool IsPointVisible(int x, int y);
        virtual void MouseEnter();
        virtual void MouseDown(int theX, int theY, int theClickCount);
        virtual void Update();
        virtual void Draw(Graphics *g);
    };
}

#endif