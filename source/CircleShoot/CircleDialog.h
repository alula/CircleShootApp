#ifndef __CIRCLEDIALOG_H__
#define __CIRCLEDIALOG_H__

#include <SexyAppFramework/Dialog.h>
#include <SexyAppFramework/Graphics.h>

namespace Sexy
{

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    class CircleDialog : public Dialog
    {
    public:
        bool mHasGodHead;

        CircleDialog(Image *theComponentImage, Image *theButtonComponentImage,
                     int theId, bool isModal, const SexyString &theDialogHeader,
                     const SexyString &theDialogLines, const SexyString &theDialogFooter,
                     int theButtonMode, bool hasGodHead);
        virtual ~CircleDialog();

        int GetLeft();
        int GetTop();
        int GetWidth();

        virtual void Draw(Graphics *g);
        virtual void MouseDown(int x, int y, int theClickCount);
        virtual int GetPreferredHeight(int theWidth);
        virtual void ButtonPress(int theId);
    };
}

#endif