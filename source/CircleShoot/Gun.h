#ifndef __GUN_H__
#define __GUN_H__

#include "CircleCommon.h"

namespace Sexy
{

void GunDeleteGlobals();

class Bullet;
class DataSync;
class Graphics;
class Image;
class MemoryImage;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
enum GunState
{
	GunState_Normal,
	GunState_Firing,
	GunState_Reloading
};

class Gun
{
protected:
	float mAngle;
	int mCenterX, mCenterY;
	int mRecoilX1, mRecoilY1;
	int mRecoilX2, mRecoilY2;
	int mRecoilCount;
	int mWidth, mHeight;
	Bullet *mBullet;
	Bullet *mNextBullet;

	GunState mState;
	float mStatePercent;
	int mBlinkCount;
	bool mWink;

	float mFireVel;
	bool mShowNextBall;

	MemoryImage *mCachedGunImage;
	float mCachedGunAngle;

	void CalcAngle();

public:
	Gun();
	virtual ~Gun();

	void SyncState(DataSync &theSync);

	void SetFireSpeed(float theSpeed) { mFireVel = theSpeed; }

	bool NeedsReload();

	void DeleteBullet();

	void Reload(int theType, bool delay = false, PowerType thePower = PowerType_None);
	void SetAngle(float theAngle);
	void Draw(Graphics *g);
	void DrawShadow(Graphics *g);
	void ToggleShowNextBall() { mShowNextBall = !mShowNextBall; }

	int GetCenterX() { return mCenterX; }
	int GetCenterY() { return mCenterY; }
	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }
	Bullet* GetBullet() { return mBullet; }
	Bullet* GetNextBullet() { return mNextBullet; }
	bool StartFire(bool recoil = true);
	Bullet* GetFiredBullet();

	void SetPos(int theX, int theY);
	void SetSize(int theWidth, int theHeight);
	float GetAngle() { return mAngle; }
	float GetCachedGunAngle() { return mCachedGunAngle; }
	bool IsFiring() { return mState == GunState_Firing; }

	void Update();
	void SwapBullets(bool playSound = true);
	void EmptyBullets();
	void CreateCachedGunImage();
	void DrawCachedGunImage(Graphics *g, float theZoom);
	void SetBulletType(int theType);
	void SetNextBulletType(int theType);
	void DoBlink(bool wink = false);

	
};
void GunDeleteGlobals();

}

#endif