#ifndef __BULLET_H__
#define __BULLET_H__
#include "Ball.h"

namespace Sexy
{

	class DataSync;
	class Graphics;
	class Image;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	struct GapInfo
	{
		int mCurve;
		int mDist;
		int mBallId;

		void SyncState(DataSync &theSync);
	};
	typedef std::list<GapInfo> GapInfoVector;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class Bullet : public Ball
	{
	protected:
		float mVelX;
		float mVelY;

		Ball *mHitBall;
		bool mHitInFront;
		bool mHaveSetPrevBall;
		float mHitX;
		float mHitY;
		float mDestX;
		float mDestY;
		float mHitPercent;
		float mMergeSpeed;

		GapInfoVector mGapInfo;
		int mCurCurvePoint[3];

		void SetBallInfo(Bullet *theBullet);

	public:
		Bullet();
		virtual ~Bullet();

		void SetVelocity(float vx, float vy);
		void SetHitBall(Ball *theBall, bool hitInFront);
		void CheckSetHitBallToPrevBall();
		void SetDestPos(float x, float y);
		void SetMergeSpeed(float theSpeed) { mMergeSpeed = theSpeed; }

		void Update();
		void MergeFully();

		float GetHitPercent() { return mHitPercent; }
		Ball *GetHitBall() { return mHitBall; }
		Ball *GetPushBall();
		bool GetHitInFront() { return mHitInFront; }
		void UpdateHitPos();

		void SetCurCurvePoint(int theCurveNum, int thePoint);
		int GetCurCurvePoint(int theCurveNum);

		bool AddGapInfo(int theCurve, int theDist, int theBallId);
		int GetCurGapBall(int theCurveNum);
		int GetMinGapDist();
		void RemoveGapInfoForBall(int theBallId);
		int GetNumGaps() { return (int)mGapInfo.size(); }

		float GetVelX() { return mVelX; }
		float GetVelY() { return mVelY; }

		virtual void Draw(Graphics *g);

		void SyncState(DataSync &theSync);
	};

} // namespace Sexy

#endif