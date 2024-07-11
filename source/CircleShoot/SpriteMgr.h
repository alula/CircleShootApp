#ifndef __SPRITEMGR_H__
#define __SPRITEMGR_H__
#include "CircleCommon.h"
#include "SexyAppFramework/Common.h"

namespace Sexy
{

void SpriteMgrDeleteGlobals();

class DataSync;
class Image;
class MemoryImage;
class LevelDesc;
class Graphics;
class SpriteDesc;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class SpriteImage
{
public:
	int x,y;
	float vx,vy;
	Image *mImage;

	SpriteImage() : mImage(NULL) { }
	virtual ~SpriteImage();
};
typedef std::list<SpriteImage> SpriteList;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class SpriteMgr
{
protected:
	SpriteList mSprites[MAX_PRIORITY];
	SpriteList mBackgroundSprites;
	MemoryImage *mBackgroundImage;
	Image *mNebulaImage;
	int mUpdateCnt;
	bool mHasBackground;
	bool mInSpace;
	bool mSpaceScroll;

	struct StarInfo
	{
		float x,y;
		float vy;
		unsigned int mColor;
	};
	typedef std::list<StarInfo> StarList;
	StarList mStarList;

	struct HoleInfo
	{
		MemoryImage *mImage;
		int mImageFrame;

		int mx,my;
		int mFrame;
		int mTotalBrightness;
		float mRotation;
		float mPercentOpen[3];
		int mBrightness[3];

		HoleInfo() : mImage(NULL) { }
		virtual ~HoleInfo();
	};

	struct HoleFlash
	{
		int mUpdateCount;
		int mCurveNum;
	};
	typedef std::list<HoleFlash> HoleFlashList;
	HoleFlashList mHoleFlashList;

	int mNumHoles;
	int mHoleMappings[3];
	HoleInfo mHoleInfo[3];

	void DrawSprites(Graphics *g, const SpriteList &theList);
	void DrawSpace(Graphics *g);
	void GenerateTransitionSprite(MemoryImage *theBackgroundImage, MemoryImage *theAlpha, const SpriteDesc &theDesc);
	void AddStar(int y);
	void UpdateHoles();
	void UpdateStars();
	void InitStars();

public:
	SpriteMgr();
	virtual ~SpriteMgr();

	void AddSprite(Image *theImage, int x, int y, int thePriority);
	void GenBackgroundForLevel(const LevelDesc &theLevel);
	void SetupLevel(const LevelDesc &theLevel, MirrorType theMirror = MirrorType_None);
	void GenerateBackgroundTransitionSprites(const LevelDesc &theLevel);

	bool HasBackground() { return mHasBackground; }

	void DrawBackgroundTransition(Graphics *g);
	void UpdateBackgroundTransition(int theStep);

	void DrawBackground(Graphics *g);
	void DrawLevel(Graphics *g);
	void DrawSprites(Graphics *g, int thePriority);
	void DrawBorder(Graphics *g);
	void DrawHole(Graphics *g, int theHoleNum);
	void DrawHoleForCurve(Graphics *g, int theCurveNum);

	void UpdateHole(int theCurveNum, float thePercentOpen);
	void UpdateHoleBrightness(int theCurveNum, int theBrightness);
	void PlaceHole(int theCurveNum, int theX, int theY, float theRotation);
	void ClearHoleFlashes();
	void AddHoleFlash(int theCurveNum, int theStagger);
	void Update();

	void SetupSpace();
	void SetInSpace(bool inSpace) { mInSpace = inSpace; }
	bool GetInSpace() { return mInSpace; }
	void ToggleSpaceScroll() { mSpaceScroll = !mSpaceScroll; }


	MemoryImage* GetBackground() { return mBackgroundImage; }

	void SyncState(DataSync &theSync);
};

void SpriteMgrDeleteGlobals();

} // namespace Sexy

#endif