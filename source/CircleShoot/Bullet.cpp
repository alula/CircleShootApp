#include "Zuma_Prefix.pch"

#include "Bullet.h"
#include "DataSync.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Bullet::Bullet()
{
    mVelX = 0.0f;
    mVelY = 0.0f;
    mHitBall = NULL;
    mHitPercent = 0.0f;
    mMergeSpeed = 0.05f;

    for (int i = 0; i < 3; i++)
    {
        mCurCurvePoint[i] = 0;
    }
}

Bullet::~Bullet()
{
    SetBallInfo(NULL);
}

// this method is inlined in executable
void Bullet::SetBallInfo(Bullet *theBullet)
{
    if (mHitBall != NULL)
    {
        mHitBall->SetBullet(theBullet);
    }
}

void Bullet::SetVelocity(float vx, float vy)
{
    mVelX = vx;
    mVelY = vy;
}

void Bullet::SetHitBall(Ball *theBall, bool hitInFront)
{
    SetBallInfo(NULL);
    mHaveSetPrevBall = false;
    mHitBall = theBall;
    mHitX = mx;
    mHitY = my;
    mHitPercent = 0.0f;
    mHitInFront = hitInFront;
    SetBallInfo(this);
}

void Bullet::CheckSetHitBallToPrevBall()
{
    if (mHaveSetPrevBall || mHitBall == NULL)
    {
        return;
    }
    Ball *aPrevBall = mHitBall->GetPrevBall();
    if (aPrevBall == NULL)
    {
        return;
    }

    if (aPrevBall->CollidesWithPhysically(this) && aPrevBall->GetClearCount() == 0)
    {
        mHaveSetPrevBall = true;
        SetBallInfo(NULL);
        mHitBall = aPrevBall;
        mHitInFront = true;
        mHitX = mx;
        mHitY = my;
        mHitPercent = 0.0f;
        SetBallInfo(this);
    }
}

void Bullet::SetDestPos(float x, float y)
{
    mDestX = x;
    mDestY = y;
}

void Bullet::Update()
{
    if (mHitBall == NULL)
    {
        mx += mVelX;
        my += mVelY;
        return;
    }

    if (mClearCount == 0)
    {
        mHitPercent += mMergeSpeed;
        if (mHitPercent > 1.0f)
        {
            mHitPercent = 1.0f;
        }

        mx = mHitX + (mDestX - mHitX) * mHitPercent;
        my = mHitY + (mDestY - mHitY) * mHitPercent;
    }
}

void Bullet::MergeFully()
{
    mHitPercent = 1.0f;
    Update();
}

Ball *Bullet::GetPushBall()
{
    if (mHitBall == NULL)
    {
        return NULL;
    }

    Ball *aPushBall = mHitBall;
    if (mHitInFront)
    {
        aPushBall = aPushBall->GetNextBall();
        if (aPushBall == NULL)
            return NULL;
    }

    if (aPushBall->CollidesWithPhysically(this))
    {
        return aPushBall;
    }

    return NULL;
}

void Bullet::UpdateHitPos()
{
    mHitX = mx;
    mHitY = my;
}

void Bullet::SetCurCurvePoint(int theCurveNum, int thePoint)
{
    mCurCurvePoint[theCurveNum] = thePoint;
}

int Bullet::GetCurCurvePoint(int theCurveNum)
{
    return mCurCurvePoint[theCurveNum];
}

bool Bullet::AddGapInfo(int theCurve, int theDist, int theBallId)
{
    for (GapInfoVector::iterator i = mGapInfo.begin(); i != mGapInfo.end(); i++)
    {
        if (i->mBallId == theBallId)
        {
            return false;
        }
    }

    mGapInfo.push_back(GapInfo());

    GapInfo &anInfo = mGapInfo.back();
    anInfo.mBallId = theBallId;
    anInfo.mDist = theDist;
    anInfo.mCurve = theCurve;

    return true;
}

// int Bullet::GetCurGapBall(int theCurveNum) {}

int Bullet::GetMinGapDist()
{
    int aMinDist = 0;

    for (GapInfoVector::iterator i = mGapInfo.begin(); i != mGapInfo.end(); i++)
    {
        if (aMinDist == 0 || i->mDist < aMinDist)
        {
            aMinDist = i->mDist;
        }
    }

    return aMinDist;
}

void Bullet::RemoveGapInfoForBall(int theBallId)
{
    for (GapInfoVector::iterator i = mGapInfo.begin(); i != mGapInfo.end();)
    {
        if (i->mBallId == theBallId)
        {
            mGapInfo.erase(i++);
        }
        else
        {
            i++;
        }
    }
}

void Bullet::Draw(Graphics *g)
{
    float aWayPoint = mWayPoint;
    mWayPoint = 0.0f;
    Ball::Draw(g);
    mWayPoint = aWayPoint;
}

void Bullet::SyncState(DataSync &theSync)
{
    Ball::SyncState(theSync);

    theSync.SyncFloat(mVelX);
    theSync.SyncFloat(mVelY);
    theSync.SyncBool(mHitInFront);
    theSync.SyncBool(mHaveSetPrevBall);
    theSync.SyncFloat(mHitX);
    theSync.SyncFloat(mHitY);
    theSync.SyncFloat(mDestX);
    theSync.SyncFloat(mDestY);
    theSync.SyncFloat(mHitPercent);
    theSync.SyncFloat(mMergeSpeed);

    for (int i = 0; i < 3; i++)
    {
        theSync.SyncShort(mCurCurvePoint[i]);
    }

    theSync.SyncPointer((void **)&mHitBall);
    Sexy::DataSync_SyncSTLContainer(theSync, mGapInfo);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void GapInfo::SyncState(DataSync &theSync)
{
    theSync.SyncByte(mCurve);
    theSync.SyncShort(mDist);
    theSync.SyncLong(mBallId);
}