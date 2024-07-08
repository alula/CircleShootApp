#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/ButtonWidget.h>
#include <SexyAppFramework/Common.h>
#include <SexyAppFramework/Graphics.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/Image.h>
#include <SexyAppFramework/MemoryImage.h>
#include <SexyAppFramework/WidgetManager.h>

#include "TransitionMgr.h"
#include "SpriteMgr.h"
#include "ParticleMgr.h"
#include "SoundMgr.h"
#include "CurveMgr.h"
#include "DataSync.h"
#include "CircleShootApp.h"
#include "ProfileMgr.h"
#include "Gun.h"
#include "Board.h"
#include "Ball.h"
#include "Bullet.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void FrogMove::SyncState(DataSync &theSync)
{
    theSync.SyncShort(mUpdateCnt);
    theSync.SyncShort(mDuration);
    theSync.SyncSShort(mStartPos.mX);
    theSync.SyncSShort(mStartPos.mY);
    theSync.SyncSShort(mEndPos.mX);
    theSync.SyncSShort(mEndPos.mY);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void FrogScale::SyncState(DataSync &theSync)
{
    theSync.SyncSShort(mUpdateCnt);
    theSync.SyncShort(mDuration);
    theSync.SyncFloat(mStartScale);
    theSync.SyncFloat(mEndScale);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TextBlurb::SyncState(DataSync &theSync)
{
    theSync.SyncSShort(mCharNum);
    theSync.SyncShort(mDelayCnt);
    theSync.SyncSShort(mPos.mX);
    theSync.SyncSShort(mPos.mY);
    theSync.SyncString(mText);
    theSync.SyncLong(mColor);
    theSync.SyncLong(mFont);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
LetterStamp::LetterStamp()
{
    mPos.mX = 0;
    mPos.mY = 0;
    mImage = NULL;
    mDuration = 0;
}

LetterStamp::~LetterStamp()
{
    if (mImage)
        delete mImage;
}

void LetterStamp::SyncState(DataSync &theSync)
{
    theSync.SyncLong(mUpdateCnt);
    theSync.SyncString(mLetter);
    theSync.SyncSShort(mPos.mX);
    theSync.SyncSShort(mPos.mY);
    theSync.SyncLong(mColor);
    theSync.SyncShort(mFont);
    theSync.SyncShort(mDuration);
}

void LetterStamp::CreateImage()
{
    if (mImage != NULL)
        return;

    Font *aFont = Sexy::GetFontById((ResourceId)mFont);
    mImage = new MemoryImage(gSexyAppBase);
    mImage->Create(aFont->StringWidth(mLetter) + 10, aFont->GetHeight() + 10);
    Graphics g(mImage);

    g.SetFont(aFont);
    g.SetColor(Color(mColor));
    g.DrawString(mLetter, 5, aFont->GetAscent());
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool gForceStageComplete = false;

TransitionMgr::TransitionMgr(Board *theBoard)
{
    mBoard = theBoard;
    mCurFrogScale = 1.0f;
    mQuakeFrame = 0;
    mState = TransitionState_None;
    mLevelFade = 0;
    mTargetLevelFade = 0;
    mLevelFadeInc = 5;
}

TransitionMgr::~TransitionMgr()
{
}

void TransitionMgr::DrawFrog(Graphics *g)
{
    if (mCurFrogScale == 1.0f)
    {
        if (!mBoard->mApp->Is3DAccelerated())
            mBoard->mGun->SetAngle(mBoard->mGun->GetCachedGunAngle());

        mBoard->mGun->Draw(g);
    }
    else if (mCurFrogScale > 0.01f)
    {
        mBoard->mGun->DrawCachedGunImage(g, mCurFrogScale);
    }
}

void TransitionMgr::DrawQuake(Graphics *g)
{
    if (mBoard->mPracticeMode != PracticeMode_Single)
    {
        Color c(0x000000);

        if (!mDoTempleUp)
        {
            int anAlpha = 255 * mStateCount / 250;
            if (anAlpha >= 10)
            {
                mBoard->mNextSpriteMgr->DrawLevel(g);

                if (anAlpha < 255)
                {
                    c = Color(0, 0, 0, 255 - anAlpha);
                }
            }
        }

        g->SetColor(c);
        g->FillRect(0, 0, 640, 480);
        mBoard->mSpriteMgr->DrawBackgroundTransition(g);
    }
    else
    {
        mBoard->mSpriteMgr->DrawLevel(g);
    }

    DrawLevelFade(g);
    if (mDoStageUp && mStateCount < 100)
    {
        DrawStageCompleteText(g);
    }

    DrawLetterStamp(g);
    mBoard->mParticleMgr->Draw(g, 0);
    DrawFrog(g);
    mBoard->mParticleMgr->Draw(g, 4);
}

void TransitionMgr::DrawDefault(Graphics *g)
{
    mBoard->mSpriteMgr->DrawLevel(g);
    DrawLevelFade(g);
    DrawLetterStamp(g);
    mBoard->mParticleMgr->Draw(g, 0);
    DrawFrog(g);
    mBoard->mParticleMgr->Draw(g, 4);
}

void TransitionMgr::DrawLetterStamp(Graphics *g)
{
    for (LetterStampList::iterator anItr = mLetterStampList.begin(); anItr != mLetterStampList.end(); anItr++)
    {
        LetterStamp &aStamp = *anItr;
        if (aStamp.mUpdateCnt < 0)
            continue;

        Font *aFont = Sexy::GetFontById((ResourceId)aStamp.mFont);

        aStamp.CreateImage();

        int aPosX = aStamp.mPos.mX - 5;
        int aPosY = aStamp.mPos.mY - aFont->GetAscent();
        int aRem = aStamp.mDuration - aStamp.mUpdateCnt;

        if (aStamp.mUpdateCnt >= 30 && (aRem < 0 || aRem >= 10))
        {
            g->DrawImage(aStamp.mImage, aPosX, aPosY);
            continue;
        }

        float aScale;
        if (aRem < 0 || aRem >= 10)
        {
            if (aStamp.mUpdateCnt >= 10)
            {
                int b = aStamp.mUpdateCnt - 10;
                int a = 20 - b;
                aScale = (float(a) * 1.1 + float(b)) / 20.0f;
            }
            else
            {
                aScale = (aStamp.mUpdateCnt * 0.6f / 10.0f) + 0.5f;
            }
        }
        else
        {
            aScale = 0.5f + (aRem / 20.0f);
        }

        if (aScale <= 0.01f)
            continue;

        int aWidth = aStamp.mImage->mWidth;
        int aHeight = aStamp.mImage->mHeight;
        int aScaledWidth = aWidth * aScale;
        int aScaledHeight = aHeight * aScale;

        Rect aSrc(0, 0, aWidth, aHeight);
        Rect aDest(aPosX + aWidth / 2 - aScaledWidth / 2,
                   aPosY + aHeight / 2 - aScaledHeight / 2,
                   aScaledWidth,
                   aScaledHeight);

        bool isFastStretch = g->GetFastStretch();
        g->SetFastStretch(!gSexyAppBase->Is3DAccelerated());
        g->DrawImage(aStamp.mImage, aDest, aSrc);
        g->SetFastStretch(isFastStretch);
    }
}

void TransitionMgr::DrawTextBlurb(Graphics *g)
{
    for (TextBlurbList::iterator anItr = mTextBlurbList.begin();
         anItr != mTextBlurbList.end();
         anItr++)
    {
        TextBlurb &aBlurb = *anItr;
        int aCharNum = aBlurb.mCharNum + 1;

        if (aCharNum > 0)
        {
            g->SetColor(Color(aBlurb.mColor));
            Font *aFont = Sexy::GetFontById((ResourceId)aBlurb.mFont);
            g->SetFont(aFont);

            if (aCharNum < aBlurb.mText.size())
            {
                g->DrawString(
                    aBlurb.mText.substr(0, aCharNum),
                    aBlurb.mPos.mX,
                    aBlurb.mPos.mY);
            }
            else
            {
                g->DrawString(
                    aBlurb.mText,
                    aBlurb.mPos.mX,
                    aBlurb.mPos.mY);
            }
        }
    }
}

void TransitionMgr::DrawStageCompleteText(Graphics *g)
{
    Graphics sg(*g);

    if (mState == TransitionState_StageComplete)
    {
        if (mStateCount < mStageCompleteFrame || mStateCount > mStageCompleteFrame + 150)
            return;

        if (mStateCount - mStageCompleteFrame < 50)
            sg.ClipRect(0, 0, mBoard->mGun->GetCenterX(), 480);
    }

    std::string aText = "COMPLETE";
    int center = Sexy::FONT_HUGE->StringWidth(aText);

    sg.SetColor(Color(0xFFFF00));
    sg.SetFont(Sexy::FONT_HUGE);
    sg.DrawString(aText, (CIRCLE_WINDOW_WIDTH / 2) - (center / 2), CIRCLE_WINDOW_HEIGHT / 2 + 10);
}

void TransitionMgr::DrawStageComplete(Graphics *g)
{
    mBoard->mSpriteMgr->DrawLevel(g);
    DrawLevelFade(g);
    DrawStageCompleteText(g);
    DrawLetterStamp(g);
    mBoard->mParticleMgr->Draw(g, 0);
    DrawFrog(g);
    mBoard->mParticleMgr->Draw(g, 4);
}

void TransitionMgr::DrawLevelBegin(Graphics *g)
{
    mBoard->mSpriteMgr->DrawBackground(g);

    for (int i = 0; i < MAX_PRIORITY; i++)
    {
        mBoard->mSpriteMgr->DrawSprites(g, i);
        mBoard->mParticleMgr->Draw(g, i);
    }

    mBoard->mGun->Draw(g);
    DrawLevelFade(g);
    DrawLetterStamp(g);
}

void TransitionMgr::DrawLevelFade(Graphics *g)
{
    if (mLevelFade == 0)
        return;

    g->SetColor(Color(0, 0, 0, mLevelFade));
    g->FillRect(0, 0, 640, 480);
}

void TransitionMgr::DrawLosing(Graphics *g)
{
    static BallDrawer aDrawer;

    mBoard->mSpriteMgr->DrawBackground(g);
    aDrawer.Reset();

    if (mBoard->mPauseCount == 0 || mBoard->mShowBallsDuringPause)
    {
        for (int i = 0; i < mBoard->mNumCurves; i++)
        {
            mBoard->mCurveMgr[i]->DrawBalls(aDrawer);
        }
    }

    aDrawer.Draw(g, mBoard->mSpriteMgr, mBoard->mParticleMgr);
    mBoard->DrawBullets(g);

    if (mResetFrame == 0 || mResetFrame - mStateCount > 200)
    {
        mBoard->mGun->SetAngle(((mStateCount * mStateCount) / 2000.0f) + mCurFrogAngle);
        mBoard->mGun->Draw(g);

        if (mStateCount > 50)
        {
            int anAlpha = (255 * (mStateCount - 50)) / 200;
            if (anAlpha > 0)
            {
                g->SetColorizeImages(true);
                g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
                if (anAlpha > 255)
                    anAlpha = 255;

                g->SetColor(Color(255, 255, 255, anAlpha));
                mBoard->mGun->Draw(g);
                g->SetColorizeImages(false);
                g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
            }
        }
    }

    DrawLevelFade(g);
    mBoard->mParticleMgr->Draw(g, 4);
    DrawLetterStamp(g);
}

void TransitionMgr::DrawTempleComplete(Graphics *g)
{
    g->SetColor(Color(0, 0, 50));
    g->FillRect(0, 0, CIRCLE_WINDOW_WIDTH, CIRCLE_WINDOW_HEIGHT);

    g->SetColor(Color(0xFFFF00));
    g->SetFont(Sexy::FONT_HUGE);

    mBoard->WriteCenteredLine(g, 120, "CONGRATULATIONS!");

    g->DrawImage(Sexy::IMAGE_GOD_HEAD,
                 (cos(((mStateCount * SEXY_PI) / 180.0f) * 2) * 5.0f) + 50.0f,
                 (sin(((mStateCount * SEXY_PI) / 180.0f) * 2) * 5.0f) + 160.0f);

    if (mStateCount > 0)
    {
        mBoard->mGun->SetAngle(SEXY_PI * 2.0f / 3.0f);
        mBoard->mGun->Draw(g);
    }

    DrawTextBlurb(g);
}

void TransitionMgr::Draw(Graphics *g)
{
    switch (mState)
    {
    case TransitionState_LevelBegin:
        DrawLevelBegin(g);
        break;
    case TransitionState_StageComplete:
        DrawStageComplete(g);
        break;
    case TransitionState_TempleComplete:
        DrawTempleComplete(g);
        break;
    case TransitionState_Quake:
        DrawQuake(g);
        break;
    case TransitionState_Losing:
        DrawLosing(g);
        break;
    default:
        DrawDefault(g);
        break;
    }
}

void TransitionMgr::Update()
{
    mStateCount++;

    Bullet *aFired = mBoard->mGun->GetFiredBullet();
    if (aFired != NULL)
    {
        delete aFired;
        mBoard->mGun->EmptyBullets();
    }

    if (mLevelFade != mTargetLevelFade)
    {
        if (mLevelFade >= mTargetLevelFade)
        {
            mLevelFade -= mLevelFadeInc;
            if (mLevelFade < mTargetLevelFade)
                mLevelFade = mTargetLevelFade;
        }
        else
        {
            mLevelFade += mLevelFadeInc;
            if (mLevelFade > mTargetLevelFade)
                mLevelFade = mTargetLevelFade;
        }
    }

    UpdateFrogMove();
    UpdateFrogScale();
    UpdateLetterStamp();
    UpdateTextBlurb();

    switch (mState)
    {
    case TransitionState_LevelBegin:
        UpdateLevelBegin();
        break;
    case TransitionState_Bonus:
        UpdateBonus();
        break;
    case TransitionState_StageComplete:
        UpdateStageComplete();
        break;
    case TransitionState_TempleComplete:
        UpdateTempleComplete();
        break;
    case TransitionState_Quake:
        UpdateQuake();
        break;
    case TransitionState_Losing:
        UpdateLosing();
        break;
    }
}

void TransitionMgr::Clear()
{
    mFrogScaleList.clear();
    mFrogMoveList.clear();
    mLetterStampList.clear();
    mTextBlurbList.clear();

    mCurFrogAngle = 1.0f;
    mLevelFade = 0;
    mTargetLevelFade = 0;
    mLevelFadeInc = 5;
    mResetFrame = 0;
    mStateCount = 0;
    mCurFrogAngle = 0.0f;
    mState = TransitionState_None;
}

void TransitionMgr::SyncState(DataSync &theSync)
{
    theSync.SyncLong(mStateCount);
    theSync.SyncLong(mQuakeFrame);
    theSync.SyncFloat(mCurFrogScale);
    theSync.SyncFloat(mCurFrogAngle);
    theSync.SyncLong(mStageCompleteFrame);
    theSync.SyncBool(mDoStageUp);
    theSync.SyncBool(mDoTempleUp);
    theSync.SyncByte(mTargetLevelFade);
    theSync.SyncByte(mLevelFade);
    theSync.SyncByte(mLevelFadeInc);
    theSync.SyncLong(mResetFrame);

    if (theSync.mReader)
    {
        theSync.SyncBytes(&mState, 4);
#ifdef CIRCLE_ENDIAN_SWAP_ENABLED
        mState = (TransitionState)ByteSwap(mState);
#endif
    }
    else
    {
#ifdef CIRCLE_ENDIAN_SWAP_ENABLED
        mState = (TransitionState)ByteSwap(mState);
#endif
        theSync.SyncBytes(&mState, 4);
    }

    DataSync_SyncSTLContainer(theSync, mFrogScaleList);
    DataSync_SyncSTLContainer(theSync, mFrogMoveList);
    DataSync_SyncSTLContainer(theSync, mLetterStampList);
    DataSync_SyncSTLContainer(theSync, mTextBlurbList);
}

void TransitionMgr::AddFrogMove(const IntPoint &theStartPos, const IntPoint &theEndPos, int theDuration, int theStagger)
{
    mFrogMoveList.push_back(FrogMove());

    FrogMove &aMove = mFrogMoveList.back();
    aMove.mUpdateCnt = -theStagger;
    aMove.mStartPos = theStartPos;
    aMove.mEndPos = theEndPos;
    aMove.mDuration = theDuration;
}

void TransitionMgr::AddFrogScale(float theStartScale, float theEndScale, int theDuration, int theStagger)
{
    mFrogScaleList.push_back(FrogScale());

    FrogScale &aScale = mFrogScaleList.back();
    aScale.mUpdateCnt = -theStagger;
    aScale.mStartScale = theStartScale;
    aScale.mEndScale = theEndScale;
    aScale.mDuration = theDuration;
}

void TransitionMgr::AddLetterStamp(char theChar, int theX, int theY, DWORD theColor, int theStagger)
{
    std::string aStr(1, theChar);

    AddLetterStamp(aStr, theX, theY, theColor, theStagger);
}

void TransitionMgr::AddLetterStamp(const std::string &theStr, int theX, int theY, DWORD theColor, int theStagger, int theFontId, int theDuration)
{
    mLetterStampList.push_back(LetterStamp());
    LetterStamp &aStamp = mLetterStampList.back();

    aStamp.mUpdateCnt = -theStagger;
    aStamp.mLetter = theStr;
    aStamp.mPos.mX = theX;
    aStamp.mPos.mY = theY;
    aStamp.mColor = theColor;

    if (theFontId == -1)
        theFontId = Sexy::FONT_HUGE_ID;

    aStamp.mFont = theFontId;
    aStamp.mDuration = theDuration;
}

int TransitionMgr::AddTextBlurb(const std::string &theBlurb, int theX, int theY, int theStagger)
{
    mTextBlurbList.push_back(TextBlurb());
    TextBlurb &aBlurb = mTextBlurbList.back();
    int aStagger = -theStagger;
    if (theStagger > 0)
        aStagger = 3 - theStagger;
    aBlurb.mCharNum = aStagger / 4;
    aBlurb.mDelayCnt = 0;
    aBlurb.mColor = 0xFFFF00;
    aBlurb.mFont = Sexy::FONT_DIALOG_ID;
    aBlurb.mPos.mX = theX;
    aBlurb.mPos.mY = theY;
    aBlurb.mText = theBlurb;

    int aDuration = -4 * aBlurb.mCharNum;
    int aSize = int(aBlurb.mText.size());
    int i = 0;
    while (i < aSize)
    {
        char aChar = aBlurb.mText[i++];
        char aNextChar = (i < aSize - 1) ? aBlurb.mText[i] : 0;

        if (aChar == '.' || aChar == '?' || aChar == '!')
        {
            aDuration += 16;
        }
        else if (aNextChar == '.' || aNextChar == '?' || aNextChar == '!')
        {
            aDuration += 16;
        }
        else
        {
            aDuration += 4;
        }
    }

    return aDuration;
}

int TransitionMgr::AddTextBlurbWrap(const std::string &theBlurb, int theX, int theY, int theMaxWidth, int theStagger)
{
    Font *aFont = Sexy::GetFontById(Sexy::FONT_DIALOG_ID);
    std::string aBlurb(theBlurb);

    char pc = 0;
    int aLineLength = 0;
    int i = 0;
    int aWordEnd = 0;
    while (i < (int)aBlurb.size())
    {
        char c = aBlurb[i];
        if (c == ' ')
            aWordEnd = i;

        aLineLength += aFont->CharWidthKern(c, pc);
        if (aLineLength > theMaxWidth)
        {
            theStagger = AddTextBlurb(aBlurb.substr(0, aWordEnd), theX, theY, theStagger);
            theY += aFont->GetLineSpacing();
            aBlurb = aBlurb.substr(aWordEnd + 1);
            pc = 0;
            aLineLength = 0;
            aWordEnd = 0;
            i = 0;
            continue;
        }

        i++;
        pc = c;
    }

    int aDuration = AddTextBlurb(aBlurb, theX, theY, theStagger);
    return aDuration;
}

void TransitionMgr::DoLevelBegin(bool firstTime)
{

    int aStagger = !firstTime ? 20 : 150;
    if (mBoard->mSpriteMgr->GetInSpace())
    {
        aStagger = 100;
    }
    else
    {
        for (int i = 0; i < mBoard->mNumCurves; ++i)
        {
            aStagger = mBoard->mCurveMgr[i]->DrawPathSparkles(0, aStagger, 1);
        }
    }

    if (firstTime)
    {
        mBoard->mSoundMgr->AddSound(SOUND_GAME_START, 1);
    }

    mResetFrame = aStagger >= 300 ? aStagger : 300;
    mState = TransitionState_LevelBegin;
    mStateCount = 0;

    int aVerbWidth = Sexy::FONT_HUGE->StringWidth(mBoard->mVerboseLevelString);
    int aTextLeft = mBoard->mWidth - aVerbWidth - 50;
    int aTextTop = mBoard->mHeight - 50;

    AddLetterStamp(mBoard->mVerboseLevelString,
                   aTextLeft, aTextTop,
                   0xFFFFFF, 20,
                   Sexy::FONT_HUGE_ID, mResetFrame - 40);

    Font *aFont = Sexy::GetFontById(Sexy::FONT_BROWNTITLE_ID);
    int anAscent = aFont->GetAscent();
    int anAscentPadding = aFont->GetAscentPadding();
    int aSmallWidth = aFont->StringWidth(mBoard->mLevelDesc->mDisplayName);

    AddLetterStamp(mBoard->mLevelDesc->mDisplayName,
                   aTextLeft + aVerbWidth - aSmallWidth,
                   aTextTop + anAscent - anAscentPadding + 4,
                   0xFFFFFF, 60,
                   Sexy::FONT_BROWNTITLE_ID, mResetFrame - 90);
}

void TransitionMgr::DoQuake()
{
    mBoard->WaitForLoadingThread();
    if (mBoard->mPracticeMode != PracticeMode_Single)
    {
        mBoard->mApp->PlaySample(Sexy::SOUND_EARTHQUAKE);
    }

    IntPoint m1(mBoard->mGun->GetCenterX(), mBoard->mGun->GetCenterY());
    IntPoint m3(mBoard->mNextLevelDesc->mGunX, mBoard->mNextLevelDesc->mGunY);
    if (mDoTempleUp)
    {
        m3 = IntPoint(320, 200);
    }

    IntPoint m2((7 * m1.mX + 3 * m3.mX) / 10, (7 * m1.mY + 3 * m3.mY) / 10);

    AddFrogMove(m1, m2, 70, 80);
    AddFrogMove(m2, m3, 40, 0);
    AddFrogScale(1.0f, 3.0f, 50, 100);
    if (mDoTempleUp)
    {
        AddFrogScale(3.0f, 0.01f, 125, 20);
    }
    else
    {
        AddFrogScale(3.0f, 1.0f, 75, 20);
        mBoard->mSoundMgr->AddSound(Sexy::SOUND_FROGLAND, 245);
    }

    mState = TransitionState_Quake;
    mStateCount = 0;
}

void DrawTextParticles(
    std::string const &theString,
    int theX,
    int theY,
    ParticleMgr *theParticleMgr,
    int theStagger)
{
    Sexy::MemoryImage aImage(gSexyAppBase);

    aImage.Create(Sexy::FONT_HUGE->StringWidth(theString), Sexy::FONT_HUGE->GetHeight());
    aImage.SetImageMode(true, true);

    Graphics sg(&aImage);
    sg.SetFont(Sexy::FONT_HUGE);
    sg.SetColor(Color(0xffffff));

    sg.DrawString(theString, 0, Sexy::FONT_HUGE->GetAscent());
    theY -= Sexy::FONT_HUGE->GetAscent();

    ulong *aBits = aImage.GetBits();

    for (int i = 0; i < aImage.mHeight; i++)
    {
        for (int j = 0; j < aImage.mWidth; j++)
        {
            if ((*aBits++ & 0xff000000) == 0xff000000 && (Sexy::Rand() % 50) == 0)
            {
                uint color = gBrightBallColors[Sexy::Rand() % MAX_BALL_COLORS];
                int stagger = theStagger + Sexy::Rand() % 10;

                theParticleMgr->AddSparkle(j + theX, i + theY, 0, 0, 0, 0, stagger, color);
            }
        }
    }
}

void TransitionMgr::DoStageComplete()
{
    mBoard->mSoundMgr->AddSound(Sexy::SOUND_STAGE_COMPLETE, 20);
    mTargetLevelFade = 128;
    if (!mBoard->mApp->Is3DAccelerated())
    {
        mBoard->mGun->SetAngle(0.0f);
        mBoard->mGun->CreateCachedGunImage();
    }

    mStateCount = 0;
    mState = TransitionState_StageComplete;
    Sexy::Font *aFont = Sexy::FONT_HUGE;

    std::string aText;
    if (!mBoard->mPracticeBoard.empty())
    {
        aText = gGauntletStages[mBoard->mLevelDesc->mStage];
    }
    else
    {
        aText = Sexy::StrFormat("STAGE %d", mBoard->mLevelDesc->mStage + 1);
    }

    int aTextWidth = 0;
    for (int i = 0; i < aText.size(); i++)
    {
        aTextWidth += aFont->CharWidth(aText[i]);
    }

    int letX = 320 - aTextWidth / 2;
    int gunCenterX = mBoard->mGun->GetCenterX();
    int gunCenterY = mBoard->mGun->GetCenterY();
    int aFrogStagger = 20;
    int aLetterStagger = 0;

    for (int i = 0; i < aText.size(); i++)
    {
        char aChar = aText[i];
        if (aChar == ' ')
        {
            letX += aFont->CharWidth(' ');
        }
        else
        {
            IntPoint m1(gunCenterX, gunCenterY);
            IntPoint m2((gunCenterX + letX) / 2, (gunCenterY + 250) / 2);
            IntPoint m3(letX, 250);

            AddFrogMove(m1, m2, 20, aFrogStagger);
            AddFrogScale(1.0f, 2.0f, 20, aFrogStagger);
            AddFrogMove(m2, m3, 15, 0);
            AddFrogScale(2.0f, 1.0f, 15, 0);

            aLetterStagger += aFrogStagger + 35;
            aFrogStagger = 0;

            AddLetterStamp(aChar, m3.mX, m3.mY - 100, 0xffff00, aLetterStagger);

            DrawTextParticles(std::string() + aChar, letX, 150, mBoard->mParticleMgr, aLetterStagger + 8);
            gunCenterX = letX;
            gunCenterY = 250;
            letX += aFont->CharWidth(aChar);
        }
    }

    IntPoint em1(gunCenterX, 250);
    IntPoint em2(150, 250);
    IntPoint em3(500, 250);
    IntPoint em4(320, 250 + mBoard->mGun->GetHeight());

    AddFrogMove(em1, em2, 20, 0);
    mStageCompleteFrame = aLetterStagger + 20;
    AddFrogMove(em2, em3, 50, 0);
    AddFrogMove(em3, em4, 30, 0);

    aText = "COMPLETE";

    int aCompWidth = aFont->StringWidth(aText);
    int v42 = 0;
    int v28 = 0;

    for (int i = 0; i < 100; i++)
    {
        mBoard->mParticleMgr->AddSparkle(
            ((320 - aCompWidth / 2) + v28 / 100),
            250,
            0,
            (Sexy::Rand() % 5) / 10.0 + 0.25,
            0,
            Sexy::Rand() % 50 + 10,
            (mStageCompleteFrame + v42 / 2) + Sexy::Rand() % 10,
            gBrightBallColors[Sexy::Rand() % MAX_BALL_COLORS]);

        v42++;
        v28 += aCompWidth;
    }
}

void TransitionMgr::DoTempleComplete()
{
    Clear();
    mBoard->mApp->PlaySong(32, 1, 0.01);
    mBoard->mSoundMgr->AddSound(Sexy::SOUND_TEMPLE_COMPLETE, 20);
    mStateCount = 0;
    mState = TransitionState_TempleComplete;

    const char *aText[5];
    if (mBoard->mApp->mProfile->mHasWon)
    {
        aText[0] = "Ribbit.  (I beg to differ.)";
        aText[1] = "Ribbit.  (I shall never give up.)";
        aText[2] = "Ribbit.  (I will fulfill the prophecy.)";
        aText[3] = "Ribbit?  (In space?)";
        aText[4] = "Ribbit!  (Werd!)";
    }
    else
    {
        aText[0] = "Ribbit.";
        aText[1] = "Ribbit.";
        aText[2] = "Ribbit.";
        aText[3] = "Ribbit?";
        aText[4] = "Ribbit!";
    }

    switch (mBoard->mLevelDesc->mStage / 3)
    {
    case 0:
    {
        int aStagger = AddTextBlurbWrap("Interloper of puny!  You suppose the secret of Zuma it is possible to take the knowing so easily?  "
                                        "No!  There is defect for your thinking!  Our cryptic hidden nature cannot be discovered "
                                        "so directly.  Three mystery shrines protect method!  Never you will strike past them!",
                                        220, 180, 380, 150);
        aStagger = AddTextBlurb(aText[0], 200, 380, aStagger + 100);
        mResetFrame = aStagger + 100;
        break;
    }
    case 1:
    {
        int aStagger = AddTextBlurbWrap("I see what?!?  You have lived still.  Is it possible to be the one of which the ancient "
                                        "prophecy speaks?  It was sent in order perhaps to release me from my wicked capture person finally.  "
                                        "But no namely this was the story of the exactly old wive.  You want to live, now retreat!",
                                        220, 180, 380, 150);
        aStagger = AddTextBlurb(aText[1], 200, 380, aStagger + 100);
        mResetFrame = aStagger + 100;
        break;
    }
    case 2:
    {
        int aStagger = AddTextBlurbWrap("As for me it is not possible to believe!  Your power namely that is large!  But there is one "
                                        "more temple which you do not find, that the Zuma deeply buried under the land.  It was hidden, "
                                        "it is the temple of secret!  You the final temple must fight in order to take the cover of position of my jail!!",
                                        220, 180, 380, 150);
        aStagger = AddTextBlurb(aText[1], 200, 380, aStagger + 100);
        mResetFrame = aStagger + 100;
        break;
    }
    case 3:
    {
        std::string aStr = "The final temple of Zuma was struck!  Your extreme power is not possible to be defeated!  "
                           "As for me it can taste the taste whose almost freedom is sweet sweetly.  But God of the sun can "
                           "be imprisoned at only that true house.";
        int aStagger = AddTextBlurbWrap(aStr, 220, 180, 380, 150);
        aStagger - AddTextBlurb(aText[1], 200, 380, aStagger + 100);

        int aTextY = 6 * Sexy::FONT_DIALOG->GetLineSpacing() + 180;
        aStagger - AddTextBlurbWrap("We come from the star, return to the star!  It is of you to rescue of my star.",
                                    220, aTextY, 380, aStagger + 50);
        mResetFrame = aStagger + 100;
        break;
    }
    case 4:
    {
        int aStagger = AddTextBlurbWrap("Excellent work, mighty frog one!  You obtained your lawful place of the stars, at the side of the ancient "
                                        "ruler of the Zuma.  In the future, as for me, you are known as our sibling.  I will call you brother, "
                                        "and we control the outer space together!  It is joyous news!  Now we are to the dance!",
                                        220, 180, 380, 150);
        aStagger = AddTextBlurb(aText[4], 200, 380, aStagger + 100);
        mResetFrame = aStagger + 100;
        break;
    }
    default:
        mResetFrame = 250;
        break;
    }
}

void TransitionMgr::DoLevelUp()
{
    Clear();

    if (gForceStageComplete)
    {
        mDoStageUp = true;
        mDoTempleUp = true;
        DoStageComplete();
        return;
    }

    mQuakeFrame = 20;
    for (int i = 0; i < mBoard->mNumCurves; i++)
    {
        if (i != 0)
            mQuakeFrame += 50;

        mQuakeFrame = mBoard->mCurveMgr[i]->DrawEndLevelBonus(mQuakeFrame);
    }

    mQuakeFrame += 50;
    mState = TransitionState_Bonus;
    mStateCount = 0;
    mDoStageUp = mBoard->mNextLevelDesc->mStage != mBoard->mLevelDesc->mStage;

    if (mBoard->IsPracticeMode() || !mDoStageUp)
    {
        mDoTempleUp = false;
    }
    else
    {
        int aStage = mBoard->mLevelDesc->mStage;
        if ((aStage + 1) % 3 == 0 || aStage == 12)
            mDoTempleUp = true;
        else
            mDoTempleUp = false;
    }
}

void TransitionMgr::DoLosing()
{
    Clear();
    mResetFrame = 0;
    mStateCount = 0;
    mState = TransitionState_Losing;
    mCurFrogAngle = mBoard->mGun->GetAngle();
}

void TransitionMgr::MouseDown(int x, int y, int theClickCount)
{
    switch (mState)
    {
    case TransitionState_LevelBegin:
        mBoard->mSoundMgr->KillAllSounds();
        mBoard->StartLevel();
        break;
    case TransitionState_TempleComplete:
        if (mStateCount < mResetFrame)
        {
            FinishAllTextBlurbs();
            mResetFrame = mStateCount;
        }

        mBoard->mGun->DoBlink();
        break;
    case TransitionState_Losing:
        if (mResetFrame > 0)
        {
            if (mBoard->mLives > 0)
            {
                mBoard->Reset(false, true);
            }
        }
        break;
    }
}

void TransitionMgr::UpdateFrogScale()
{
    if (!mFrogScaleList.empty())
    {
        FrogScale &aScale = mFrogScaleList.front();

        aScale.mUpdateCnt++;
        if (aScale.mUpdateCnt < 0)
            return;

        float aDelta = (float)aScale.mUpdateCnt / (float)aScale.mDuration;

        mCurFrogScale = (1.0f - aDelta) * aScale.mStartScale + aDelta * aScale.mEndScale;

        if (aScale.mUpdateCnt >= aScale.mDuration)
            mFrogScaleList.pop_front();
    }
}

void TransitionMgr::UpdateFrogMove()
{
    if (!mFrogMoveList.empty())
    {
        FrogMove &aMove = mFrogMoveList.front();

        aMove.mUpdateCnt++;
        if (aMove.mUpdateCnt < 0)
            return;

        float aDelta = (float)aMove.mUpdateCnt / (float)aMove.mDuration;

        mBoard->mGun->SetPos((1.0f - aDelta) * aMove.mStartPos.mX + aDelta * aMove.mEndPos.mX,
                             (1.0f - aDelta) * aMove.mStartPos.mY + aDelta * aMove.mEndPos.mY);

        mBoard->MouseMove(mBoard->mWidgetManager->mLastMouseX, mBoard->mWidgetManager->mLastMouseY);

        if (aMove.mUpdateCnt >= aMove.mDuration)
            mFrogMoveList.pop_front();
    }
}

void TransitionMgr::UpdateLetterStamp()
{
    for (LetterStampList::iterator anItr = mLetterStampList.begin(); anItr != mLetterStampList.end();)
    {
        LetterStamp &aStamp = *anItr;
        aStamp.mUpdateCnt++;

        if (aStamp.mDuration <= 0 || aStamp.mUpdateCnt < aStamp.mDuration)
        {
            anItr++;
        }
        else
        {
            LetterStampList::iterator aRemoveItr = anItr++;
            mLetterStampList.erase(aRemoveItr);
        }
    }
}

void TransitionMgr::UpdateLevelBegin()
{
    if (mStateCount > mResetFrame)
        mBoard->StartLevel();
}

void TransitionMgr::UpdateBonus()
{
    if (mStateCount >= mQuakeFrame)
    {
        if (mBoard->mNextLevelDesc->mStage != mBoard->mLevelDesc->mStage)
        {
            DoStageComplete();
        }
        else if (mBoard->mPracticeMode == PracticeMode_Single)
        {
            mBoard->Reset(false, false);
        }
        else
        {
            DoQuake();
        }
    }
    else if (mStateCount == mQuakeFrame - 1)
    {
        mBoard->mApp->PlaySample(Sexy::SOUND_LEVEL_UP);
        mBoard->mScoreDisplay = mBoard->mScore;
        mBoard->mApp->DoStatsDialog(true, true);
    }
}

void TransitionMgr::UpdateQuake()
{
    if (mStateCount == 100)
    {
        mTargetLevelFade = 0;
        mLetterStampList.clear();
    }

    if (mBoard->mPracticeMode != PracticeMode_Single)
    {
        mBoard->mSpriteMgr->UpdateBackgroundTransition(mStateCount);
    }

    if (mDoTempleUp)
    {
        if (mStateCount >= 300)
        {
            DoTempleComplete();
        }
    }
    else if (mStateCount >= 250)
    {
        mBoard->Reset(false, false);
    }
}

void TransitionMgr::UpdateStageComplete()
{
    if (mStateCount == mStageCompleteFrame + 50)
    {
        std::string aText = "COMPLETE";
        int aCompWidth = Sexy::FONT_HUGE->StringWidth(aText); // return ignored??

        for (int i = 0; i < 500; i++)
        {
            mBoard->mParticleMgr->AddSparkle(
                Sexy::Rand() % CIRCLE_WINDOW_WIDTH,
                Sexy::Rand() % CIRCLE_WINDOW_HEIGHT,
                0.0f,
                (Sexy::Rand() % 5) * 0.1f + 0.25f,
                4,
                Sexy::Rand() % 50 + 10,
                Sexy::Rand() % 80 + 10,
                gBrightBallColors[Sexy::Rand() % MAX_BALL_COLORS]);
        }
    }
    else if (mStateCount == mStageCompleteFrame + 100)
    {
        mBoard->mGun->DoBlink(true);
    }

    if (mStateCount > mStageCompleteFrame + 125)
        DoQuake();
}

void TransitionMgr::UpdateTempleComplete()
{
    mBoard->mGun->SetPos((sin((mStateCount * SEXY_PI) / 180.0f) * -1.5f) * 5.0f + 120.0f,
                         (cos((mStateCount * SEXY_PI) / 180.0f) * -1.5f) * 5.0f + 380.0f);

    if (mStateCount > mResetFrame)
    {
        mBoard->mContinueButton->SetVisible(true);
    }
}

void TransitionMgr::UpdateLosing()
{
    if (mResetFrame == 0 || mResetFrame - mStateCount > 200)
    {
        if ((mStateCount % 30) == 0)
        {
            mBoard->mSoundMgr->AddSound(Sexy::SOUND_TRAIL_LIGHT, 0, 0, mStateCount / 30);
        }
    }

    if (mResetFrame > 0 && mResetFrame - mStateCount == 205)
    {
        mBoard->mSoundMgr->AddSound(Sexy::SOUND_POP);
        if (mBoard->mLives == 0)
        {
            mBoard->mSoundMgr->AddSound(Sexy::SOUND_GAME_OVER, 20);
        }
        else
        {
            mBoard->mSoundMgr->AddSound(Sexy::SOUND_LOST_LIFE, 20);
        }
    }

    if (mStateCount > 75)
    {
        if (mResetFrame == 0 || mResetFrame - mStateCount > 200)
        {
            int v4 = (mStateCount / 60 > 5) ? 5 : mStateCount / 60;
            for (int i = 0; i < v4; i++)
            {
                float anAngle = ((Sexy::Rand() % 360) * SEXY_PI) / 180.0f;
                float aVel = (Sexy::Rand() % 10 + 1);

                mBoard->mParticleMgr->AddSparkle(
                    mBoard->mGun->GetCenterX(),
                    mBoard->mGun->GetCenterY(),
                    aVel * sinf(anAngle),
                    aVel * cosf(anAngle),
                    4,
                    Sexy::Rand() % 30 + 20,
                    0,
                    gBrightBallColors[Sexy::Rand() % 6]);
            }
        }
    }

    if (mResetFrame != 0)
    {
        if (mResetFrame == mStateCount)
        {
            if (mBoard->mLives <= 0)
            {
                mBoard->mApp->DoStatsDialog(true, true);
            }
            else
            {
                int aLevelFade = mLevelFade;
                mBoard->Reset(false, true);
                mLevelFade = aLevelFade; // ??
                mTargetLevelFade = 0;

                LevelDesc *aLevelDesc = mBoard->mLevelDesc;
                IntPoint frogEndPos(aLevelDesc->mGunX, aLevelDesc->mGunY);
                IntPoint frogStartPos(-100, -100);

                AddFrogMove(frogStartPos, frogEndPos, 50, 0);
                mBoard->mGun->SetPos(-100, -100);
            }
        }
        return;
    }

    bool aCanRestart = true;

    for (int i = 0; i < mBoard->mNumCurves; i++)
    {
        mBoard->mCurveMgr[i]->UpdateLosing();
        if (!mBoard->mCurveMgr[i]->CanRestart())
        {
            aCanRestart = false;
        }
    }

    if (!aCanRestart)
        return;

    if (mStateCount <= 150)
        return;

    mResetFrame = mStateCount + 300;

    std::string aText;

    if (mBoard->mLives <= 0)
    {
        aText = "GAME OVER";
    }
    else if (mBoard->mLives == 1)
    {
        aText = Sexy::StrFormat("LAST LIFE!"); // why is this a StrFormat?
    }
    else
    {
        aText = Sexy::StrFormat("%d LIVES LEFT", mBoard->mLives);
    }

    int len = Sexy::FONT_HUGE->StringWidth(aText);
    AddLetterStamp(aText, 320 - len / 2, 240, 0xFFFFFF, 110, Sexy::FONT_HUGE_ID, 160);
    mBoard->mSoundMgr->StopLoop(LoopType_RollOut);
    mTargetLevelFade = 128;
    mLevelFadeInc = 2;
}

void TransitionMgr::UpdateTextBlurb()
{
    for (TextBlurbList::iterator anItr = mTextBlurbList.begin();
         anItr != mTextBlurbList.end();
         anItr++)
    {
        int aTick = 4;

        if (anItr->mCharNum >= 0 && anItr->mCharNum < anItr->mText.size())
        {
            char aChar = anItr->mText[anItr->mCharNum];
            char aNext = anItr->mCharNum < anItr->mText.size() - 1 ? anItr->mText[anItr->mCharNum + 1] : 0;

            if ((aChar == '.' || aChar == '?' || aChar == '!') && aNext != '.' && aNext != '?' && aNext != '!')
            {
                aTick = 16;
            }
        }

        anItr->mDelayCnt++;
        if (aTick <= anItr->mDelayCnt)
        {
            anItr->mCharNum++;
            anItr->mDelayCnt = 0;
        }
    }
}

void TransitionMgr::FinishAllTextBlurbs()
{
    for (TextBlurbList::iterator anItr = mTextBlurbList.begin();
         anItr != mTextBlurbList.end();
         anItr++)
    {
        anItr->mCharNum = anItr->mText.size();
    }
}