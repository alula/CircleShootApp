#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/Graphics.h>
#include <SexyAppFramework/Image.h>
#include <SexyAppFramework/MemoryImage.h>
#include <SexyAppFramework/SexyVector.h>

#include "CircleCommon.h"
#include "Ball.h"
#include "Bullet.h"
#include "CurveData.h"
#include "SpriteMgr.h"
#include "WayPoint.h"

#include <math.h>

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
WayPointMgr::WayPointMgr()
{
}

WayPointMgr::~WayPointMgr()
{
}

void WayPointMgr::SetWayPoint(Ball *theBall, float thePoint)
{

    if (mWayPoints.empty())
        return;

    int aPoint = thePoint;
    int aNextPoint = 0;

    if (thePoint < 0)
    {
        aPoint = 0;
        aNextPoint = 1;
    }
    else if (aPoint >= mWayPoints.size())
    {
        aPoint = mWayPoints.size() - 1;
        aNextPoint = aPoint + 1;
    }
    else
    {
        aPoint = thePoint;
        aNextPoint = aPoint + 1;
    }

    WayPoint *aWayPoint = &mWayPoints[aPoint];
    WayPoint *aNextWayPoint = aNextPoint < mWayPoints.size() ? &mWayPoints[aNextPoint] : aWayPoint;
    float aMix = (thePoint - aPoint);

    CalcAvgRotationForPoint(aPoint);
    theBall->SetPos(aWayPoint->x + aMix * (aNextWayPoint->x - aWayPoint->x),
                    aWayPoint->y + aMix * (aNextWayPoint->y - aWayPoint->y));
    theBall->SetWayPoint(thePoint);
    theBall->SetRotation(aWayPoint->mAvgRotation, false);
}

void WayPointMgr::SetWayPointInt(Ball *theBall, int thePoint)
{
    if (mWayPoints.empty())
        return;

    int aPoint = 0;

    if (thePoint >= 0)
    {
        if (thePoint >= mWayPoints.size())
        {
            aPoint = mWayPoints.size() - 1;
        }
        else
        {
            aPoint = thePoint;
        }
    }

    WayPoint *aWayPoint = &mWayPoints[aPoint];
    CalcAvgRotationForPoint(aPoint);
    theBall->SetPos(aWayPoint->x, aWayPoint->y);
    theBall->SetWayPoint(thePoint);
    theBall->SetRotation(aWayPoint->mAvgRotation, false);
}

void WayPointMgr::FindFreeWayPoint(Ball *theExistingBall, Ball *theNewBall, bool inFront, int thePad)
{
    int aWayPoint = theExistingBall->GetWayPoint();
    bool v7 = inFront
                  ? (theNewBall->GetWayPoint() <= aWayPoint)
                  : (theNewBall->GetWayPoint() >= aWayPoint);

    if (!v7)
    {
        aWayPoint = (int)theNewBall->GetWayPoint();
    }

    while (aWayPoint >= 0)
    {
        if (aWayPoint >= mWayPoints.size())
        {
            break;
        }

        WayPoint &aWayPoint2 = mWayPoints[aWayPoint];
        theNewBall->SetPos(aWayPoint2.x, aWayPoint2.y);
        if (!theNewBall->CollidesWithPhysically(theExistingBall, thePad))
        {
            break;
        }

        aWayPoint += !inFront ? -1 : 1;
    }

    SetWayPointInt(theNewBall, aWayPoint);
}

SexyVector3 WayPointMgr::CalcPerpendicular(float theWayPoint)
{
    int aWayPoint = theWayPoint;
    if (aWayPoint < 0)
        aWayPoint = 0;

    if (aWayPoint >= mWayPoints.size())
        aWayPoint = mWayPoints.size() - 1;

    CalcPerpendicularForPoint(aWayPoint);
    return mWayPoints[aWayPoint].mPerpendicular;
}

float WayPointMgr::GetRotationForPoint(int theWayPoint)
{
    int aWayPoint = theWayPoint;
    if (aWayPoint < 0)
        aWayPoint = 0;

    if (aWayPoint >= mWayPoints.size() - 1)
        aWayPoint = mWayPoints.size() - 1;

    CalcPerpendicularForPoint(aWayPoint);
    return mWayPoints[aWayPoint].mRotation;
}

void WayPointMgr::CalcPerpendicularForPoint(int theWayPoint)
{
    WayPoint *p1 = &mWayPoints[theWayPoint];
    if (p1->mHavePerpendicular)
        return;

    WayPoint *p2 = p1;
    if (theWayPoint + 1 < mWayPoints.size())
    {
        p2 = &mWayPoints[theWayPoint + 1];
    }
    else
    {
        p2 = &mWayPoints[theWayPoint - 1];
    }

    SexyVector3 v1(1, 0, 0);

    p1->mPerpendicular = SexyVector3(p2->y - p1->y, p1->x - p2->x, 0.0f).Normalize();
    p1->mRotation = acosf(p1->mPerpendicular.Dot(v1));

    if (p1->mPerpendicular.y > 0.0f)
    {
        p1->mRotation = -p1->mRotation;
    }

    if (p1->mRotation < 0.0f)
    {
        p1->mRotation += 2 * SEXY_PI;
    }

    p1->mHavePerpendicular = true;
}

void WayPointMgr::CalcAvgRotationForPoint(int theWayPoint)
{
    WayPoint *p1 = &mWayPoints[theWayPoint];
    if (p1->mHaveAvgRotation)
    {
        return;
    }

    CalcPerpendicularForPoint(theWayPoint);
    p1->mHaveAvgRotation = true;
    p1->mAvgRotation = p1->mRotation;

    int first = theWayPoint - 10;
    int last = theWayPoint + 10;

    if (first < 0)
    {
        first = 0;
    }
    if (last >= mWayPoints.size())
    {
        last = mWayPoints.size() - 1;
    }

    CalcPerpendicularForPoint(first);

    for (int i = first + 1; i < last; i++)
    {
        CalcPerpendicularForPoint(i);

        float dr = mWayPoints[i].mRotation - mWayPoints[i - 1].mRotation;
        if (dr > 0.0f)
        {
            while (dr > SEXY_PI)
            {
                dr = dr - 2 * SEXY_PI;
            }
        }
        else if (dr < 0.0)
        {
            while (dr < -SEXY_PI)
            {
                dr = dr + 2 * SEXY_PI;
            }
        }

        if (dr > 0.1f || dr < -0.1f)
        {
            WayPoint &w1 = mWayPoints[i];
            WayPoint &w2 = mWayPoints[i - 1];

            float mix = 1.0f - (float)(i - first) / (float)(last - first);
            p1->mAvgRotation = mWayPoints[first].mRotation + mix * dr;
            return;
        }
    }
}

int WayPointMgr::GetPriority(Ball *theBall)
{
    double aRadius = theBall->GetRadius();
    int aPrevPriority = GetPriority(theBall->GetWayPoint() - aRadius);
    int aNextPriority = GetPriority(theBall->GetWayPoint() + aRadius);

    return max(aPrevPriority, aNextPriority);
}

int WayPointMgr::GetPriority(int thePoint)
{
    if (thePoint < 0)
        return 0;

    int numWayPoints = mWayPoints.size();

    if (thePoint >= numWayPoints)
        return 0;
    else
        return mWayPoints[thePoint].mPriority;
}

int WayPointMgr::GetPriority(Bullet *theBullet)
{
    if (theBullet->GetWayPoint() == 0 || theBullet->GetHitPercent() < 0.7f)
        return 4;

    return GetPriority(reinterpret_cast<Ball *>(theBullet));
}

bool WayPointMgr::LoadCurve(const std::string &theFile, MirrorType theMirror)
{
    std::string curvePath(theFile);

    if (curvePath.rfind('.') == -1)
    {
        curvePath += ".dat";
    }

    if (mCurvePath == theFile)
    {
        return true;
    }

    mCurvePath = theFile;
    mWayPoints.clear();

    CurveData curveData;
    WayPoint aWayPoint;

    if (!curveData.Load(curvePath))
    {
        return false;
    }

    for (PathPointList::iterator pItr = curveData.mPointList.begin(); pItr != curveData.mPointList.end(); ++pItr)
    {
        aWayPoint.x = pItr->x;
        aWayPoint.y = pItr->y;
        aWayPoint.mHavePerpendicular = false;
        aWayPoint.mHaveAvgRotation = false;
        aWayPoint.mPerpendicular = SexyVector3(0.0, 0.0, 0.0);
        aWayPoint.mInTunnel = false;
        aWayPoint.mPriority = 0;

        mWayPoints.push_back(aWayPoint);
        mWayPoints.back().mInTunnel = pItr->mInTunnel;
        mWayPoints.back().mPriority = pItr->mPriority;
    }

    if (theMirror != MirrorType_None)
    {
        for (int i = 0; i < mWayPoints.size(); i++)
        {
            WayPoint *point = &mWayPoints[i];
            Sexy::MirrorPoint(point->x, point->y, theMirror);
        }
    }

    return true;
}

void WayPointMgr::DrawCurvePiece(Graphics *g, int theWayPoint, float theThickness)
{
    if (theWayPoint == 0)
        return;

    WayPoint &aPrevWayPoint = mWayPoints[theWayPoint - 1];
    WayPoint &aCurWayPoint = mWayPoints[theWayPoint];

    SexyVector3 aPrevPerpendicular = CalcPerpendicular(theWayPoint - 1);
    float p1 = theThickness * aPrevPerpendicular.x;
    float p2 = theThickness * aPrevPerpendicular.y;

    SexyVector3 aCurPerpendicular = CalcPerpendicular(theWayPoint);
    float x1 = aPrevWayPoint.x;
    float y1 = aPrevWayPoint.y;
    float x2 = aCurWayPoint.x;
    float y2 = aCurWayPoint.y;

    Point aPoints[4];
    aPoints[0].mX = (int)(x1 - p1);
    aPoints[0].mY = (int)(y1 - p2);
    aPoints[1].mX = (int)(p1 + x1);
    aPoints[1].mY = (int)(p2 + y1);
    aPoints[2].mX = (int)(x2 + aCurPerpendicular.x * theThickness);
    aPoints[2].mY = (int)(y2 + aCurPerpendicular.y * theThickness);
    aPoints[3].mX = (int)(x2 - aCurPerpendicular.x * theThickness);
    aPoints[3].mY = (int)(y2 - aCurPerpendicular.y * theThickness);

    g->PolyFill(aPoints, 4, false);
}

void WayPointMgr::DrawCurve(CurveDrawer &theDrawer, const Color &theColor, int theDangerPoint)
{
    if (mWayPoints.empty())
        return;

    for (int i = 0; i < mWayPoints.size(); i++)
    {
        WayPoint &aWayPoint = mWayPoints[i];
        Graphics *g = theDrawer.GetGraphics(aWayPoint.mPriority - (aWayPoint.mPriority == 0));

        if (i > theDangerPoint)
        {
            int v8 = mWayPoints.size();
            int v9 = mWayPoints.size();
            int v10 = v8 - i;
            int v11;
            int v13;

            if (v8 - i < 0)
                v11 = (float)(v10 & 1 | (v10 >> 1)) + (float)(v10 & 1 | (v10 >> 1));
            else
                v11 = (float)(int)v10;

            int v12 = v9 - theDangerPoint;
            if (v12 < 0)
                v13 = (float)(v12 & 1 | (v12 >> 1)) + (float)(v12 & 1 | (v12 >> 1));
            else
                v13 = (float)(int)v12;

            Color aColor((1.0f - (v11 / (float)v13)) * 255.0f +
                             theColor.mRed * (v11 / (float)v13),
                         theColor.mGreen * (v11 / (float)v13),
                         theColor.mBlue * (v11 / (float)v13));

            g->SetColor(aColor);
            DrawCurvePiece(g, i, 5.0f);
        }
        else
        {
            g->SetColor(theColor);
            DrawCurvePiece(g, i, 5.0f);
        }
    }
}

void WayPointMgr::DrawTunnel(CurveDrawer &theDrawer) {}

bool WayPointMgr::InTunnel(int theWayPoint)
{
    if (theWayPoint < 0)
        return true;

    if (theWayPoint >= (int)mWayPoints.size())
        return false;

    return mWayPoints[theWayPoint].mInTunnel;
}

bool WayPointMgr::InTunnel(Ball *theBall, bool inFront)
{
    int aWayPoint = theBall->GetWayPoint();

    if (inFront)
        return InTunnel(aWayPoint + theBall->GetRadius());
    else
        return InTunnel(aWayPoint - theBall->GetRadius());
}

bool WayPointMgr::InTunnel(Bullet *theBullet)
{
    Ball *theBall = theBullet->GetHitBall();
    int aWayPoint = theBall->GetWayPoint();

    if (theBullet->GetHitInFront())
    {
        aWayPoint += theBall->GetRadius() * 3;
    }
    else
    {
        aWayPoint -= theBall->GetRadius() * 3;
    }

    return InTunnel(aWayPoint);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CurveDrawer::CurveDrawer(SpriteMgr *theMgr)
{
    mSpriteMgr = theMgr;
    mBackG = new Graphics(theMgr->GetBackground());

    for (int i = 0; i < MAX_PRIORITY; i++)
    {
        mForeImage[i] = NULL;
        mForeG[i] = NULL;
    }
}

CurveDrawer::~CurveDrawer()
{
}

Graphics *CurveDrawer::GetGraphics(int thePriority)
{
    if (thePriority < 0)
        return mBackG;

    if (mForeG[thePriority] != NULL)
        return mForeG[thePriority];

    mForeImage[thePriority] = new MemoryImage(gSexyAppBase);
    mForeImage[thePriority]->Create(640, 480);
    mForeG[thePriority] = new Graphics(mForeImage[thePriority]);

    return mForeG[thePriority];
}

void CurveDrawer::AddCurvesToMgr()
{
    for (int i = 0; i < 5; i++)
    {
        if (mForeImage[i] != NULL)
            mSpriteMgr->AddSprite(mForeImage[i], 0, 0, i);

        delete mForeG[i];
    }
}