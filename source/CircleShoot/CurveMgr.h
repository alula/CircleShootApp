#ifndef __CURVEMGR_H__
#define __CURVEMGR_H__

#include <SexyAppFramework/Common.h>

#include "CircleCommon.h"

#include <list>
#include <vector>

extern int gBoardColor;
extern int gCurveColor;

namespace Sexy
{

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class CircleShootApp;
	class WayPointMgr;
	class Gun;
	class Ball;
	class Bullet;
	class LevelDesc;
	class CurveDesc;
	class Board;
	class Graphics;
	class SpriteMgr;
	class CurveDrawer;
	class SexyVector3;
	class ParticleMgr;
	class DataSync;

	typedef std::list<Ball *> BallList;
	typedef std::list<Bullet *> BulletList;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class BallDrawer
	{
	public:
		int mNumBalls[MAX_PRIORITY];
		int mNumShadows[MAX_PRIORITY];

		Ball *mBalls[MAX_PRIORITY][1024];
		Ball *mShadows[MAX_PRIORITY][1024];

		void Reset();
		void AddBall(Ball *theBall, int thePriority);
		void AddShadow(Ball *theBall, int thePriority);

		void Draw(Graphics *g, SpriteMgr *theSpriteMgr, ParticleMgr *theParticleMgr);
	};

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class CurveMgr
	{
	protected:
		CircleShootApp *mApp;
		Board *mBoard;
		WayPointMgr *mWayPointMgr;
		LevelDesc *mLevelDesc;
		CurveDesc *mCurveDesc;
		SpriteMgr *mSpriteMgr;
		int mCurveNum;

		BulletList mBulletList;
		BallList mBallList;
		BallList mPendingBalls;
		int mLastPowerUpFrame[PowerType_Max];

		int mStopTime;
		int mSlowCount;
		int mBackwardCount;
		int mTotalBalls;
		float mAdvanceSpeed;
		int mFirstChainEnd;
		bool mFirstBallMovedBackwards;
		bool mHaveSets;
		int mDangerPoint;
		int mPathLightEndFrame;
		int mLastClearedBallPoint;
		bool mHadPowerUp;
		bool mStopAddingBalls;
		bool mInDanger;
		unsigned int mLastPathShowTick;

		void DeleteBullet(Bullet *theBullet);
		void DeleteBall(Ball *theBall);

		void SetFarthestBall(int thePoint);
		int GetNumInARow(Ball *theBall, int theColor, Ball **theNextEnd = NULL, Ball **thePrevEnd = NULL);
		bool CheckSet(Ball *theBall);
		void DoScoring(Ball *theBall, int theNumBalls, int theComboCount, int theGapBonus, int theNumGaps);

		int GetNumPendingSingles(int theNumGroups);
		void AddPendingBall();
		void AddBall();
		void UpdateBallRotation();
		void AdvanceBalls();
		void AdvanceBackwardBalls();
		void UpdateSuckingBalls();
		void UpdateSets();
		void UpdatePowerUps();
		void RemoveBallsAtFront();
		void AdvanceMergingBullet(BulletList::iterator &theBulletItr);
		void AdvanceBullets();
		void StartClearCount(Ball *theBall);
		void ActivateBomb(Ball *theBall);
		void ClearPendingSucks(Ball *theEndBall);

		void RollBallsIn();

	public:
		CurveMgr(Board *theBoard);
		virtual ~CurveMgr();

		void SetLosing();
		void SetupLevel(LevelDesc *theDesc, SpriteMgr *theSpriteMgr, int theCurveNum, MirrorType theMirror = MirrorType_None);
		void SetupLevelDesc(LevelDesc *theDesc);
		void StartLevel();

		void UpdatePlaying();
		void UpdateLosing();

		void DrawBalls(BallDrawer &theDrawer);
		bool CheckCollision(Bullet *theBullet);
		bool CheckGapShot(Bullet *theBullet);

		int GetRandomPendingBallColor(int theMaxNumBalls);
		bool HasPendingBallOfType(int theType, int theMaxNumBalls);
		bool IsLosing();
		bool IsWinning();
		bool CanRestart();
		bool CanFire();
		bool IsInDanger() { return mInDanger; }

		Ball *CheckBallIntersection(const SexyVector3 &p1, const SexyVector3 &v1, float &t);

		void ActivatePower(Ball *theBall);

		void DrawCurve(CurveDrawer &theDrawer);
		void DrawTunnel(CurveDrawer &theDrawer);
		void DeleteBalls();
		void GetPoint(int thePoint, int &x, int &y, int &pri);
		int GetCurveLength();
		int GetTotalBalls();
		void SetStopAddingBalls(bool stop);
		void DoEndlessZumaEffect();
		void DetonateBalls();
		int GetFarthestBallPercent();
		int GetLastClearedBallPoint() { return mLastClearedBallPoint; }
		int DrawPathSparkles(int theStartPoint, int theStagger, bool addSound);
		int DrawEndLevelBonus(int theStagger);
		bool HasReachedCruisingSpeed();
		void AddPowerUp(PowerType thePower);

		void SyncState(DataSync &theSync);
	};

} // namespace Sexy

#endif