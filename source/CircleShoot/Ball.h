#ifndef __BALL_H__
#define __BALL_H__

#include "CircleCommon.h"

#include <list>

extern bool gSpeedUp;
extern bool gWasReset;
extern bool gBallBlink;
extern bool gForceTreasure;
extern int gForceTreasureNum;
extern int gColorOverride;
extern int gNumColors;
extern bool gCheckCollision;

namespace Sexy
{

	void BallDeleteGlobals();

	class DataSync;
	class Bullet;
	class Graphics;
	class SexyVector3;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class Ball;
	typedef std::list<Ball *> BallList;

	class Ball
	{
	protected:
		static int mIdGen;

		int mId;
		int mType;
		float mWayPoint;
		float mRotation;
		float mDestRotation;
		float mRotationInc;
		float mx, my;

		BallList *mList;
		BallList::iterator mListItr;
		bool mCollidesWithNext;
		bool mNeedCheckCollision;
		bool mSuckPending;

		Bullet *mBullet;
		int mClearCount;
		int mSuckCount;
		int mBackwardsCount;
		float mBackwardsSpeed;
		int mComboCount;
		int mComboScore;
		int mStartFrame;
		int mPowerCount;
		int mPowerFade;
		unsigned short mGapBonus;
		unsigned short mNumGaps;

		struct Particle
		{
			float x, y, vx, vy;
			int mSize;
		};
		Particle *mParticles;

		PowerType mPowerType;
		PowerType mDestPowerType;

		void DrawBomb(Graphics *g);
		void DrawStandardPower(Graphics *g, int theBallImageId, int theBlinkImageId, int thePowerType);
		void DrawPower(Graphics *g);
		void DrawExplosion(Graphics *g);
		void DoDraw(Graphics *g);

	public:
		Ball();
		virtual ~Ball();

		void SetPos(float x, float y);
		void SetWayPoint(float thePoint);
		void SetType(int theType) { mType = theType; }
		void SetRotation(float theRot, bool immediate = true);
		virtual void Draw(Graphics *g);
		void DrawShadow(Graphics *g);

		float GetX() { return mx; }
		float GetY() { return my; }
		float GetWayPoint() { return mWayPoint; }
		int GetRadius();
		int GetType() { return mType; }
		float GetRotation() { return mRotation; }

		bool CollidesWithPhysically(Ball *theBall, int thePad = 0);
		bool CollidesWith(Ball *theBall, int thePad = 0);
		bool Intersects(const SexyVector3 &p1, const SexyVector3 &v1, float &t);

		void SetBullet(Bullet *theBullet);
		Bullet *GetBullet() { return mBullet; }

		void SetCollidesWithNext(bool collidesWithNext) { mCollidesWithNext = collidesWithNext; }
		void SetCollidesWithPrev(bool collidesWithPrev);
		bool GetCollidesWithNext() { return mCollidesWithNext; }
		bool GetCollidesWithPrev();
		void UpdateCollisionInfo(int thePad = 0);

		int GetClearCount() { return mClearCount; }
		void StartClearCount(bool inTunnel);
		void SetClearCount(int theCount);

		int GetSuckCount() { return mSuckCount; }
		void SetSuckCount(int theCount) { mSuckCount = theCount; }

		int GetComboCount() { return mComboCount; }
		int GetComboScore() { return mComboScore; }
		void SetComboCount(int theCount, int theScore)
		{
			mComboCount = theCount;
			mComboScore = theScore;
		}

		int GetBackwardsCount() { return mBackwardsCount; }
		void SetBackwardsCount(int theCount) { mBackwardsCount = theCount; }

		float GetBackwardsSpeed() { return mBackwardsSpeed; }
		void SetBackwardsSpeed(float theSpeed) { mBackwardsSpeed = theSpeed; }

		void SetNeedCheckCollision(bool needCheck) { mNeedCheckCollision = needCheck; }
		bool GetNeedCheckCollision() { return mNeedCheckCollision; }

		void SetSuckPending(bool pending) { mSuckPending = pending; }
		bool GetSuckPending() { return mSuckPending; }

		void SetPowerType(PowerType theType, bool delay = true);
		PowerType GetPowerType() { return mPowerType; }
		PowerType GetDestPowerType() { return mDestPowerType; }
		PowerType GetPowerTypeWussy();

		void RemoveFromList();
		const BallList::iterator &InsertInList(BallList &theList, const BallList::iterator &theInsertItr);
		const BallList::iterator &GetListItr() { return mListItr; }

		Ball *GetPrevBall(bool mustCollide = false);
		Ball *GetNextBall(bool mustCollide = false);

		void UpdateRotation();
		void SetFrame(int theFrame);
		void IncFrame(int theInc);
		void RandomizeFrame();
		void SetGapBonus(int theBonus, int theNumGaps)
		{
			mGapBonus = theBonus;
			mNumGaps = theNumGaps;
		}
		int GetGapBonus() { return mGapBonus; }
		int GetNumGaps() { return mNumGaps; }
		int GetStartFrame() { return mStartFrame; }
		unsigned int GetId() { return mId; }

		void SyncState(DataSync &theSync);
	};

	void BallDeleteGlobals();

} // namespace Sexy

#endif