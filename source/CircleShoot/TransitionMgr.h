#ifndef __TRANSITIONMGR_H__
#define __TRANSITIONMGR_H__

#include <SexyAppFramework/Point.h>

#include "CircleCommon.h"

extern bool gForceStageComplete; //Fix broken cheat

namespace Sexy
{

class Board;
class DataSync;
class Graphics;
class MemoryImage;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct FrogMove
{
	int mUpdateCnt;
	int mDuration;
	IntPoint mStartPos;
	IntPoint mEndPos;

	void SyncState(DataSync &theSync);
};
typedef std::list<FrogMove> FrogMoveList;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct FrogScale
{
	int mUpdateCnt;
	int mDuration;
	float mStartScale;
	float mEndScale;

	void SyncState(DataSync &theSync);
};
typedef std::list<FrogScale> FrogScaleList;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct LetterStamp
{
	int mUpdateCnt;
	IntPoint mPos;
	std::string mLetter;
	ulong mColor;
	int mFont;
	int mDuration;
	MemoryImage *mImage;

	LetterStamp();
	virtual ~LetterStamp();

	void SyncState(DataSync &theSync);
	void CreateImage();
};
typedef std::list<LetterStamp> LetterStampList;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct TextBlurb
{
	int mCharNum;
	int mDelayCnt;
	IntPoint mPos;
	std::string mText;
	DWORD mColor;
	int mFont;

	void SyncState(DataSync &theSync);
};
typedef std::list<TextBlurb> TextBlurbList;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
enum TransitionState
{
	TransitionState_None,
	TransitionState_LevelBegin,
	TransitionState_Bonus,
	TransitionState_StageComplete,
	TransitionState_TempleComplete,
	TransitionState_Quake,
	TransitionState_Losing
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class TransitionMgr
{
protected:
	Board *mBoard;
	FrogMoveList mFrogMoveList;
	FrogScaleList mFrogScaleList;
	LetterStampList mLetterStampList;
	TextBlurbList mTextBlurbList;

	float mCurFrogScale;
	float mCurFrogAngle;
	int mQuakeFrame;
	int mStateCount;
	int mStageCompleteFrame;
	int mLevelFade;
	int mTargetLevelFade;
	int mLevelFadeInc;
	int mResetFrame;
	bool mDoStageUp;
	bool mDoTempleUp;
	TransitionState mState;

	void UpdateFrogScale();
	void UpdateFrogMove();
	void UpdateLetterStamp();
	void UpdateLevelBegin();
	void UpdateBonus();
	void UpdateQuake();
	void UpdateStageComplete();
	void UpdateTempleComplete();
	void UpdateLosing();
	void UpdateTextBlurb();
	void FinishAllTextBlurbs();


	void DrawFrog(Graphics *g);
	void DrawQuake(Graphics *g);
	void DrawDefault(Graphics *g);
	void DrawLetterStamp(Graphics *g);
	void DrawTextBlurb(Graphics *g);
	void DrawStageCompleteText(Graphics *g);
	void DrawStageComplete(Graphics *g);
	void DrawLevelBegin(Graphics *g);
	void DrawLevelFade(Graphics *g);
	void DrawLosing(Graphics *g);
	void DrawTempleComplete(Graphics *g);

public:
	TransitionMgr(Board *theBoard);
	virtual ~TransitionMgr();

	void Update();
	void Draw(Graphics *g);
	void Clear();
	void SyncState(DataSync &theSync);

	void AddFrogMove(const IntPoint &theStartPos, const IntPoint &theEndPos, int theDuration = 0, int theStagger = 0);
	void AddFrogScale(float theStartScale, float theEndScale, int theDuration, int theStagger = 0);
	void AddLetterStamp(char theChar, int theX, int theY, DWORD theColor, int theStagger = 0);
	void AddLetterStamp(const std::string &theStr, int theX, int theY, DWORD theColor, int theStagger = 0, int theFontId = -1, int theDuration = 0);
	int AddTextBlurb(const std::string &theBlurb, int theX, int theY, int theStagger = 0);
	int AddTextBlurbWrap(const std::string &theBlurb, int theX, int theY, int theMaxWidth, int theStagger = 0);

	void DoLevelBegin(bool firstTime);
	void DoQuake();
	void DoStageComplete();
	void DoTempleComplete();
	void DoLevelUp();
	void DoLosing();
	void MouseDown(int x, int y, int theClickCount);
};

}

#endif