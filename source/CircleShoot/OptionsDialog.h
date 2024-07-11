#ifndef __OPTIONSDIALOG_H__
#define __OPTIONSDIALOG_H__

#include "CircleDialog.h"
#include <SexyAppFramework/SliderListener.h>
#include <SexyAppFramework/CheckboxListener.h>

namespace Sexy
{
    class WidgetManager;
    class CircleButton;
    class Graphics;
    class Slider;
    class Checkbox;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class OptionsDialog : public CircleDialog, SliderListener, CheckboxListener
    {
    public:
        OptionsDialog(bool inMainMenu);
        virtual ~OptionsDialog();

        virtual void Resize(int theX, int theY, int theWidth, int theHeight);
        virtual int GetPreferredHeight(int theWidth);

        virtual void AddedToManager(WidgetManager *theWidgetManager);
        virtual void RemovedFromManager(WidgetManager *theWidgetManager);

        virtual void ButtonPress(int theId);
        virtual void ButtonDepress(int theId);
        virtual void SliderVal(int theId, double theVal);
        virtual void CheckboxChecked(int theId, bool checked);

        virtual void Draw(Graphics *g);

        Slider *mMusicVolumeSlider;
        Slider *mSfxVolumeSlider;
        Checkbox *mFullScreenCheckbox;
        Checkbox *mCustomCursorsCheckbox;
        Checkbox *m3DAccelCheckbox;
        CircleButton *mButtonHelp;
        CircleButton *mButtonRegister;
        CircleButton *mButtonUpdates;
        CircleButton *mButtonBack;
        bool mInMainMenu;
        int mExtraHeight;
    };
};

#endif