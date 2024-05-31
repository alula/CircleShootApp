#include "Zuma_Prefix.pch"

#include <SexyAppFramework/MTRand.h>
#include <SexyAppFramework/ButtonListener.h>
#include <SexyAppFramework/Dialog.h>
#include <SexyAppFramework/DialogButton.h>
#include <SexyAppFramework/Debug.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/Graphics.h>
#include <SexyAppFramework/MemoryImage.h>
#include <SexyAppFramework/SexyAppBase.h>

#include "Board.h"
#include "CircleShootApp.h"
#include "CircleCommon.h"
#include "CircleButton.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

MTRand Sexy::gThreadRand;
MTRand Sexy::gAppRand;

int Sexy::gButtonSequenceCount = 0;
int Sexy::gLastButtonOver = 4;
uint Sexy::gBallColors[MAX_BALL_COLORS] = {0x1980FF, 0xFFFF00, 0xFF0000, 0x00FF00, 0xFF00FF, 0xFFFFFF};
uint Sexy::gBrightBallColors[MAX_BALL_COLORS] = {0x80FFFF, 0xFFFF40, 0xFFAAAA, 0x80FF80, 0xFF80FF, 0xFFFFFF};
uint Sexy::gDarkBallColors[MAX_BALL_COLORS] = {0x231679, 0x60510A, 0xA00F14, 0x204422, 0x561643, 0x381B22};
uint Sexy::gTextBallColors[MAX_BALL_COLORS] = {0x2D8BFF, 0xFFFF00, 0xFF0000, 0xFF00, 0xFF00FF, 0xFFFFFF};
const char *Sexy::gGauntletStages[] = {
    "RABBIT",
    "EAGLE",
    "JAGUAR",
    "SUNGOD",
};
const char *Sexy::gSmallGauntletStages[] = {
    "RAB",
    "EAG",
    "JAG",
    "SG",
};
int Sexy::gSaveGameVersion = 5;

int Sexy::gMainThreadId = 0;

std::string Sexy::GetTimeString(int aTime)
{
    if (aTime / 60 / 60 > 0)
    {
        return Sexy::StrFormat("%2d:%02d:%02d", aTime / 60 / 60, (aTime / 60) % 60, aTime % 60);
    }
    else
    {
        return Sexy::StrFormat("%d:%02d", aTime / 60, aTime % 60);
    }
}

int Sexy::AppGetTickCount()
{
    return 10 * gSexyAppBase->mUpdateCount;
}

int Sexy::GetBoardStateCount()
{
    Board *board = ((CircleShootApp *)gSexyAppBase)->GetBoard();
    if (board == NULL)
    {
        return 0;
    }

    return board->GetStateCount();
}

int Sexy::AppRand()
{
#ifndef _WIN32
    // DBG_ASSERT(pthread_self() == gMainThreadId); // this is there in Mac version
#endif
    return gAppRand.Next();
}

int Sexy::ThreadRand()
{
#ifdef _WIN32
    bool isCurrent = GetCurrentThreadId() == gSexyAppBase->mPrimaryThreadId;
#else
    bool isCurrent = pthread_self() == gMainThreadId;
#endif

    if (isCurrent)
    {
        return gAppRand.Next();
    }
    else
    {
        return gThreadRand.NextNoAssert();
    }
}

void Sexy::MirrorPoint(float &x, float &y, MirrorType theMirror)
{
    switch (theMirror)
    {
    case MirrorType_None:
        break;
    case MirrorType_X:
        x = 640.0f - x;
        break;
    case MirrorType_Y:
        y = 480.0f - y;
        break;
    case MirrorType_XY:
        x = 640.0f - x;
        y = 480.0f - y;
        break;
    }
}

void Sexy::MirrorPoint(int &x, int &y, MirrorType theMirror)
{
    switch (theMirror)
    {
    case MirrorType_None:
        break;
    case MirrorType_X:
        x = 640 - x;
        break;
    case MirrorType_Y:
        y = 480 - y;
        break;
    case MirrorType_XY:
        x = 640 - x;
        y = 480 - y;
        break;
    }
}

void Sexy::MirrorRect(int &x, int &y, int w, int h, MirrorType theMirror)
{
    switch (theMirror)
    {
    case MirrorType_None:
        break;
    case MirrorType_X:
        x = 640 - (x + w);
        break;
    case MirrorType_Y:
        y = 480 - (y + h);
        break;
    case MirrorType_XY:
        x = 640 - (x + w);
        y = 480 - (y + h);
        break;
    }
}

void Sexy::ImageMirror(MemoryImage *theImage, MirrorType theMirror)
{
    switch (theMirror)
    {
    case MirrorType_None:
        break;
    case MirrorType_X:
        gSexyAppBase->MirrorImage(theImage);
        break;
    case MirrorType_Y:
        gSexyAppBase->FlipImage(theImage);
        break;
    case MirrorType_XY:
        gSexyAppBase->MirrorImage(theImage);
        gSexyAppBase->FlipImage(theImage);
        break;
    }
}

void Sexy::SetupButton(DialogButton *theButton, int numCols)
{
    if (theButton)
    {
        theButton->mTranslateX = -1;
        theButton->mTranslateY = 1;

        int width = theButton->mComponentImage->mWidth / numCols;
        int height = theButton->mComponentImage->mHeight;

        theButton->mNormalRect.mHeight = height;
        theButton->mNormalRect.mWidth = width;
        theButton->mNormalRect.mY = 0;
        theButton->mNormalRect.mX = 0;
        theButton->mOverRect.mHeight = height;
        theButton->mOverRect.mWidth = width;
        theButton->mOverRect.mY = 0;
        theButton->mOverRect.mX = width;
        theButton->mDownRect.mHeight = height;
        theButton->mDownRect.mWidth = width;
        theButton->mDownRect.mY = 0;
        theButton->mDownRect.mX = 2 * width;

        theButton->mTextOffsetX = 4;
        theButton->mTextOffsetY = -8;

        theButton->SetFont(Sexy::FONT_DIALOG); // TODO
        theButton->SetColor(0, Sexy::Color(0xD5E520));
        theButton->SetColor(1, Sexy::Color(0xFFFFFF));

        theButton->mHasAlpha = true;
        theButton->mHasTransparencies = true;

        if (theButton->mWidth == 0)
        {
            theButton->Resize(theButton->mX,
                              theButton->mY,
                              theButton->mFont->StringWidth(theButton->mLabel),
                              theButton->mComponentImage->mHeight);
        }
    }
}

CircleButton *Sexy::MakeButton(int id, ButtonListener *theListener, std::string const &label, int flags, Sexy::Image *image, int numCols)
{
    bool customImage;

    if (image)
    {
        customImage = true;
    }
    else
    {
        image = Sexy::IMAGE_DIALOG_BUTTON;
        customImage = false;
    }

    CircleButton *theButton = new CircleButton(image, id, theListener);
    theButton->mLabel = label;

    if ((flags & 1) != 0)
        theButton->mClickSound = Sexy::SOUND_BUTTON1;

    if ((flags & 2) != 0)
    {
        int pitchTable[5];
        pitchTable[0] = 2;
        pitchTable[1] = 4;
        pitchTable[2] = 0;
        pitchTable[3] = -12;
        pitchTable[4] = -5;

        double pitch = pitchTable[id % 5] + 10;
        theButton->mHoverSound = Sexy::GetSoundById(Sexy::SOUND_CHORAL1_ID);
        theButton->mPitch = pitch;
    }

    if (customImage)
    {
        theButton->mWidth = image->mWidth / numCols;
        theButton->mHeight = image->mHeight;
    }

    SetupButton(theButton, numCols);

    if (customImage)
    {
        theButton->mButtonImage = image;
        theButton->mComponentImage = NULL;
    }

    return theButton;
}

void Sexy::SetupDialog(Dialog *theDialog, int theMinWidth)
{
    theDialog->SetHeaderFont(Sexy::FONT_TITLE);
    theDialog->SetLinesFont(Sexy::FONT_DIALOG);
    theDialog->SetColor(0, Sexy::Color(203, 201, 187));
    theDialog->SetColor(1, Sexy::Color(0xD5E520));

    int aWinWidth = theDialog->mBackgroundInsets.mRight + theDialog->mBackgroundInsets.mLeft + theDialog->mContentInsets.mRight + theDialog->mContentInsets.mLeft + Sexy::FONT_TITLE->StringWidth(theDialog->mDialogHeader) + 20;

    if (aWinWidth <= theMinWidth)
        aWinWidth = theMinWidth;

    Sexy::Dialog *topDialog;
    if (!gSexyAppBase->mDialogMap.empty())
    {
        topDialog = gSexyAppBase->mDialogMap.begin()->second;
    }
    else
    {
        topDialog = NULL;
    }

    int winY = 90 - theDialog->mBackgroundInsets.mTop;
    if (topDialog != NULL)
    {
        winY += topDialog->mY + 20;
    }

    int winHeight = theDialog->GetPreferredHeight(aWinWidth);
    int screenWidth = 640;
    if (winY + winHeight >= 471)
    {
        winY = 10;
    }

    theDialog->Resize((screenWidth - aWinWidth) / 2, winY, aWinWidth, winHeight);

    SetupButton(theDialog->mYesButton, 3);
    SetupButton(theDialog->mNoButton, 3);
}

MemoryImage *Sexy::CutoutImageFromAlpha(MemoryImage *theBackgroundImage, MemoryImage *theAlpha, int x, int y)
{
    Rect a1 = Rect(x, y, theAlpha->GetWidth(), theAlpha->GetHeight());
    Rect a2 = Rect(0, 0, theBackgroundImage->GetWidth(), theBackgroundImage->GetHeight());
    Rect a3 = a1.Intersection(a2);

    if (a1.mWidth <= 0 || a1.mHeight <= 0)
    {
        return NULL;
    }

    MemoryImage *theImage = new MemoryImage(gSexyAppBase);
    theImage->Create(a1.mWidth, a1.mHeight);

    Graphics g(theImage);
    g.DrawImage(theBackgroundImage, -a1.mX, -a1.mY);

    int size = a1.mWidth * a1.mHeight;
    uchar *bitsAlpha = reinterpret_cast<uchar *>(theAlpha->GetBits());
    uchar *bitsImg = reinterpret_cast<uchar *>(theImage->GetBits());

    for (int i = 0; i < size; ++i)
    {
        bitsImg[4 * i + 3] = bitsAlpha[4 * i + 3];
    }

    return theImage;
}

void Sexy::FillCircle(Graphics *g, int x, int y, int theRadius)
{
    Span aSpans[8192];

    int v4 = theRadius;
    Image *anImage = g->mDestImage;
    int clipRectLeft = g->mClipRect.mX;
    int clipRectTop = g->mClipRect.mY;
    int clipRectRight = g->mClipRect.mWidth + clipRectLeft;
    int clipRectBottom = g->mClipRect.mHeight + clipRectTop;
    int v26 = 5 - 4 * theRadius;
    int v25 = 0;
    int spanCount = 0;
    Span *spanItr = &aSpans[0];

    while (v4 > v25)
    {
        int v5 = y - v25;
        int v6 = v4 + x;
        int v7 = x - v4;

        if (clipRectTop <= y - v25 && clipRectBottom > v5)
        {
            int v8 = clipRectLeft;
            if (clipRectLeft <= v7)
                v8 = x - v4;

            int v9 = clipRectRight;
            if (clipRectRight >= v6)
                v9 = x + v4;

            if (v8 < v9)
            {
                ++spanCount;
                spanItr->mX = v8;
                spanItr->mY = v5;
                spanItr->mWidth = v9 - v8;
                ++spanItr;
            }
        }

        if (v25 != 0)
        {
            if (clipRectTop <= y + v25 && clipRectBottom > y + v25)
            {
                if (clipRectLeft > v7)
                    v7 = clipRectLeft;
                if (clipRectRight < v6)
                    v6 = clipRectRight;
                if (v7 < v6)
                {
                    ++spanCount;
                    spanItr->mX = v7;
                    spanItr->mY = y + v25;
                    spanItr->mWidth = v6 - v7;
                    ++spanItr;
                }
            }
        }

        if (v26 <= 0)
        {
            v26 += 8 * v25++ + 4;
        }
        else
        {
            int v12 = x + v25;
            int v13 = x - v25;
            if (clipRectTop <= y - v4 && clipRectBottom > y - v4)
            {
                int v14 = clipRectLeft;
                if (clipRectLeft <= v13)
                    v14 = x - v25;
                int v15 = clipRectRight;
                if (clipRectRight >= v12)
                    v15 = x + v25;
                if (v14 < v15)
                {
                    ++spanCount;
                    spanItr->mX = v14;
                    spanItr->mY = y - v4;
                    spanItr->mWidth = v15 - v14;
                    ++spanItr;
                }
            }

            if (v4 != 0)
            {
                if (clipRectTop <= v4 + y && clipRectBottom > v4 + y)
                {
                    if (clipRectLeft > v13)
                        v13 = clipRectLeft;
                    if (clipRectRight < v12)
                        v12 = clipRectRight;
                    if (v13 < v12)
                    {
                        ++spanCount;
                        spanItr->mX = v13;
                        spanItr->mY = v4 + y;
                        spanItr->mWidth = v12 - v13;
                        ++spanItr;
                    }
                }
            }
            v26 += 8 * (v25 - v4--) + 4;
            ++v25;
        }
    }

    int aDrawMode = g->GetDrawMode();
    const Color &aColor = g->GetColor();
    anImage->FillScanLines(aSpans, spanCount, aColor, aDrawMode);
}

#ifdef CIRCLE_ENDIAN_SWAP_ENABLED
unsigned int ByteSwap(unsigned int theValue)
{
    return (((theValue >> 24) & 0xFF) | ((theValue >> 8) & 0xFF00) | ((theValue << 8) & 0xFF0000) | ((theValue << 24) & 0xFF000000));
}
#endif

int Sexy::BoardGetTickCount()
{
    int aTickCount;

    Board *aBoard = ((CircleShootApp *)gSexyAppBase)->mBoard;
    if (aBoard)
        aTickCount = 10 * aBoard->GetStateCount();

    return aTickCount;
}