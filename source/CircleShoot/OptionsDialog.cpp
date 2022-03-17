#include "Zuma_Prefix.pch"

#include <SexyAppFramework/WidgetManager.h>

#include "CircleButton.h"
#include "CircleCommon.h"
#include "OptionsDialog.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

OptionsDialog::OptionsDialog(bool inMainMenu) : CircleDialog(Sexy::IMAGE_DIALOG_BACK, Sexy::IMAGE_DIALOG_BUTTON, 0, true,
                                                             inMainMenu ? _S("MENU") : _S("OPTIONS"), _S(""),
                                                             inMainMenu ? _S("Back To Game") : _S("Done"),
                                                             3, false)
{
    button = MakeButton(1, this, _S("This is pretty fun"), 1, 0, 3);
    mExtraHeight = 200;
}

OptionsDialog::~OptionsDialog()
{
}

void OptionsDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
    Dialog::Resize(theX, theY, theWidth, theHeight);

    int left = GetLeft();
    int top = GetTop();
    int width = GetWidth();

    button->Resize(left, top, 200, button->mHeight);
}

int OptionsDialog::GetPreferredHeight(int theWidth)
{
    return mExtraHeight + CircleDialog::GetPreferredHeight(theWidth);
}

void OptionsDialog::AddedToManager(WidgetManager *theWidgetManager)
{
    Dialog::AddedToManager(theWidgetManager);

    theWidgetManager->AddWidget(button);
}

void OptionsDialog::RemovedFromManager(WidgetManager *theWidgetManager)
{
    Dialog::RemovedFromManager(theWidgetManager);

    theWidgetManager->RemoveWidget(button);
}

void OptionsDialog::Draw(Graphics *g)
{
    CircleDialog::Draw(g);
}