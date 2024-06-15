#include "Zuma_Prefix.pch"

#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/Graphics.h>

#include "Board.h"
#include "CircleButton.h"
#include "CircleCommon.h"
#include "CircleDialog.h"
#include "CircleShootApp.h"
#include "HighScoreMgr.h"
#include "StatsDialog.h"
#include "SoundMgr.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

StatsDialog::StatsDialog(Board *theBoard, bool doCounter) : CircleDialog(Sexy::IMAGE_DIALOG_BACK, Sexy::IMAGE_DIALOG_BUTTON, DialogType_Stats, true,
                                                                         "STATS", "", "OK",
                                                                         3, true)
{
    const char *aNextStageTexts[12];
    const char *aGameOverTexts[8];

    aNextStageTexts[0] = "Fill the orange gauge to complete a level. No new balls will enter the screen.";
    aNextStageTexts[1] = "Score points to fill the orange gauge.  The more points, the faster the gauge fills.";
    aNextStageTexts[2] = "Click the right mouse button to swap the current ball with the next ball.";
    aNextStageTexts[3] = "Shoot balls through a gap to score a gap bonus!  The smaller the gap, the larger the bonus!";
    aNextStageTexts[4] = "Create combos of 2 or more explosions to score big points and push the curve backwards!";
    aNextStageTexts[5] = "Close gaps by placing balls of the same color on either side.";
    aNextStageTexts[6] = "Get a coin for a big point boost to finish the level more quickly!";
    aNextStageTexts[7] = "Finish a level quickly and you can get bonus points!";
    aNextStageTexts[8] = "Get an extra frog every 50,000 points!";
    aNextStageTexts[9] = "Shoot balls through two or more gaps to score a double gap bonus!";
    aNextStageTexts[10] = "Press space bar to pause the game.";
    aNextStageTexts[11] = "Press 't' to show the amount of time left to get an Ace time.";
    aGameOverTexts[0] = "Once you can accept the universe as matter expanding into nothing that is some"
                        "thing, wearing stripes with plaid comes easy.";
    aGameOverTexts[1] = "There is only one corner of the universe you can be certain of improving and t"
                        "hat is your own self.";
    aGameOverTexts[2] = "It is always wise to look ahead but difficult to look farther than you can see.";
    aGameOverTexts[3] = "Good timber does not grow with ease. The stronger the wind the stronger the trees.";
    aGameOverTexts[4] = "The fact that no one understands you doesn't make you an artist.";
    aGameOverTexts[5] = "Honest criticism is hard to take, particularly from a relative, a friend, an a"
                        "cquaintance, or a stranger.";
    aGameOverTexts[6] = "A good plan today is better than a perfect plan tomorrow.";
    aGameOverTexts[7] = "Good luck is often with the man who doesn't include it in his plans.";

    mBoard = theBoard;
    mTimeBonus = 0;
    mTargetTimeBonus = 0;
    mIsGameOver = theBoard->IsGameOver();
    mIsWinning = theBoard->IsWinning();

    if (mIsGameOver)
    {
        mScore = mBoard->mScore;
        if (mIsWinning)
        {
            mDialogHeader = "WINNING TOTALS";
            mTargetTimeBonus = 50000 * mBoard->mLives;
        }
        else
        {
            mDialogHeader = "GAME OVER";
        }

        mCurrentTip = aGameOverTexts[rand() % 8];
    }
    else
    {
        mScore = mBoard->mScore - mBoard->mLevelBeginScore;
        if (mBoard->mPracticeBoard.empty() && mBoard->mNextLevelDesc->mInSpace)
        {
            mCurrentTip = "Press the 's' key to toggle star scrolling.";
        }
        else
        {
            mCurrentTip = aNextStageTexts[mBoard->mLevel % 12];
        }

        mTargetTimeBonus = mBoard->GetParBonus(mBoard->mLevelStats.mTimePlayed / 100, mBoard->mLevelDesc->mParTime);
        LowTime *aLowTime = mBoard->mApp->mHighScoreMgr->GetLowTime(mBoard->GetUniqueLevelString());
        if (aLowTime != NULL)
        {
            mBestTimeLeft = "Best Time " + Sexy::GetTimeString(aLowTime->mTime);
            mBestTimeRight = aLowTime->mName;
        }
    }

    if (!doCounter)
    {
        mTimeBonus = mTargetTimeBonus;
    }

    mHighScoreChecked = false;
}

StatsDialog::~StatsDialog()
{
}

void StatsDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
    Dialog::Resize(theX, theY, theWidth, theHeight);
}

int StatsDialog::GetPreferredHeight(int theWidth)
{
    return ((!mIsWinning && mIsGameOver) ? 140 : 180) + CircleDialog::GetPreferredHeight(theWidth);
}

void StatsDialog::AddedToManager(WidgetManager *theWidgetManager)
{
    Dialog::AddedToManager(theWidgetManager);
}

void StatsDialog::RemovedFromManager(WidgetManager *theWidgetManager)
{
    Dialog::RemovedFromManager(theWidgetManager);
}

void StatsDialog::Update()
{
    Widget::Update();

    if (mTimeBonus < mTargetTimeBonus)
    {
        if (mUpdateCnt > 100)
        {
            if (mUpdateCnt % 10 == 1)
            {
                mBoard->mSoundMgr->AddSound(
                    Sexy::SOUND_BONUS_EXPLOSION, 0, 0,
                    (float)mTimeBonus / (mIsWinning ? 25000.0f : 1000.0f));
            }

            int aOldTimeBonus = mTimeBonus;
            mTimeBonus = (!mIsWinning ? -2400 : 0) + mTimeBonus + 2500;
            if (mTimeBonus > mTargetTimeBonus)
            {
                mTimeBonus = mTargetTimeBonus;
            }

            mBoard->IncScore(mTimeBonus - aOldTimeBonus, false);

            if (mIsGameOver)
            {
                mScore = mBoard->mScore;
            }
            else
            {
                mScore = mBoard->mScore - mBoard->mLevelBeginScore;
            }

            MarkDirty();
        }
    }
}

void StatsDialog::ButtonDepress(int theId)
{
    if (mTimeBonus < mTargetTimeBonus)
    {
        mBoard->IncScore(mTargetTimeBonus - mTimeBonus, false);
        mTimeBonus = mTargetTimeBonus;
    }

    if (!mHighScoreChecked && mIsWinning)
    {
        mBoard->CheckHighScore();
        mHighScoreChecked = true;
    }

    Dialog::ButtonDepress(theId);
}

void StatsDialog::Draw(Graphics *g)
{
    char buf[50];

    CircleDialog::Draw(g);

    g->SetFont(Sexy::FONT_MAIN12);
    int aDialogTop = GetTop() - mY + 10;
    int aDialogLeft = GetLeft() - mX;

    int aCol1 = aDialogLeft + 10;
    int aCol2 = aDialogLeft + 140;
    int aCol3 = aDialogLeft + 220;
    int aCol4 = aDialogLeft + 320;
    int v34;

    int aLineOffset = 6 + Sexy::FONT_MAIN12->GetLineSpacing();
    if (!mIsGameOver)
    {
        g->SetColor(Color(0xFFFF00));
        g->DrawString("POINTS", aCol1, aDialogTop);
        g->DrawString("COMBOS", aCol1, aDialogTop + aLineOffset);
        g->DrawString("COINS", aCol1, aDialogTop + 2 * aLineOffset);
        g->DrawString("GAPS", aCol1, aDialogTop + 3 * aLineOffset);
        g->DrawString("MAX CHAIN", aCol1, aDialogTop + 4 * aLineOffset);
        g->DrawString("MAX COMBO", aCol1, aDialogTop + 5 * aLineOffset);

        g->SetColor(Color(0xFFA500));
        g->DrawString(itoa(mScore, buf, 10), aCol2, aDialogTop);
        g->DrawString(itoa(mBoard->mLevelStats.mNumCombos, buf, 10), aCol2, aDialogTop + aLineOffset);
        g->DrawString(itoa(mBoard->mLevelStats.mNumGemsCleared, buf, 10), aCol2, aDialogTop + 2 * aLineOffset);
        g->DrawString(itoa(mBoard->mLevelStats.mNumGaps, buf, 10), aCol2, aDialogTop + 3 * aLineOffset);
        g->DrawString(itoa(mBoard->mLevelStats.mMaxInARow, buf, 10), aCol2, aDialogTop + 4 * aLineOffset);
        g->DrawString(itoa(mBoard->mLevelStats.mMaxCombo + 1, buf, 10), aCol2, aDialogTop + 5 * aLineOffset);

        g->SetColor(Color(0xFFFF00));
        g->DrawString("YOUR TIME", aCol3, aDialogTop);
        g->DrawString("ACE TIME", aCol3, aDialogTop + aLineOffset);

        g->FillRect(aCol3, aDialogTop + aLineOffset + 4, 160, 1);

        g->SetColor(Color(mTargetTimeBonus > 0 ? 0x2AFF03 : 0xFFA500));
        g->DrawString(Sexy::GetTimeString(mBoard->mLevelStats.mTimePlayed / 100), aCol4, aDialogTop);

        g->SetColor(Color(0xFFA500));
        g->DrawString(Sexy::GetTimeString(mBoard->mLevelDesc->mParTime), aCol4, aDialogTop + aLineOffset);

        if (mTargetTimeBonus <= 0)
        {
            g->SetColor(Color(0x9DCBA2));
            g->DrawString("NO TIME BONUS", aCol3, 4 + aDialogTop + 2 * aLineOffset);
        }
        else if (mTimeBonus > 0)
        {
            g->SetColor(Color(0x2AFF03));
            g->DrawString(Sexy::StrFormat("BONUS +%d", mTimeBonus), aCol3, 4 + aDialogTop + 2 * aLineOffset);
        }

        g->SetColor(Color(0xF8F792));
        g->DrawString(mBestTimeLeft, aCol3, aDialogTop + 4 * aLineOffset);
        g->DrawString(mBestTimeRight, aCol3, aDialogTop + 5 * aLineOffset);

        v34 = aDialogTop + 7 * aLineOffset;
    }
    else
    {
        int v33 = aDialogLeft + 350;
        int v32 = aDialogLeft + 130;

        if (mIsWinning)
        {
            g->SetColor(Color(0xFFFF00));
            g->DrawString("EXTRA LIFE BONUS", aCol1, aDialogTop);
            g->SetColor(Color(0x2AFF03));
            if (mTimeBonus > 0)
            {
                g->DrawString(Sexy::StrFormat("+%d", mTimeBonus), aCol3, aDialogTop);
            }

            aDialogTop += 2 * aLineOffset;
        }

        g->SetColor(Color(0xFFFF00));
        g->DrawString("TOTAL TIME", aCol1, aDialogTop);
        g->DrawString("COMBOS", aCol1, aDialogTop + aLineOffset);
        g->DrawString("COINS", aCol1, aDialogTop + 2 * aLineOffset);
        g->DrawString("GAPS", aCol3, aDialogTop);
        g->DrawString("MAX CHAIN", aCol3, aDialogTop + aLineOffset);
        g->DrawString("MAX COMBO", aCol3, aDialogTop + 2 * aLineOffset);

        g->SetColor(Color(0xFFA500));
        g->DrawString(Sexy::GetTimeString(mBoard->mGameStats.mTimePlayed / 100), v32, aDialogTop);
        g->DrawString(itoa(mBoard->mGameStats.mNumCombos, buf, 10), v32, aDialogTop + aLineOffset);
        g->DrawString(itoa(mBoard->mGameStats.mNumGemsCleared, buf, 10), v32, aDialogTop + 2 * aLineOffset);
        g->DrawString(itoa(mBoard->mGameStats.mNumGaps, buf, 10), v33, aDialogTop);
        g->DrawString(itoa(mBoard->mGameStats.mMaxInARow, buf, 10), v33, aDialogTop + aLineOffset);
        g->DrawString(itoa(mBoard->mGameStats.mMaxCombo + 1, buf, 10), v33, aDialogTop + 2 * aLineOffset);

        v34 = aDialogTop + 4 * aLineOffset;
    }

    if (!mCurrentTip.empty())
    {
        g->SetColor(Color(0xFFFF00));
        g->DrawString("TIP", aCol1, v34 + 4);
        g->SetColor(Color(0xFFFFFF));
        g->SetFont(Sexy::FONT_DIALOG);

        Rect aRc = Rect(
            GetLeft() - mX + 60,
            v34 - g->GetFont()->GetAscent(),
            GetWidth() - 80,
            200);

        WriteWordWrapped(g, aRc, mCurrentTip, -1, -1);
    }
}

void StatsDialog::FinishScoreInc()
{
    if (mTimeBonus < mTargetTimeBonus)
    {
        mBoard->IncScore(mTargetTimeBonus - mTimeBonus, false);
        mTimeBonus = mTargetTimeBonus;
    }
}