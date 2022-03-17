#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SexyAppBase.h>
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
        float aScale = 1.0f;

        if (aStamp.mUpdateCnt >= 30)
        {
            if (aRem > 9)
            {
                g->DrawImage(aStamp.mImage, aPosX, aPosY);
                continue;
            }

            aScale = (aRem / 20.0f) + 0.5f;
        }
        else
        {
            if (aRem > 9)
            {
                if (aStamp.mUpdateCnt > 9)
                {
                    aScale = ((20 - (aStamp.mUpdateCnt - 10)) * 1.1f + (aStamp.mUpdateCnt - 10)) / 20.0f;
                }
                else
                {
                    aScale = (aStamp.mUpdateCnt * 0.6f) / 10.0f + 0.5f;
                }
            }
            else
            {
                aScale = (aRem / 20.0f) + 0.5f;
            }
        }

        if (aScale > 0.01f)
        {
            int aWidth = aStamp.mImage->mWidth;
            int aHeight = aStamp.mImage->mHeight;

            Rect aSrc(0, 0, aWidth, aHeight);
            Rect aDest(aPosX + aWidth / 2 - (aWidth * aScale) / 2,
                       aPosY + aHeight / 2 - (aHeight * aScale) / 2,
                       aWidth * aScale,
                       aHeight * aScale);

            bool isFastStretch = g->GetFastStretch();
            g->SetFastStretch(!gSexyAppBase->Is3DAccelerated());
            g->DrawImage(aStamp.mImage, aDest, aSrc);
            g->SetFastStretch(isFastStretch);
        }
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

void TransitionMgr::DrawStageCompleteText(Graphics *g) {}

void TransitionMgr::DrawStageComplete(Graphics *g) {}

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

void TransitionMgr::DrawLosing(Graphics *g) {}

void TransitionMgr::DrawTempleComplete(Graphics *g) {}

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
        if (mLevelFade >= mLevelFadeInc)
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
        if (mStateCount > mResetFrame)
            mBoard->StartLevel();
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

void TransitionMgr::SyncState(DataSync &theSync) {}

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
                   aTextLeft, aTextTop, 0xFFFFFF, 20,
                   Sexy::FONT_HUGE_ID, mResetFrame - 40);

    Font *aFont = Sexy::GetFontById(Sexy::FONT_BROWNTITLE_ID);
    int anAscent = aFont->GetAscent();
    int anAscentPadding = aFont->GetAscentPadding();
    int aSmallWidth = aFont->StringWidth(mBoard->mLevelDesc->mDisplayName);

    AddLetterStamp(mBoard->mLevelDesc->mDisplayName,
                   aTextLeft + aVerbWidth - aSmallWidth,
                   aTextTop + anAscent - anAscentPadding + 4,
                   0xFFFFFF, 60, Sexy::FONT_BROWNTITLE_ID, mResetFrame - 90);
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
        m3.mY = 200;
        m3.mX = 320;
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

    std::string aText;
    if (!mBoard->mPracticeBoard.empty())
    {
        aText = gGauntletStages[mBoard->mLevelDesc->mStage];
    }
    else
    {
        aText = Sexy::StrFormat("STAGE %d", mBoard->mLevelDesc->mStage + 1);
    }

    int v2 = 0;
    for (int i = 0; i < aText.size(); i++)
    {
        v2 += Sexy::FONT_HUGE->CharWidth(aText[i]);
    }

    int v33 = 320 - v2 / 2;
    int a5 = 20;
    int v35 = 0;

    for (int i = 0; i < aText.size(); i++)
    {
        char v10 = aText[i];
        if (v10 == ' ')
        {
            v33 += Sexy::FONT_HUGE->CharWidth(' ');
        }
        else
        {
            IntPoint m1(mBoard->mGun->GetCenterX(), mBoard->mGun->GetCenterY());
            IntPoint m2((mBoard->mGun->GetCenterX() + v33) / 2, (mBoard->mGun->GetCenterY() + 250) / 2);
            IntPoint m3(v33, 250);

            AddFrogMove(m1, m2, 20, a5);
            AddFrogScale(1.0f, 2.0f, 20, a5);
            AddFrogMove(m2, m3, 15, 0);
            AddFrogScale(2.0f, 1.0f, 15, 0);

            // todo
        }
    }
}

void TransitionMgr::DoTempleComplete() {}

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
    mDoTempleUp = false;
    if (!mBoard->IsPracticeMode() && mDoStageUp)
    {
        int aStage = mBoard->mLevelDesc->mStage;
        if (aStage + 1 == 3 * ((aStage + 1) / 3) || aStage == 12)
            mDoTempleUp = true;
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
    case TransitionState_Losing:
        if (mResetFrame > 0)
        {
            if (mBoard->mLives > 0)
            {
                mBoard->Reset(false, true);
            }
        }
        break;
    case TransitionState_TempleComplete:
        if (mStateCount < mResetFrame)
        {
            FinishAllTextBlurbs();
            mResetFrame = mStateCount;
        }

        mBoard->mGun->DoBlink();
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

        if (aStamp.mDuration > 0 && aStamp.mUpdateCnt >= aStamp.mDuration)
        {
            LetterStampList::iterator aRemoveItr = anItr++;
            mLetterStampList.erase(aRemoveItr);
        }
        else
        {
            anItr++;
        }
    }
}

void TransitionMgr::UpdateLevelBegin()
{
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

void TransitionMgr::UpdateStageComplete() {}

void TransitionMgr::UpdateTempleComplete() {}

void TransitionMgr::UpdateLosing() {}

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