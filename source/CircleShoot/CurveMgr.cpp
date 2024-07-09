#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/SoundManager.h>
#include <SexyAppFramework/SoundInstance.h>

#include "Board.h"
#include "Bullet.h"
#include "CurveMgr.h"
#include "CircleShootApp.h"
#include "CircleCommon.h"
#include "CurveData.h"
#include "DataSync.h"
#include "LevelParser.h"
#include "WayPoint.h"
#include "ParticleMgr.h"
#include "ProfileMgr.h"
#include "SpriteMgr.h"
#include "SoundMgr.h"
#include "Res.h"

#include <math.h>

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void BallDrawer::Reset()
{
    for (int i = 0; i < 5; i++)
    {
        mNumBalls[i] = 0;
        mNumShadows[i] = 0;
    }
}

// void BallDrawer::AddBall(Ball *theBall, int thePriority)
// {
// }

// void BallDrawer::AddShadow(Ball *theBall, int thePriority)
// {
// }

void BallDrawer::Draw(Graphics *g, SpriteMgr *theSpriteMgr, ParticleMgr *theParticleMgr)
{
    for (int i = 0; i < 5; i++)
    {
        theSpriteMgr->DrawSprites(g, i);
        theParticleMgr->Draw(g, i);

        int aNumShadows = mNumShadows[i];
        for (int j = 0; j < aNumShadows; j++)
        {
            mShadows[i][j]->DrawShadow(g);
        }

        int aNumBalls = mNumBalls[i];
        for (int j = 0; j < aNumBalls; j++)
        {
            mBalls[i][j]->Draw(g);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CurveMgr::CurveMgr(Board *theBoard)
{
    mApp = theBoard->mApp;
    mBoard = theBoard;
    mSpriteMgr = NULL; // ??
    mWayPointMgr = new WayPointMgr();
}

CurveMgr::~CurveMgr()
{
    DeleteBalls();

    delete mWayPointMgr;
}

void CurveMgr::SetLosing()
{
    for (BulletList::iterator anItr = mBulletList.begin(); anItr != mBulletList.end(); anItr++)
    {
        Bullet *aBullet = *anItr;
        delete aBullet;
    }

    mBulletList.clear();

    for (BallList::iterator anItr = mBallList.begin(); anItr != mBallList.end(); anItr++)
    {
        Ball *aBall = *anItr;
        aBall->SetSuckCount(mAdvanceSpeed * 4.0f);
    }
}

void CurveMgr::SetupLevel(LevelDesc *theDesc, SpriteMgr *theSpriteMgr, int theCurveNum, MirrorType theMirror)
{
    mLevelDesc = theDesc;
    mCurveDesc = &theDesc->mCurveDesc[theCurveNum];
    mSpriteMgr = theSpriteMgr;
    mWayPointMgr->LoadCurve(theDesc->mCurveDesc[theCurveNum].mPath, theMirror);
    mCurveNum = theCurveNum;

    float aSkullRotation = mCurveDesc->mSkullRotation;
    if (aSkullRotation >= 0.0f)
    {
        aSkullRotation = aSkullRotation * SEXY_PI / 180.0f;
    }

    int aHoleX = 0;
    int aHoleY = 0;

    if (!mWayPointMgr->GetWayPointList().empty())
    {
        mWayPointMgr->CalcPerpendicularForPoint(mWayPointMgr->GetEndPoint());
        WayPoint const &aPoint = mWayPointMgr->GetWayPointList().back();

        aHoleX = aPoint.x;
        aHoleY = aPoint.y;

        if (aSkullRotation < 0.0f)
            aSkullRotation = aPoint.mRotation;
    }

    mSpriteMgr->PlaceHole(mCurveNum, aHoleX, aHoleY, aSkullRotation);
    mDangerPoint = mWayPointMgr->GetNumPoints() - mCurveDesc->mDangerDistance;

    if (mDangerPoint >= mWayPointMgr->GetNumPoints())
        mDangerPoint = mWayPointMgr->GetEndPoint();
}

void CurveMgr::SetupLevelDesc(LevelDesc *theDesc)
{
    mLevelDesc = theDesc;
    mCurveDesc = &theDesc->mCurveDesc[mCurveNum];

    mDangerPoint = mWayPointMgr->GetNumPoints() - mCurveDesc->mDangerDistance;

    if (mDangerPoint >= mWayPointMgr->GetNumPoints())
        mDangerPoint = mWayPointMgr->GetEndPoint();
}

void MakeCombo(BallList &theList, int theNumBalls, int theComboSize, int theNumColors)
{
    std::vector<int> aBallCounts;
    std::vector<int> aBallTypes;

    aBallTypes.resize(theComboSize);
    aBallCounts.resize(theComboSize);

    int aBallCount = theNumBalls;
    int aPrevColor = -1;

    for (int i = 0; i < theComboSize; i++)
    {
        int aNewColor;
        do
        {
            aNewColor = Sexy::AppRand() % theNumColors;
        } while (aPrevColor == aNewColor);

        aBallTypes[i] = aNewColor;
        aBallCounts[i] = 3;
        aBallCount -= 3;
        aPrevColor = aNewColor;
    }

    for (int i = 0; i < aBallCount; i++)
    {
        aBallCounts[Sexy::AppRand() % theComboSize]++;
    }

    BallList::iterator aBallItr = theList.begin();
    for (int i = 0; i < theComboSize; i++)
    {
        int aType = aBallTypes[i];
        int aCount = aBallCounts[i];

        for (int j = 0; j < aCount; j++)
        {
            bool insertNext = (Sexy::AppRand() % 2) != 0;
            Ball *aBall = new Ball();
            aBall->SetType(aType);
            aBall->RandomizeFrame();

            if (insertNext)
            {
                aBallItr = theList.insert(aBallItr, aBall);
            }
            else
            {
                theList.insert(theList.begin(), aBall);
            }
        }
    }
}

void CurveMgr::StartLevel()
{
    mLevelDesc = mBoard->mLevelDesc;
    mCurveDesc = &mLevelDesc->mCurveDesc[mCurveNum];
    mPathLightEndFrame = 0;
    mLastPathShowTick = Sexy::BoardGetTickCount() - 1000000;
    mLastClearedBallPoint = 0;

    for (int i = 0; i < (int)PowerType_Max; i++)
    {
        mLastPowerUpFrame[i] = mBoard->mStateCount - 1000;
    }

    mSpriteMgr->UpdateHole(mCurveNum, 0.0f);
    DeleteBalls();
    AppRand();

    int aNumBalls = mCurveDesc->mNumBalls;
    if (aNumBalls == 0)
        aNumBalls = 10;

    if (mBoard->mApp->mProfile->mMaxLevel < 2 && mLevelDesc->mStage < 1)
    {
        MakeCombo(mPendingBalls, 0, 2, mCurveDesc->mNumColors);
        MakeCombo(mPendingBalls, 0, 2, mCurveDesc->mNumColors);
        MakeCombo(mPendingBalls, 0, 2, mCurveDesc->mNumColors);
        MakeCombo(mPendingBalls, 0, 2, mCurveDesc->mNumColors);
        MakeCombo(mPendingBalls, 0, 2, mCurveDesc->mNumColors);
    }

    for (int i = 0; i < aNumBalls; i++)
    {
        AddPendingBall();
    }

    mStopTime = 0;
    mSlowCount = 0;
    mBackwardCount = 0;
    mTotalBalls = mCurveDesc->mNumBalls;
    mStopAddingBalls = false;
    mInDanger = false;
    mFirstChainEnd = 0;
    mFirstBallMovedBackwards = false;

    RollBallsIn();
}

void CurveMgr::UpdatePlaying()
{
    bool ballsAtBeginning = mBallList.empty() || mBallList.back()->GetWayPoint() < 50.0f;

    if (mStopTime > 0)
    {
        mStopTime--;

        if (ballsAtBeginning)
            mStopTime = 0;

        if (mStopTime == 0)
            mAdvanceSpeed = 0;
    }

    if (mSlowCount > 0)
    {
        mSlowCount--;

        if (ballsAtBeginning)
            mSlowCount = 0;
    }

    if (mBackwardCount > 0)
    {
        mBackwardCount--;

        if (ballsAtBeginning)
            mBackwardCount = 0;
    }

    AddBall();
    UpdateBallRotation();
    AdvanceBullets();
    UpdateSuckingBalls();
    AdvanceBalls();
    AdvanceBackwardBalls();
    RemoveBallsAtFront();
    UpdateSets();
    UpdatePowerUps();

    if (mBallList.empty())
    {
        SetFarthestBall(0);
    }
    else
    {
        SetFarthestBall(mBallList.back()->GetWayPoint());
    }
}

void CurveMgr::UpdateLosing()
{
    BallList::iterator aBallItr = mBallList.begin();
    float anEndPoint = mWayPointMgr->GetEndPoint();
    bool isDirty = false;

    while (aBallItr != mBallList.end())
    {
        Ball *aBall = *aBallItr;

        if (aBall->GetWayPoint() >= anEndPoint)
        {
            int aSuckCount = aBall->GetSuckCount();
            if (aSuckCount < 0)
            {
                aBall->SetSuckCount(aSuckCount + 1);
            }
            else
            {
                delete aBall;
                BallList::iterator aDeleteItr = aBallItr;
                ++aBallItr;
                mBallList.erase(aDeleteItr);

                isDirty = true;
                continue;
            }
        }
        else
        {
            mWayPointMgr->SetWayPoint(aBall, aBall->GetWayPoint() + (aBall->GetSuckCount() / 4));
            aBall->SetSuckCount(aBall->GetSuckCount() + 1);

            if (aBall->GetWayPoint() > anEndPoint)
            {
                aBall->SetSuckCount(0);
            }
        }

        aBallItr++;
        isDirty = true;
    }

    if (!mBallList.empty())
    {
        SetFarthestBall(mBallList.back()->GetWayPoint());
    }

    if (isDirty)
    {
        mBoard->MarkDirty();
    }
}

void CurveMgr::DrawBalls(BallDrawer &theDrawer)
{
    for (BallList::iterator aBallItr = mBallList.begin(); aBallItr != mBallList.end(); ++aBallItr)
    {
        Ball *aBall = *aBallItr;
        int aPriority = mWayPointMgr->GetPriority(aBall);
        Ball *aNextBall = aBall->GetNextBall(true);
        int aNextPriority = (aNextBall && aPriority > mWayPointMgr->GetPriority(aNextBall))
                                ? mWayPointMgr->GetPriority(aNextBall)
                                : aPriority;

        int aNumBalls = theDrawer.mNumBalls[aPriority]++;
        theDrawer.mBalls[aPriority][aNumBalls] = aBall;

        int aNumShadows = theDrawer.mNumShadows[aNextPriority]++;
        theDrawer.mShadows[aNextPriority][aNumShadows] = aBall;
    }

    for (BulletList::iterator aBulletItr = mBulletList.begin(); aBulletItr != mBulletList.end(); ++aBulletItr)
    {
        Bullet *aBullet = *aBulletItr;
        int aPriority = mWayPointMgr->GetPriority(aBullet);

        int aNumBalls = theDrawer.mNumBalls[aPriority]++;
        theDrawer.mBalls[aPriority][aNumBalls] = reinterpret_cast<Ball *>(aBullet);

        int aNumShadows = theDrawer.mNumShadows[aPriority]++;
        theDrawer.mShadows[aPriority][aNumShadows] = reinterpret_cast<Ball *>(aBullet);
    }
}

bool CurveMgr::CheckCollision(Bullet *theBullet)
{
    Bullet *aBullet = theBullet;
    Ball *ball;
    bool flag;

    for (BulletList::iterator anItr = mBulletList.begin(); anItr != mBulletList.end(); anItr++)
    {
        aBullet = *anItr;

        if (theBullet->CollidesWithPhysically(aBullet))
        {
            aBullet->Update();
            AdvanceMergingBullet(anItr);
            break;
        }
    }

    BallList::iterator aBallItr;
    for (aBallItr = mBallList.begin();; ++aBallItr)
    {
        if (aBallItr == mBallList.end())
        {
            return false;
        }

        ball = *aBallItr;

        if (ball->CollidesWithPhysically(theBullet) && ball->GetBullet() == NULL && ball->GetClearCount() == 0)
        {
            Ball *aPrevBall = ball->GetPrevBall(true);
            if (aPrevBall == NULL || aPrevBall->GetBullet() == NULL)
            {
                Ball *aNextBall = ball->GetNextBall(true);
                if (aNextBall == NULL || aNextBall->GetBullet() == NULL)
                {
                    SexyVector3 v(ball->GetX(), ball->GetY(), 0.0f);
                    SexyVector3 impliedObject(theBullet->GetX(), theBullet->GetY(), 0.0f);
                    SexyVector3 v2 = mWayPointMgr->CalcPerpendicular(ball->GetWayPoint());

                    flag = (impliedObject - v).Cross(v2).z < 0.0f;
                    if (!mWayPointMgr->InTunnel(ball, flag))
                    {
                        break;
                    }
                }
            }
        }
    }

    if (aBallItr != mBallList.end())
    {
        theBullet->SetHitBall(ball, flag);
        theBullet->SetMergeSpeed(mCurveDesc->mMergeSpeed);

        Ball *nextBall2 = ball->GetNextBall(false);
        if (!flag)
        {
            theBullet->RemoveGapInfoForBall(ball->GetId());
        }
        else if (nextBall2 != NULL)
        {
            theBullet->RemoveGapInfoForBall(nextBall2->GetId());
        }

        mApp->PlaySample(Sexy::SOUND_BALLCLICK2);
        mBulletList.push_back(theBullet);

        return true;
    }
    return false;
}

bool CurveMgr::CheckGapShot(Bullet *theBullet)
{
    int aBulRadius = theBullet->GetRadius();
    int aBulDiameter = aBulRadius * 2;
    float aBulDiameterSq = (float)aBulDiameter * (float)aBulDiameter;
    float aBulX = theBullet->GetX();
    float aBulY = theBullet->GetY();
    int aNumWayPoints = (int)mWayPointMgr->GetNumPoints();
    int aBallIdx = theBullet->GetCurCurvePoint(mCurveNum);

    if (aBallIdx > 0 && aBallIdx < aNumWayPoints)
    {
        const WayPoint *aWayPoint = &mWayPointMgr->GetWayPointList()[aBallIdx];
        if (aBulDiameterSq > ((aWayPoint->y - aBulY) * (aWayPoint->y - aBulY) +
                              (aWayPoint->x - aBulX) * (aWayPoint->x - aBulX)))
        {
            return false;
        }

        theBullet->SetCurCurvePoint(mCurveNum, 0);
    }

    for (int i = 1; i < aNumWayPoints; i += aBulDiameter)
    {
        const WayPoint *aWayPoint = &mWayPointMgr->GetWayPointList()[i];
        if (!aWayPoint->mInTunnel && (aBulDiameterSq > (aWayPoint->y - aBulY) * (aWayPoint->y - aBulY) +
                                                           (aWayPoint->x - aBulX) * (aWayPoint->x - aBulX)))
        {
            theBullet->SetCurCurvePoint(mCurveNum, i);

            for (BallList::iterator aBallItr = mBallList.begin(); aBallItr != mBallList.end(); ++aBallItr)
            {
                Ball *aBall = *aBallItr;

                if (aBall->GetWayPoint() > i)
                {
                    Ball *aPrevBall = aBall->GetPrevBall();
                    if (aPrevBall == NULL)
                    {
                        return false;
                    }

                    int aBallDist = aBall->GetWayPoint() - aPrevBall->GetWayPoint();
                    if (aBallDist <= 0)
                    {
                        return false;
                    }

                    return theBullet->AddGapInfo(mCurveNum, aBallDist, aBall->GetId());
                }
            }

            return false;
        }
    }

    return false;
}

int CurveMgr::GetRandomPendingBallColor(int theMaxNumBalls)
{
    return Sexy::AppRand() % theMaxNumBalls;
}

bool CurveMgr::HasPendingBallOfType(int theType, int theMaxNumBalls)
{
    int aCount = 0;
    for (BallList::iterator aBallItr = mPendingBalls.begin(); aBallItr != mPendingBalls.end(); ++aBallItr)
    {
        if (aCount >= theMaxNumBalls)
        {
            break;
        }

        Ball *aBall = *aBallItr;
        if (aBall->GetType() == theType)
        {
            return true;
        }

        aCount++;
    }

    return false;
}

bool CurveMgr::IsLosing()
{
    if (mHaveSets ||
        mBallList.empty() ||
        mWayPointMgr->GetEndPoint() > mBallList.back()->GetWayPoint() ||
        !mBulletList.empty() ||
        mBackwardCount > 0)
    {
        return false;
    }

    Ball *aBall = mBallList.back();
    while (aBall != NULL)
    {
        if (aBall->GetSuckCount() > 0)
            return false;

        aBall = aBall->GetPrevBall(true);
    }

    return true;
}

bool CurveMgr::IsWinning()
{
    bool is_empty = mBallList.empty() && mPendingBalls.empty();

    return is_empty;
}

bool CurveMgr::CanRestart()
{
    return mBallList.empty();
}

bool CurveMgr::CanFire()
{
    if (mBallList.empty())
        return true;

    return mBallList.back()->GetWayPoint() < mWayPointMgr->GetEndPoint();
}

Ball *CurveMgr::CheckBallIntersection(const SexyVector3 &p1, const SexyVector3 &v1, float &t)
{
    BallList::iterator aBallItr = mBallList.begin();
    Ball *anIntersectBall = NULL;

    while (aBallItr != mBallList.end())
    {
        Ball *aBall = *aBallItr;

        if (!mWayPointMgr->InTunnel(aBall->GetWayPoint()))
        {
            float t2;
            if (aBall->Intersects(p1, v1, t2))
            {
                if (t2 < t && t2 > 0.0f)
                {
                    t = t2;
                    anIntersectBall = aBall;
                }
            }
        }

        ++aBallItr;
    }

    return anIntersectBall;
}

bool gGotPowerUp[(int)PowerType_Max] = {false, false, false, false};

void CurveMgr::ActivatePower(Ball *theBall)
{
    PowerType aPowerType = theBall->GetPowerTypeWussy();
    gGotPowerUp[aPowerType] = true;

    if (aPowerType == PowerType_Bomb)
    {
        ActivateBomb(theBall);
    }
    else if (aPowerType == PowerType_MoveBackwards)
    {
        if (!mBallList.empty())
        {
            mBackwardCount = 300;
        }
    }
    else if (aPowerType == PowerType_SlowDown)
    {
        if (mSlowCount < 1000)
        {
            mSlowCount = 800;
        }
    }
}

int gBoardColor;
int gCurveColor;

void CurveMgr::DrawCurve(CurveDrawer &theDrawer)
{
    mWayPointMgr->DrawCurve(theDrawer, Color(gCurveColor), mDangerPoint);
}

void CurveMgr::DrawTunnel(CurveDrawer &theDrawer)
{
    mWayPointMgr->DrawTunnel(theDrawer);
}

void CurveMgr::DeleteBalls()
{
    for (BulletList::iterator aBulletItr = mBulletList.begin(); aBulletItr != mBulletList.end(); aBulletItr++)
    {
        Bullet *aBullet = *aBulletItr;
        delete aBullet;
    }

    for (BallList::iterator aBallItr = mBallList.begin(); aBallItr != mBallList.end(); aBallItr++)
    {
        Ball *aBall = *aBallItr;
        delete aBall;
    }

    for (BallList::iterator aBallItr = mPendingBalls.begin(); aBallItr != mPendingBalls.end(); aBallItr++)
    {
        Ball *aBall = *aBallItr;
        delete aBall;
    }

    mBallList.clear();
    mPendingBalls.clear();
    mBulletList.clear();
}

void CurveMgr::GetPoint(int thePoint, int &x, int &y, int &pri)
{
    if (thePoint < 0)
        thePoint = 0;

    if (thePoint >= mWayPointMgr->GetNumPoints())
        thePoint = mWayPointMgr->GetEndPoint();

    const WayPoint *aWayPoint = &mWayPointMgr->GetWayPointList()[thePoint];

    x = aWayPoint->x;
    y = aWayPoint->y;
    pri = aWayPoint->mPriority;
}

int CurveMgr::GetCurveLength()
{
    return mWayPointMgr->GetNumPoints();
}

int CurveMgr::GetTotalBalls()
{
    if (mCurveDesc->mNumBalls == 0)
    {
        return 0;
    }

    return mTotalBalls;
}

void CurveMgr::SetStopAddingBalls(bool stop)
{
    if (mStopAddingBalls == stop)
        return;

    if (GetFarthestBallPercent() > 50)
    {
        mBackwardCount = mCurveDesc->mZumaBack;
        mSlowCount = mCurveDesc->mZumaSlow;
    }

    mStopAddingBalls = stop;
    if (stop)
    {
        for (BallList::iterator aBallItr = mPendingBalls.begin();
             aBallItr != mPendingBalls.end();
             aBallItr++)
            delete *aBallItr;

        mPendingBalls.clear();
    }
}

void CurveMgr::DoEndlessZumaEffect()
{
    if (GetFarthestBallPercent() > 50)
    {
        mBackwardCount = mCurveDesc->mZumaBack;
    }
}

void CurveMgr::DetonateBalls()
{
    for (BallList::iterator aBallItr = mBallList.begin(); aBallItr != mBallList.end(); ++aBallItr)
    {
        Ball *aBall = *aBallItr;
        if (aBall->GetClearCount() == 0)
        {
            aBall->StartClearCount(true);
            mBoard->UpdateBallColorMap(aBall, false);
        }
    }
}

int CurveMgr::GetFarthestBallPercent()
{
    if (mBallList.empty())
        return 0;

    float aWayPoint = mBallList.back()->GetWayPoint();

    return (int)(aWayPoint * 100.0f / (float)mWayPointMgr->GetNumPoints());
}

int CurveMgr::DrawPathSparkles(int theStartPoint, int theStagger, bool addSound)
{
    int aPathHiliteWP = theStartPoint;
    bool forwardPitch = ((mCurveNum ^ 1) & 1) != 0;
    int aPathHilitePitch = forwardPitch ? 0 : -20;
    int aSoundCtr = 0;

    while (aPathHiliteWP < mWayPointMgr->GetNumPoints())
    {
        int aSparkleX, aSparkleY, aSparklePriority;

        GetPoint(aPathHiliteWP, aSparkleX, aSparkleY, aSparklePriority);
        mBoard->mParticleMgr->AddSparkle(aSparkleX, aSparkleY, 0, 0, aSparklePriority, 0, theStagger, 0xFFFF00);

        if (addSound && (aSoundCtr % 25) == 0)
        {
            if (forwardPitch)
            {
                if (aPathHilitePitch > -20)
                    aPathHilitePitch--;
            }
            else
            {
                if (aPathHilitePitch < 0)
                    aPathHilitePitch++;
            }

            mBoard->mSoundMgr->AddSound(Sexy::SOUND_TRAIL_LIGHT, theStagger, 0, aPathHilitePitch * 0.8f);
        }

        aPathHiliteWP += 11;
        ++theStagger;
        ++aSoundCtr;
    }

    if (addSound)
        mBoard->mSoundMgr->AddSound(Sexy::SOUND_TRAIL_LIGHT_END, theStagger);

    mSpriteMgr->AddHoleFlash(mCurveNum, theStagger);

    return theStagger + 60;
}

int CurveMgr::DrawEndLevelBonus(int theStagger)
{
    FloatingTextHelper aFloat;

    int aNumPoints = mWayPointMgr->GetNumPoints();
    int aPoint = mLastClearedBallPoint + (aNumPoints - mLastClearedBallPoint) % 60;
    int aStagger = theStagger;

    while (aPoint <= aNumPoints)
    {
        int aExplodeX, aExplodeY, aExplodePriority;
        GetPoint(aPoint, aExplodeX, aExplodeY, aExplodePriority);
        for (int i = 0; i < 5; i++)
        {
            int ox = Sexy::AppRand() % 21 - 10;
            int oy = Sexy::AppRand() % 21 - 10;

            mBoard->mParticleMgr->AddExplosion(
                aExplodeX + ox,
                aExplodeY + oy,
                0,
                0,
                aStagger);
        }

        aFloat.Clear();
        aFloat.AddText(Sexy::StrFormat("+%d", 100), Sexy::FONT_FLOAT_ID, 0xFFFF00);
        aFloat.AddToMgr(mBoard->mParticleMgr, aExplodeX, aExplodeY, aStagger + 10, 100);

        aPoint += 60;
        aStagger += 4;
    }

    for (int i = theStagger + 1; i < aStagger; i += 10)
    {
        mBoard->mSoundMgr->AddSound(Sexy::SOUND_BONUS_EXPLOSION, i);
    }

    return aStagger;
}

bool CurveMgr::HasReachedCruisingSpeed()
{
    return (mAdvanceSpeed - mCurveDesc->mSpeed) < 0.1f;
}

void CurveMgr::AddPowerUp(PowerType thePower)
{
    int aBallIdx = Sexy::AppRand() % mBallList.size();
    BallList::iterator aBallItr = mBallList.begin();
    for (int i = 0; i < aBallIdx; ++i)
    {
        ++aBallItr;
    }

    Ball *b = *aBallItr;

    if (b->GetPowerType() == PowerType_None && b->GetDestPowerType() == PowerType_None)
        b->SetPowerType(thePower);
}

void CurveMgr::SyncState(DataSync &theSync)
{
    DataReader *aReader = theSync.mReader;
    DataWriter *aWriter = theSync.mWriter;

    if (aReader)
    {
        DeleteBalls();

        int aBulCount = aReader->ReadShort();
        for (int i = 0; i < aBulCount; i++)
        {
            Bullet *aBullet = new Bullet();
            aBullet->SyncState(theSync);
            mBulletList.push_back(aBullet);
        }

        int aPendingBallCount = aReader->ReadShort();
        for (int i = 0; i < aPendingBallCount; i++)
        {
            Ball *aBall = new Ball();
            aBall->SyncState(theSync);
            mPendingBalls.push_back(aBall);
        }

        int aBallCount = aReader->ReadShort();
        for (int i = 0; i < aBallCount; i++)
        {
            Ball *aBall = new Ball();
            aBall->SyncState(theSync);
            aBall->InsertInList(mBallList, mBallList.end());
        }
    }
    else
    {
        aWriter->WriteShort((short)mBulletList.size());
        for (BulletList::iterator aBulletItr = mBulletList.begin(); aBulletItr != mBulletList.end(); aBulletItr++)
        {
            Bullet *aBullet = *aBulletItr;
            aBullet->SyncState(theSync);
        }

        aWriter->WriteShort((short)mPendingBalls.size());
        for (BallList::iterator aBallItr = mPendingBalls.begin(); aBallItr != mPendingBalls.end(); aBallItr++)
        {
            Ball *aBall = *aBallItr;
            aBall->SyncState(theSync);
        }

        aWriter->WriteShort((short)mBallList.size());
        for (BallList::iterator aBallItr = mBallList.begin(); aBallItr != mBallList.end(); aBallItr++)
        {
            Ball *aBall = *aBallItr;
            aBall->SyncState(theSync);
        }
    }

    for (int i = 0; i < (int)PowerType_Max; i++)
    {
        theSync.SyncLong(mLastPowerUpFrame[i]);
    }

    theSync.SyncLong(mStopTime);
    theSync.SyncLong(mSlowCount);
    theSync.SyncLong(mBackwardCount);
    theSync.SyncLong(mTotalBalls);
    theSync.SyncFloat(mAdvanceSpeed);
    theSync.SyncShort(mFirstChainEnd);
    theSync.SyncBool(mFirstBallMovedBackwards);
    theSync.SyncBool(mHaveSets);
    theSync.SyncLong(mPathLightEndFrame);
    theSync.SyncBool(mHadPowerUp);
    theSync.SyncLong(mLastPathShowTick);
    theSync.SyncLong(mLastClearedBallPoint);
    theSync.SyncBool(mStopAddingBalls);
    theSync.SyncBool(mInDanger);
}

void CurveMgr::DeleteBullet(Bullet *theBullet)
{
    if (theBullet == NULL)
        return;

    BulletList::iterator anItr = std::find(mBulletList.begin(), mBulletList.end(), theBullet);

    if (anItr != mBulletList.end())
    {
        mBulletList.erase(anItr);
    }

    delete theBullet;
}

void CurveMgr::DeleteBall(Ball *theBall)
{
    Bullet *aBullet = theBall->GetBullet();
    if (aBullet != NULL)
    {
        aBullet->MergeFully();
        BulletList::iterator anItr = std::find(mBulletList.begin(), mBulletList.end(), aBullet);
        if (anItr != mBulletList.end())
            AdvanceMergingBullet(anItr);
    }

    DeleteBullet(aBullet);
    theBall->SetCollidesWithPrev(false);
    delete theBall;
}

void CurveMgr::SetFarthestBall(int thePoint)
{
    int aLastPoint = mDangerPoint;
    if (aLastPoint < 0)
        aLastPoint = 0;

    float aPercentOpen = 0.0f;
    if (aLastPoint <= thePoint)
        aPercentOpen = (float)(thePoint - aLastPoint) / (float)(mWayPointMgr->GetNumPoints() - aLastPoint);

    mSpriteMgr->UpdateHole(mCurveNum, aPercentOpen);
}

int CurveMgr::GetNumInARow(Ball *theBall, int theColor, Ball **theNextEnd, Ball **thePrevEnd)
{
    if (theBall->GetType() != theColor)
        return 0;

    Ball *aBall = theBall;
    int aColor = theColor;
    int aCount = 1;

    Ball *aNextEnd = aBall;
    while (true)
    {
        Ball *aNextBall = aNextEnd->GetNextBall(true);
        if (aNextBall == NULL || aNextBall->GetType() != aColor)
            break;

        aNextEnd = aNextBall;
        aCount++;
    }

    Ball *aPrevEnd = aBall;
    while (true)
    {
        Ball *aPrevBall = aPrevEnd->GetPrevBall(true);
        if (aPrevBall == NULL || aPrevBall->GetType() != aColor)
            break;

        aPrevEnd = aPrevBall;
        aCount++;
    }

    if (theNextEnd != NULL)
        *theNextEnd = aNextEnd;
    if (thePrevEnd != NULL)
        *thePrevEnd = aPrevEnd;

    return aCount;
}

bool CurveMgr::CheckSet(Ball *theBall)
{
    mHadPowerUp = false;
    Ball *aPrevEnd = NULL;
    Ball *aNextEnd = NULL;
    int aComboCount = theBall->GetComboCount();

    int aCount = GetNumInARow(theBall, theBall->GetType(), &aNextEnd, &aPrevEnd);

    if (aCount < 3)
    {
        return false;
    }

    mBoard->mNumCleared = 0;
    mBoard->mClearedXSum = 0;
    mBoard->mClearedYSum = 0;
    mBoard->mCurComboCount = aComboCount;
    mBoard->mCurComboScore = theBall->GetComboScore();
    mBoard->mNeedComboCount.clear();

    for (int i = 0; i < PowerType_Max; i++)
        gGotPowerUp[i] = false;

    int aGapBonus = 0;
    int aNumGaps = 0;
    Ball *anEndBall = aNextEnd->GetNextBall();
    Ball *aBall = aPrevEnd;

    while (aBall != anEndBall)
    {
        if (aBall->GetSuckPending())
        {
            aBall->SetSuckPending(false);
            mBoard->mNumClearsInARow++;
        }

        StartClearCount(aBall);
        aGapBonus += aBall->GetGapBonus();
        if (aBall->GetNumGaps() > aNumGaps)
            aNumGaps = aBall->GetNumGaps();

        aBall->SetGapBonus(0, 0);
        aBall = aBall->GetNextBall();
    }

    DoScoring(theBall, mBoard->mNumCleared, aComboCount, aGapBonus, aNumGaps);

    if (mBoard->mCurComboCount > mBoard->mLevelStats.mMaxCombo ||
        mBoard->mCurComboCount == mBoard->mLevelStats.mMaxCombo &&
            mBoard->mCurComboScore >= mBoard->mLevelStats.mMaxComboScore)
    {
        mBoard->mLevelStats.mMaxCombo = mBoard->mCurComboCount;
        mBoard->mLevelStats.mMaxComboScore = mBoard->mCurComboScore;
    }

    aBall = aPrevEnd;
    while (aBall != anEndBall)
    {
        aBall->SetComboCount(aComboCount, mBoard->mCurComboScore);
        aBall = aBall->GetNextBall();
    }

    BallList::iterator anItr = mBoard->mNeedComboCount.begin();
    for (;
         anItr != mBoard->mNeedComboCount.end();
         anItr++)
    {
        Ball *aBall = *anItr;
        aBall->SetComboCount(aComboCount, mBoard->mCurComboScore);
    }

    mBoard->mNeedComboCount.clear();

    if (!mHadPowerUp)
    {
        int *destroySound = &Sexy::SOUND_BALLDESTROYED1;

        switch (aComboCount)
        {
        case 0:
            destroySound = &Sexy::SOUND_BALLDESTROYED1;
            break;
        case 1:
            destroySound = &Sexy::SOUND_BALLDESTROYED2;
            break;
        case 2:
            destroySound = &Sexy::SOUND_BALLDESTROYED3;
            break;
        case 3:
            destroySound = &Sexy::SOUND_BALLDESTROYED4;
            break;
        default:
            destroySound = &Sexy::SOUND_BALLDESTROYED5;
            break;
        }

        mApp->PlaySample(*destroySound);

        SoundInstance *aSound = mApp->mSoundManager->GetSoundInstance(Sexy::SOUND_COMBO);
        if (aSound != NULL)
        {
            aSound->AdjustPitch(2 * aComboCount);
            aSound->SetVolume(min(1.0, aComboCount * 0.2 + 0.4));
            aSound->Play(false, true);
        }
    }

    mBoard->mCurComboCount = 0;
    mBoard->mCurComboScore = 0;

    return true;
}

void CurveMgr::DoScoring(Ball *theBall, int theNumBalls, int theComboCount, int theGapBonus, int theNumGaps)
{
    if (theNumBalls == 0)
    {
        return;
    }

    FloatingTextHelper aFloat;
    int aNumPoints = 100 * theComboCount + 10 * theNumBalls + theGapBonus;
    bool inARow = false;
    int aRowBonus = 0;

    if (mBoard->mNumClearsInARow > 4 && theComboCount == 0)
    {
        aRowBonus = 10 * mBoard->mNumClearsInARow + 50;
        aNumPoints += aRowBonus;
        mBoard->mCurInARowBonus += aRowBonus;
        inARow = true;
    }

    mBoard->mCurComboScore += aNumPoints;
    mBoard->IncScore(aNumPoints);

    if (theComboCount > 0)
        ++mBoard->mLevelStats.mNumCombos;

    if (theGapBonus)
        ++mBoard->mLevelStats.mNumGaps;

    int theColor = gTextBallColors[theBall->GetType()];
    aFloat.AddText(Sexy::StrFormat("+%d", aNumPoints), Sexy::FONT_FLOAT_ID, theColor);

    if (theComboCount > 0)
    {
        aFloat.AddText(Sexy::StrFormat("COMBO x%d", theComboCount + 1), Sexy::FONT_FLOAT_ID, theColor);
    }

    if (theGapBonus > 0)
    {
        std::string scoreString;

        if (theNumGaps > 1)
        {
            mBoard->mSoundMgr->AddSound(Sexy::SOUND_GAP_BONUS, 15, 0, 2.0);

            if (theNumGaps > 2)
            {
                scoreString = "TRIPLE GAP BONUS";
            }
            else
            {
                mBoard->mSoundMgr->AddSound(Sexy::SOUND_GAP_BONUS, 30, 0, 3.0);
                scoreString = "DOUBLE GAP BONUS";
            }
        }
        else
        {
            scoreString = "GAP BONUS";
        }

        aFloat.AddText(scoreString, Sexy::FONT_FLOAT_ID, theColor);
        mBoard->mApp->PlaySample(Sexy::SOUND_GAP_BONUS);
    }

    if (inARow)
    {
        aFloat.AddText(Sexy::StrFormat("CHAIN BONUS x%d", mBoard->mNumClearsInARow), Sexy::FONT_FLOAT_ID, theColor);
        mBoard->mSoundMgr->AddSound(Sexy::SOUND_CHAIN_BONUS, 0, 0, mBoard->mNumClearsInARow - 5);
    }

    int aClrX = mBoard->mClearedXSum / theNumBalls;
    int aClrY = mBoard->mClearedYSum / theNumBalls;

    if (gGotPowerUp[PowerType_SlowDown])
    {
        aFloat.AddText("SLOWDOWN Ball", Sexy::FONT_FLOAT_ID, theColor);
    }

    if (gGotPowerUp[PowerType_MoveBackwards])
    {
        aFloat.AddText("BACKWARDS Ball", Sexy::FONT_FLOAT_ID, theColor);
    }

    if (gGotPowerUp[PowerType_Accuracy])
    {
        aFloat.AddText("ACCURACY Ball", Sexy::FONT_FLOAT_ID, theColor);
    }

    aFloat.AddToMgr(mBoard->mParticleMgr, aClrX, aClrY);
}

static void GetNumPendingSinglesHelper(int aColor, int &aNumGroups, int &aPrevColor, int &aNumSingles, int &aGroupCount)
{
    if (aColor == aPrevColor)
    {
        ++aGroupCount;
    }
    else
    {
        if (aGroupCount == 1)
            ++aNumSingles;
        aGroupCount = 1;
        ++aNumGroups;
        aPrevColor = aColor;
    }
}

int CurveMgr::GetNumPendingSingles(int theNumGroups)
{
    int aNumGroups = 0;
    int aPrevColor = -1;
    int aNumSingles = 0;
    int aGroupCount = 0;

    BallList::reverse_iterator anRItr = mPendingBalls.rbegin();
    while (anRItr != mPendingBalls.rend())
    {
        Ball *aBall = *anRItr;
        if (aNumGroups > theNumGroups)
            break;

        GetNumPendingSinglesHelper(aBall->GetType(), aNumGroups, aPrevColor, aNumSingles, aGroupCount);

        anRItr++;
    }

    for (BallList::iterator anItr = mPendingBalls.begin(); anItr != mPendingBalls.end(); ++anItr)
    {
        Ball *aBall = *anItr;

        GetNumPendingSinglesHelper(aBall->GetType(), aNumGroups, aPrevColor, aNumSingles, aGroupCount);
    }

    return aNumSingles;
}

void CurveMgr::AddPendingBall()
{
    int aNewColor = 0;
    int aPrevColor = 0;
    int aNumColors = mCurveDesc->mNumColors;
    Ball *aBall = new Ball();
    aBall->RandomizeFrame();

    if (gColorOverride)
        aNumColors = gNumColors;

    if (!mPendingBalls.empty())
    {
        aPrevColor = mPendingBalls.back()->GetType();
    }
    else if (!mBallList.empty())
    {
        aPrevColor = mBallList.front()->GetType();
    }
    else
    {
        aPrevColor = GetRandomPendingBallColor(aNumColors);
    }

    if (aPrevColor >= aNumColors)
    {
        aPrevColor = GetRandomPendingBallColor(aNumColors);
    }

    int aMaxSingle = mCurveDesc->mMaxSingle;
    if (Sexy::AppRand() % 100 <= mCurveDesc->mBallRepeat)
    {
        aNewColor = aPrevColor;
    }
    else if (aMaxSingle < 10 && GetNumPendingSingles(1) == 1 && (aMaxSingle == 0 || GetNumPendingSingles(10) > aMaxSingle))
    {
        aNewColor = aPrevColor;
    }
    else
    {
        do
        {
            aNewColor = GetRandomPendingBallColor(aNumColors);
        } while (aNewColor == aPrevColor);
    }

    aBall->SetType(aNewColor);
    mPendingBalls.push_back(aBall);
}

void CurveMgr::AddBall()
{
    if (mPendingBalls.empty())
    {
        if (mCurveDesc->mNumBalls != 0 || mStopAddingBalls)
            return;

        AddPendingBall();
    }

    Ball *aBall = mPendingBalls.front();
    mWayPointMgr->SetWayPoint(aBall, 1.0f);

    if (!mBallList.empty())
    {
        Ball *aFrontBall = mBallList.front();
        if (aBall->GetWayPoint() > aFrontBall->GetWayPoint() || aFrontBall->CollidesWith(aBall))
        {
            return;
        }
    }

    mBoard->UpdateBallColorMap(aBall, true);

    aBall->InsertInList(mBallList, mBallList.begin());
    aBall->UpdateCollisionInfo(5 + mAdvanceSpeed);
    aBall->SetNeedCheckCollision(true);
    aBall->SetRotation(mWayPointMgr->GetRotationForPoint(aBall->GetWayPoint()));
    aBall->SetBackwardsCount(0);
    aBall->SetSuckCount(0);
    aBall->SetGapBonus(0, 0);
    aBall->SetComboCount(0, 0);

    mPendingBalls.pop_front();
}

void CurveMgr::UpdateBallRotation()
{
    for (BallList::iterator anItr = mBallList.begin(); anItr != mBallList.end(); ++anItr)
    {
        Ball *aBall = *anItr;
        aBall->UpdateRotation();
    }

    for (BulletList::iterator anItr = mBulletList.begin(); anItr != mBulletList.end(); ++anItr)
    {
        Bullet *aBullet = *anItr;
        aBullet->UpdateRotation();
    }
}

void CurveMgr::AdvanceBalls()
{
    if (mBallList.empty())
        return;

    float aMaxSpeed = mCurveDesc->mSpeed;
    if (mCurveDesc->mAccelerationRate != 0.0f)
    {
        mCurveDesc->mCurAcceleration += mCurveDesc->mAccelerationRate;
        aMaxSpeed += mCurveDesc->mCurAcceleration;

        if (aMaxSpeed > mCurveDesc->mMaxSpeed)
        {
            aMaxSpeed = mCurveDesc->mMaxSpeed;
        }
    }

    if (mSlowCount != 0)
    {
        aMaxSpeed /= 4.0f;
    }

    if (mFirstChainEnd >= mDangerPoint - mCurveDesc->mSlowDistance)
    {
        if (mFirstChainEnd < mDangerPoint)
        {
            float aDist = (mFirstChainEnd - (mDangerPoint - mCurveDesc->mSlowDistance)) / float(mCurveDesc->mSlowDistance);
            aMaxSpeed = (1 - aDist) * aMaxSpeed + aDist * aMaxSpeed / float(mCurveDesc->mSlowFactor);
        }
        else
        {
            aMaxSpeed /= mCurveDesc->mSlowFactor;
        }
    }

    if (mBoard->mIsEndless)
    {
        if (mBoard->mStateCount > 300)
        {
            int farthestBallPercent = GetFarthestBallPercent();
            int gauntletHurryDist = 30;
            if (farthestBallPercent < gauntletHurryDist)
            {
                aMaxSpeed = ((aMaxSpeed + (gauntletHurryDist - farthestBallPercent) * aMaxSpeed) * 3.0f) / (float)gauntletHurryDist;

                if (aMaxSpeed > mAdvanceSpeed)
                {
                    mAdvanceSpeed += 0.03f;
                }
            }
        }
    }

    if (mAdvanceSpeed > aMaxSpeed)
    {
        mAdvanceSpeed -= 0.1f;
    }

    if (mAdvanceSpeed < aMaxSpeed)
    {
        mAdvanceSpeed += 0.005f;

        if (mAdvanceSpeed >= aMaxSpeed)
        {
            mAdvanceSpeed = aMaxSpeed;
        }
    }

    Ball *aBall = mBallList.front();
    float aNextWayPoint = aBall->GetWayPoint();
    if (!mFirstBallMovedBackwards && !mStopTime)
    {
        mWayPointMgr->SetWayPoint(aBall, aNextWayPoint + mAdvanceSpeed);
    }

    BallList::iterator anItr = mBallList.begin();
    Ball *aFirstChainEnd = NULL;

    while (anItr != mBallList.end())
    {
        aBall = *anItr++;

        if (anItr == mBallList.end())
        {
            break;
        }

        Ball *aNextBall = *anItr;
        aNextWayPoint = aNextBall->GetWayPoint();
        float aWayPoint = aBall->GetWayPoint();

        if (aWayPoint > aNextWayPoint - aBall->GetRadius() - aNextBall->GetRadius())
        {
            mWayPointMgr->SetWayPoint(aNextBall, aWayPoint + aBall->GetRadius() + aNextBall->GetRadius());

            if (!aBall->GetCollidesWithNext())
            {
                aBall->SetCollidesWithNext(true);
                mBoard->PlayBallClick(Sexy::SOUND_BALLCLICK1);
            }

            aBall->SetNeedCheckCollision(false);
        }

        if (aFirstChainEnd == NULL)
        {
            if (!aBall->GetCollidesWithNext())
                aFirstChainEnd = aBall;
        }
    }

    if (aFirstChainEnd == NULL)
    {
        aFirstChainEnd = mBallList.back();
    }

    mFirstChainEnd = aFirstChainEnd->GetWayPoint();

    if (mFirstChainEnd >= mDangerPoint)
    {
        int aTick = Sexy::BoardGetTickCount();
        int aMaxTime = 100 + 4000 * (GetCurveLength() - mFirstChainEnd) / (GetCurveLength() - mDangerPoint);
        int aFrame = mBoard->GetStateCount();

        if (aFrame >= mPathLightEndFrame && aTick - mLastPathShowTick >= aMaxTime)
        {
            mApp->PlaySample(Sexy::SOUND_WARNING);
            mLastPathShowTick = aTick;
            mPathLightEndFrame = aFrame + DrawPathSparkles(mFirstChainEnd, 0, false);
        }
    }

    mInDanger = mBallList.back()->GetWayPoint() >= mDangerPoint;
}

void CurveMgr::AdvanceBackwardBalls()
{
    mFirstBallMovedBackwards = false;

    if (mBallList.empty())
        return;

    BallList::reverse_iterator anItr = mBallList.rbegin();
    bool aCollided = false;
    float aBackwardsSpeed = 0.0f;

    if (mBackwardCount != 0)
    {
        mBallList.back()->SetBackwardsSpeed(1.0f);
        mBallList.back()->SetBackwardsCount(1);
    }

    for (;;)
    {
        Ball *aBall = *anItr;
        int aBackwardsCount = aBall->GetBackwardsCount();

        if (aBackwardsCount > 0)
        {
            aBackwardsSpeed = aBall->GetBackwardsSpeed();
            mWayPointMgr->SetWayPoint(aBall, aBall->GetWayPoint() - aBackwardsSpeed);
            aBall->SetBackwardsCount(aBackwardsCount - 1);
            aCollided = true;
        }

        anItr++;
        if (anItr == mBallList.rend())
        {
            break;
        }

        Ball *aNextBall = *anItr;

        if (aCollided)
        {
            if (aNextBall->GetCollidesWithNext())
            {
                float aPoint = aNextBall->GetWayPoint() - aBackwardsSpeed;
                mWayPointMgr->SetWayPoint(aNextBall, aPoint);
            }
            else
            {
                const float aWayOffNext = aBall->GetWayPoint() - (float)aBall->GetRadius() - (float)aNextBall->GetRadius();

                if (aNextBall->GetWayPoint() > aWayOffNext)
                {
                    aNextBall->SetCollidesWithNext(true);
                    aCollided = true;
                    mBoard->PlayBallClick(Sexy::SOUND_BALLCLICK1);
                    aBackwardsSpeed = aNextBall->GetWayPoint() - aWayOffNext;
                    aNextBall->SetWayPoint(aWayOffNext);
                }
                else
                {
                    aCollided = false;
                }
            }
        }
    }

    if (aCollided)
    {
        mFirstBallMovedBackwards = true;
        if (mStopTime < 20)
        {
            mStopTime = 20;
        }
    }
}

void CurveMgr::UpdateSuckingBalls()
{
    BallList::iterator anItr = mBallList.begin();
    while (anItr != mBallList.end())
    {
        Ball *aBall = *anItr;
        int aSuckCount = aBall->GetSuckCount();

        if (aSuckCount <= 0)
        {
            ++anItr;
            continue;
        }

        Ball *aNextBall = NULL;
        float aSuck = aSuckCount / 8;
        while (anItr != mBallList.end())
        {

            aNextBall = *anItr++;
            aNextBall->SetSuckCount(0);
            mWayPointMgr->SetWayPoint(aNextBall, aNextBall->GetWayPoint() - aSuck);

            Bullet *aBullet = aNextBall->GetBullet();
            if (aBullet != NULL)
            {
                Ball *aPushBall = aBullet->GetPushBall();
                if (aPushBall != NULL)
                {
                    mWayPointMgr->FindFreeWayPoint(aPushBall, aBullet, false);
                }

                aBullet->UpdateHitPos();
            }

            if (!aNextBall->GetCollidesWithNext())
                break;
        }

        aBall->SetSuckCount(aSuckCount + 1);
        Ball *aPrevBall = aBall->GetPrevBall(false);

        if (aPrevBall == NULL)
        {
            aBall->SetSuckCount(0);
            continue;
        }

        float aNewWayPoint = (aBall->GetWayPoint() - aBall->GetRadius()) - aPrevBall->GetRadius();
        if (aPrevBall->GetWayPoint() > aNewWayPoint)
        {
            mWayPointMgr->SetWayPoint(aPrevBall, aNewWayPoint);
            mBoard->PlayBallClick(SOUND_BALLCLICK1);
            aPrevBall->SetCollidesWithNext(true);
            aBall->SetSuckCount(0);
            if (!CheckSet(aBall))
            {
                aBall->SetComboCount(0, 0);
            }

            if (aNextBall->GetBackwardsCount() == 0)
            {
                aNextBall->SetBackwardsCount(30);
                float aBackwardsSpeed = aBall->GetComboCount() * 1.5f;

                if (aBackwardsSpeed <= 0.5f)
                {
                    aBackwardsSpeed = 0.5f;
                }

                aNextBall->SetBackwardsSpeed(aBackwardsSpeed);
            }

            ClearPendingSucks(aNextBall);
        }
    }
}

void CurveMgr::UpdateSets()
{
    mHaveSets = false;
    BallList::iterator anItr = mBallList.begin();
    while (anItr != mBallList.end())
    {
        Ball *aBall = *anItr;
        int aClearCount = aBall->GetClearCount();
        if (aClearCount > 0)
        {
            mHaveSets = true;
        }

        if (aClearCount < 40)
        {
            if (aClearCount > 0)
            {
                aBall->SetClearCount(aClearCount + 1);
            }
            anItr++;
        }
        else
        {
            Ball *aNextBall = aBall->GetNextBall();
            Ball *aPrevBall = aBall->GetPrevBall();

            if (aNextBall != NULL && aNextBall->GetClearCount() == 0 && aPrevBall != NULL && aNextBall->GetType() == aPrevBall->GetType())
            {
                aNextBall->SetSuckCount(10);
                aNextBall->SetComboCount(aBall->GetComboCount() + 1, aBall->GetComboScore());
            }

            if (anItr == mBallList.begin())
            {
                mAdvanceSpeed = 0.0f;
                if (mStopTime < 40)
                {
                    mStopTime = 40;
                }
            }

            DeleteBall(aBall);
            mBallList.erase(anItr++);
        }
    }
}

void CurveMgr::UpdatePowerUps()
{
    if (mBallList.empty())
        return;

    for (int i = 0; i < (int)PowerType_Max; i++)
    {
        int aFreq = mCurveDesc->mPowerUpFreq[i];

        if (aFreq > 0 && (Sexy::AppRand() % aFreq) == 0 && aFreq < mBoard->GetStateCount() - mLastPowerUpFrame[i])
        {
            AddPowerUp((PowerType)i);
            mLastPowerUpFrame[i] = mBoard->GetStateCount();
        }
    }
}

void CurveMgr::RemoveBallsAtFront()
{
    BallList::iterator anItr = mBallList.begin();
    while (anItr != mBallList.end())
    {
        Ball *aBall = *anItr;

        if (aBall->GetWayPoint() >= 1.0f)
        {
            break;
        }

        anItr++;
        DeleteBullet(aBall->GetBullet());
        aBall->RemoveFromList();

        if (aBall->GetClearCount() == 0)
        {
            mBoard->UpdateBallColorMap(aBall, false);
        }

        if (aBall->GetClearCount() != 0 || mStopAddingBalls)
        {
            DeleteBall(aBall);
        }
        else
        {
            mPendingBalls.push_front(aBall);
        }
    }
}

void CurveMgr::AdvanceMergingBullet(BulletList::iterator &theBulletItr)
{
    Bullet *aBul = *theBulletItr;
    aBul->CheckSetHitBallToPrevBall();
    Ball *aHitBall = aBul->GetHitBall();
    mWayPointMgr->SetWayPoint(aBul, aHitBall->GetWayPoint());
    mWayPointMgr->FindFreeWayPoint(aHitBall, aBul, aBul->GetHitInFront());
    aBul->SetDestPos(aBul->GetX(), aBul->GetY());
    aBul->Update();

    Ball *aPushBall = aBul->GetPushBall();

    if (aPushBall != NULL)
    {
        float num = 1.0f - aBul->GetHitPercent();
        float f = -aBul->GetRadius() * num / 2.0f;
        float aPoint = aPushBall->GetWayPoint();
        float aPercent = (aPushBall->GetRadius() + aBul->GetRadius()) * (aBul->GetHitPercent() * aBul->GetHitPercent());

        mWayPointMgr->FindFreeWayPoint(aBul, aBul->GetPushBall(), true, (int)f);

        if (aPushBall->GetWayPoint() - aBul->GetWayPoint() > aPercent)
        {
            float anEndPoint = aBul->GetWayPoint() + aPercent;
            if (anEndPoint > aPoint)
            {
                mWayPointMgr->SetWayPoint(aPushBall, anEndPoint);
            }
            else
            {
                mWayPointMgr->SetWayPoint(aPushBall, aPoint);
            }
        }

        aPushBall->SetNeedCheckCollision(true);
    }

    if (aBul->GetHitPercent() >= 1.0f)
    {
        BallList::iterator aBallItr = aHitBall->GetListItr();
        if (aBul->GetHitInFront())
        {
            aBallItr++;
        }

        Ball *aNewBall = new Ball();
        aNewBall->SetRotation(aBul->GetRotation());
        aNewBall->SetType(aBul->GetType());
        aNewBall->SetPowerType(aBul->GetPowerType(), false);
        mWayPointMgr->SetWayPoint(aNewBall, aBul->GetWayPoint());
        aNewBall->SetFrame(0);
        aNewBall->InsertInList(mBallList, aBallItr);
        mBoard->UpdateBallColorMap(aNewBall, true);

        int aMinGapDist = aBul->GetMinGapDist();
        int aNumGaps = aBul->GetNumGaps();

        delete aBul;
        theBulletItr = mBulletList.erase(theBulletItr);
        mTotalBalls++;

        Ball *aPrevBall = aNewBall->GetPrevBall();
        Ball *aNextBall = aNewBall->GetNextBall();
        aNewBall->UpdateCollisionInfo(5);
        aNewBall->SetNeedCheckCollision(true);

        if (aPrevBall && aNewBall->GetCollidesWithPrev())
        {
            aPrevBall->SetNeedCheckCollision(true);
        }

        if (aMinGapDist > 0)
        {
            aMinGapDist -= GetDefaultBallRadius() * 4;
            if (aMinGapDist < 0)
            {
                aMinGapDist = 0;
            }

            int aBonusRate = (mBoard->mIsEndless ? 250 : 500);
            int aGapBonus = ((MAX_GAP_SIZE - aMinGapDist) * aBonusRate / MAX_GAP_SIZE);
            aGapBonus = (aGapBonus / 10) * 10;
            if (aGapBonus < 10)
            {
                aGapBonus = 10;
            }
            if (aNumGaps > 1)
            {
                aGapBonus *= aNumGaps;
            }
            aNewBall->SetGapBonus(aGapBonus, aNumGaps);
        }

        mBoard->mNumClearsInARow++;
        if (!CheckSet(aNewBall))
        {
            mBoard->mNumClearsInARow--;

            if (aPrevBall != NULL &&
                !aPrevBall->GetCollidesWithNext() &&
                aPrevBall->GetType() == aNewBall->GetType() &&
                aPrevBall->GetBullet() == NULL &&
                aPrevBall->GetClearCount() == 0)
            {
                aNewBall->SetSuckPending(true);
                aNewBall->SetSuckCount(1);
            }
            else if (aNextBall != NULL &&
                     !aNewBall->GetCollidesWithNext() &&
                     aNextBall->GetType() == aNewBall->GetType() &&
                     aNextBall->GetBullet() == NULL &&
                     aNextBall->GetClearCount() == 0)
            {
                aNewBall->SetSuckPending(true);
                if (aNextBall->GetSuckCount() <= 0)
                {
                    aNextBall->SetSuckCount(1);
                }
            }
            else
            {
                mBoard->ResetInARowBonus();
                aNewBall->SetGapBonus(0, 0);
            }
        }
    }
    else
    {
        theBulletItr++;
    }
}

void CurveMgr::AdvanceBullets()
{
    BulletList::iterator anItr = mBulletList.begin();

    while (anItr != mBulletList.end())
    {
        AdvanceMergingBullet(anItr);
    }
}

void CurveMgr::StartClearCount(Ball *theBall)
{
    if (theBall->GetClearCount() > 0)
        return;

    mBoard->UpdateBallColorMap(theBall, false);
    mBoard->mLevelStats.mNumBallsCleared++;
    mBoard->mNumCleared++;

    mBoard->mClearedXSum += theBall->GetX();
    mBoard->mClearedYSum += theBall->GetY();
    mLastClearedBallPoint = theBall->GetWayPoint();

    if (theBall->GetSuckPending())
    {
        theBall->SetSuckPending(false);
    }

    theBall->StartClearCount(mWayPointMgr->InTunnel(theBall->GetWayPoint()));

    if (theBall->GetPowerTypeWussy() != PowerType_None)
    {
        mBoard->ActivatePower(theBall);
        mHadPowerUp = true;
    }
}

void CurveMgr::ActivateBomb(Ball *theBall)
{
    int aColor = gBallColors[theBall->GetType()];
    int aBallX = theBall->GetX();
    int aBallY = theBall->GetY();
    mBoard->mParticleMgr->AddExplosion(aBallX, aBallY, 0, aColor, 5);
    int v19 = Sexy::IMAGE_EXPLOSION->mWidth / 3;

    for (int i = v19, a6 = 7; i < 100; i += v19, a6 += 4)
    {
        float v21 = 0.0f;
        do
        {
            mBoard->mParticleMgr->AddExplosion(
                aBallX + (Sexy::AppRand() % 21 - 10) + (sinf(v21) * i),
                aBallY + (Sexy::AppRand() % 21 - 10) + (cosf(v21) * i),
                0,
                aColor,
                a6);
            v21 += ((float)v19 / (float)i);
        } while (v21 < SEXY_PI * 2);
    }

    mBoard->mParticleMgr->AddExplosion(
        theBall->GetX(),
        theBall->GetY(),
        0,
        aColor,
        0);

    for (BallList::iterator anItr = mBallList.begin(); anItr != mBallList.end(); anItr++)
    {
        Ball *aBall = *anItr;

        if (aBall->GetClearCount() == 0 && aBall->CollidesWithPhysically(theBall, 45))
        {
            aBall->SetComboCount(mBoard->mCurComboCount, mBoard->mCurComboScore);
            mBoard->mNeedComboCount.push_back(aBall);
            StartClearCount(aBall);
            mBoard->mParticleMgr->AddExplosion(
                theBall->GetX(),
                theBall->GetY(),
                0,
                aColor,
                0);
        }
    }
}

void CurveMgr::ClearPendingSucks(Ball *theEndBall)
{
    if (theEndBall == NULL)
        return;

    Ball *aBall = theEndBall;
    bool aCollided = true;

    while (aBall != NULL)
    {
        if (aBall->GetSuckPending())
        {
            aBall->SetSuckPending(false);
            mBoard->ResetInARowBonus();
            aBall->SetGapBonus(0, 0);
        }

        aBall = aBall->GetPrevBall();
        if (aBall == NULL)
        {
            return;
        }

        if (!aBall->GetCollidesWithNext())
        {
            aCollided = false;
        }

        if (!aCollided && aBall->GetSuckCount() != 0)
        {
            return;
        }
    }
}

void CurveMgr::RollBallsIn()
{
    float aSpeed = mCurveDesc->mSpeed;
    int aStartDistance = 50;
    if (!mBoard->mIsEndless)
    {
        aStartDistance = mCurveDesc->mStartDistance;
    }

    float aWayPoint = (float)(aStartDistance * mWayPointMgr->GetNumPoints() / 100);
    aWayPoint -= (float)mFirstChainEnd / mWayPointMgr->GetNumPoints();

    if (mFirstChainEnd <= 0 || aWayPoint > 0.0f)
    {
        mAdvanceSpeed = aSpeed + ((sqrtf(((aSpeed + 20.0f) * (aSpeed + 20.0f)) + ((aWayPoint * -20.0f) * -4.0f)) - (aSpeed + 20.0f)) * 0.5f + 18.0f) * 0.1f;
    }
    else
    {
        mAdvanceSpeed = mCurveDesc->mSpeed;
    }
}