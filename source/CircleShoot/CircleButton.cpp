#include "Zuma_Prefix.pch"

#include "CircleButton.h"
#include "CircleCommon.h"

#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/MemoryImage.h>
#include <SexyAppFramework/SoundManager.h>
#include <SexyAppFramework/SoundInstance.h>

using namespace Sexy;

CircleButton::CircleButton(Image *theComponentImage, int theId, ButtonListener *theListener) : DialogButton(theComponentImage, theId, theListener)
{
    unkF8 = -1000;
    mClickSound = -1;
    mHoverSound = -1;
    unkFC = true;
    unkFD = false;
}

CircleButton::~CircleButton()
{
}

bool CircleButton::IsPointVisible(int x, int y)
{
    if (!mButtonImage)
        return true;

    if (x < 0 || y < 0 || x >= mButtonImage->mWidth, y >= mButtonImage->mHeight)
        return false;

    return (*(reinterpret_cast<MemoryImage *>(mButtonImage)->GetBits() + (y * mButtonImage->mWidth + x)) & 0xFF000000) != 0;
}

void CircleButton::MouseEnter()
{
    ButtonWidget::MouseEnter();

    if (mHoverSound >= 0)
    {
        SoundInstance *inst = gSexyAppBase->mSoundManager->GetSoundInstance(mHoverSound);
        if (inst)
        {
            inst->AdjustPitch(mPitch);
            inst->Play(false, true);

            if (mId == (gLastButtonOver + 1) % 5)
            {
                gLastButtonOver = mId;

                if (gLastButtonOver == 4)
                    ++gButtonSequenceCount;
            }
            else
            {
                gLastButtonOver = 4;
                gButtonSequenceCount = 0;
            }
        }

        unkF8 = gSexyAppBase->mUpdateCount;
    }
}

void CircleButton::MouseDown(int theX, int theY, int theClickCount)
{
    ButtonWidget::MouseDown(theX, theY, theClickCount);

    if (mClickSound >= 0)
    {
        gSexyAppBase->PlaySample(mClickSound);
    }
}

void CircleButton::Update()
{
    ButtonWidget::Update();

    if (unkFD)
    {
        MarkDirty();
    }
}

void CircleButton::Draw(Graphics *g)
{
    if (unkFC)
    {
        DialogButton::Draw(g);

        if (unkFD && !mIsOver)
        {
            mIsOver = true;
            int v2 = mUpdateCnt % 100;
            if (v2 > 50)
                v2 = 100 - v2;

            g->SetColor(Sexy::Color(255, 255, 255, 255 * v2 / 50));
            g->SetColorizeImages(true);
            DialogButton::Draw(g);
            g->SetColorizeImages(false);
            mIsOver = false;
        }
    }
}