#ifndef __SEXY_WAYPOINT_H__
#define __SEXY_WAYPOINT_H__

#include "CircleCommon.h"

#include <SexyAppFramework/SexyVector.h>

#include <vector>
#include <list>


namespace Sexy
{

class Ball;
class Bullet;
class Color;
class Graphics;
class Image;
class MemoryImage;
class SpriteMgr;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct WayPoint
{
	float x,y;

	bool mHavePerpendicular;
	bool mHaveAvgRotation;
	SexyVector3 mPerpendicular;
	float mRotation;
	float mAvgRotation;
	bool mInTunnel;
	unsigned char mPriority;

	WayPoint() : mHavePerpendicular(false), mHaveAvgRotation(false), mInTunnel(false), mPriority(0) { }
	WayPoint(float theX, float theY) : x(theX), y(theY), mHaveAvgRotation(false), mHavePerpendicular(false), mInTunnel(false), mPriority(0) { }
};
typedef std::vector<WayPoint> WayPointList;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CurveDrawer
{
public:
	SpriteMgr *mSpriteMgr;
	Graphics *mBackG;
	Graphics *mForeG[MAX_PRIORITY];
	MemoryImage *mForeImage[MAX_PRIORITY];

public:
	CurveDrawer(SpriteMgr *theMgr);
	virtual ~CurveDrawer();

	Graphics* GetGraphics(int thePriority);
	void AddCurvesToMgr();
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class WayPointMgr
{
protected:
	WayPointList mWayPoints;

	std::string mCurvePath;

	void DrawCurvePiece(Graphics *g, int theWayPoint, float theThickness); 

public:
	WayPointMgr();
	virtual ~WayPointMgr();

	void SetWayPoint(Ball *theBall, float thePoint);
	void SetWayPointInt(Ball *theBall, int thePoint);
	void FindFreeWayPoint(Ball *theExistingBall, Ball *theNewBall, bool inFront, int thePad = 0);
	SexyVector3 CalcPerpendicular(float theWayPoint);
	float GetRotationForPoint(int theWayPoint);
	void CalcPerpendicularForPoint(int theWayPoint);
	void CalcAvgRotationForPoint(int theWayPoint);
	int GetPriority(Ball *theBall);
	int GetPriority(int thePoint);
	int GetPriority(Bullet *theBullet);


	int GetNumPoints() { return (int)mWayPoints.size(); }
	int GetEndPoint() { return (int)mWayPoints.size()-1; }

	bool LoadCurve(const std::string &theFile, MirrorType theMirror = MirrorType_None);

	void DrawCurve(CurveDrawer &theDrawer, const Color &theColor, int theDangerPoint); 
	void DrawTunnel(CurveDrawer &theDrawer); 
	bool InTunnel(int theWayPoint);
	bool InTunnel(Ball *theBall, bool inFront);
	bool InTunnel(Bullet *theBullet);

	const WayPointList& GetWayPointList() { return mWayPoints; }
};

} // namespace Sexy

#endif