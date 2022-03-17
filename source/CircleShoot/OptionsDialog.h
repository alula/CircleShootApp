#ifndef __OPTIONSDIALOG_H__
#define __OPTIONSDIALOG_H__

#include "CircleDialog.h"

namespace Sexy
{
    class WidgetManager;
    class CircleButton;
    class Graphics;

    class OptionsDialog : public CircleDialog
    {
        public:
        OptionsDialog(bool inMainMenu);
        virtual ~OptionsDialog();

        virtual void Resize(int theX, int theY, int theWidth, int theHeight);
        virtual int GetPreferredHeight(int theWidth);

        virtual void AddedToManager(WidgetManager *theWidgetManager);
        virtual void RemovedFromManager(WidgetManager *theWidgetManager);

        virtual void Draw(Graphics *g);

        CircleButton* button;
        int mExtraHeight;
    };
};

#endif