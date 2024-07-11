#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SexyAppBase.h>

#include "CircleDialog.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CircleDialog::CircleDialog(Image *theComponentImage, Image *theButtonComponentImage,
                           int theId, bool isModal, const SexyString &theDialogHeader,
                           const SexyString &theDialogLines, const SexyString &theDialogFooter,
                           int theButtonMode, bool hasGodHead)
    : Dialog(theComponentImage, theButtonComponentImage, theId, isModal, theDialogHeader, theDialogLines, theDialogFooter, theButtonMode)
{
    mHasGodHead = hasGodHead;
    if (hasGodHead)
    {
        mBackgroundInsets = Insets(0, 80, 0, 0);
        mContentInsets = Insets(36, 52, 36, 36);
    }
    else
    {
        mContentInsets = Insets(36, 28, 36, 36);
    }
}

CircleDialog::~CircleDialog()
{
}

int CircleDialog::GetLeft()
{
    return mBackgroundInsets.mLeft + mContentInsets.mLeft + mX;
}

int CircleDialog::GetTop()
{
    return mBackgroundInsets.mTop + mContentInsets.mTop + mY + 54;
}

int CircleDialog::GetWidth()
{
    return mWidth - mContentInsets.mLeft - mContentInsets.mRight - mBackgroundInsets.mLeft - mBackgroundInsets.mRight;
}

void CircleDialog::Draw(Graphics *g)
{
    Dialog::Draw(g);

    if (mHasGodHead)
    {
        g->DrawImage(Sexy::IMAGE_GOD_HEAD, (mWidth - Sexy::IMAGE_GOD_HEAD->mWidth) / 2, 0);
    }
}

void CircleDialog::MouseDown(int x, int y, int theClickCount)
{
    int v4; // ecx
    int v5; // eax

    if (!this->mHasGodHead || (v4 = Sexy::IMAGE_GOD_HEAD->mWidth, v5 = (mWidth - v4) / 2, y >= mBackgroundInsets.mTop) || x >= v5 && x <= v4 + v5)
    {
        Dialog::MouseDown(x, y, theClickCount);
    }
}

int CircleDialog::GetPreferredHeight(int theWidth)
{
    return Dialog::GetPreferredHeight(theWidth);
}

void CircleDialog::ButtonPress(int theId)
{
    if (theId > 1999)
    {
        gSexyAppBase->PlaySample(Sexy::SOUND_BUTTON1);
    }
}