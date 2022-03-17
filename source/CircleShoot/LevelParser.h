#ifndef __SEXY_LEVELPARSER_H__
#define __SEXY_LEVELPARSER_H__

#include "CircleCommon.h"

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

namespace Sexy
{

	class UserProfile;
	class XMLParser;
	class XMLElement;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class SpriteDesc
	{
	public:
		std::string mImagePath;
		int mPriority;
		int x;
		int y;
		float vx;
		float vy;
		bool mCutout;

		SpriteDesc() : mPriority(0), x(0), y(0), vx(0), vy(0), mCutout(false) {}
	};
	typedef std::list<SpriteDesc> SpriteDescList;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class TreasurePoint
	{
	public:
		int x, y;
		int mCurveDist[3];
	};
	typedef std::list<TreasurePoint> TreasurePointList;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class CurveDesc
	{
	public:
		std::string mPath;
		int mDangerDistance;
		float mSpeed;
		int mStartDistance;
		float mMergeSpeed;
		int mNumBalls;
		int mBallRepeat;
		int mMaxSingle;
		int mNumColors;
		float mSlowFactor;
		float mSlowSpeed;
		int mSlowDistance;
		float mAccelerationRate;
		float mCurAcceleration;
		int mScoreTarget;
		int mSkullRotation;
		int mZumaBack;
		int mZumaSlow;
		float mMaxSpeed;

		int mPowerUpFreq[PowerType_Max];

		unsigned int mCurveColor;
		bool mDrawCurve;
		bool mDrawTunnels;
		bool mDestroyAll;

		CurveDesc();
	};

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class LevelDesc
	{
	public:
		int mId;
		std::string mName;
		std::string mDisplayName;
		std::string mImagePath;
		float mFireSpeed;
		int mReloadDelay;
		int mGunX, mGunY;
		int mBGColor;
		int mDifficulty;
		int mHueRot;
		int mTreasureFreq;
		int mParTime;
		SpriteDescList mSprites;
		SpriteDescList mBackgroundAlpha;
		TreasurePointList mTreasurePoints;
		bool mInSpace;

		int mStage;
		int mLevel;

		CurveDesc mCurveDesc[3];

		LevelDesc();
		int GetNumCurves();
	};
	typedef std::vector<LevelDesc> LevelDescList;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class LevelParser
	{
	public:
		XMLParser *mXMLParser;
		std::string mError;
		bool mHasFailed;
		LevelDescList mLevels;
		std::string mCurDir;

		typedef std::map<std::string, LevelDesc> LevelDescMap;
		LevelDescMap mSettingsMap;
		LevelDescMap mGraphicsMap;

		typedef std::pair<std::string, int> LevelPair;
		typedef std::vector<LevelPair> LevelProgression;
		typedef std::map<std::string, LevelProgression> LevelProgressionMap;
		typedef std::vector<LevelDesc> LevelDescVector;

		typedef std::vector<LevelDescVector> StageMap;
		LevelProgressionMap mLevelProgressionMap;
		StageMap mStageMap;

		typedef std::list<std::string> StringList;
		typedef std::vector<std::string> StringVector;
		typedef std::set<std::string> BoardSet;

		StringVector mBoardProgression;
		BoardSet mBoardSet;

		std::string mCurLevelId;

		std::string GetPath(const std::string &theStr);
		void CopyGraphics(LevelDesc &to, LevelDesc &from);
		bool Fail(const std::string &theErrorText);
		bool DoParseLevels();
		bool DoParseLevel(XMLElement &theElem, bool isLevel);
		bool DoParseSprite(XMLElement &theElem, SpriteDesc &theSprite);
		bool DoParseTreasure(XMLElement &theElem, TreasurePoint &thePoint);
		bool DoParseLevelProgression(XMLElement &theElem);
		bool DoParseStageProgression(XMLElement &theElem, int theStage);
		void CreateBoardProgression();

	public:
		LevelParser();
		virtual ~LevelParser();

		bool ParseLevelFile(const std::string &theFilename);
		std::string GetErrorText();
		bool HadError();

		LevelDesc &GetLevelById(int theId);
		LevelDesc &GetLevelByName(const std::string &theName, int theDifficulty);
		LevelDesc &GetLevel(int theDifficulty, const StringList &theAvoidCurve, const std::string &theFindCurve);
		LevelDesc &GetStageLevel(int theLevelNum);
		void GetPracticeBoard(int theNum, std::string &theName, std::string &theDisplayName);
		LevelDesc &GetSequenceBoard(int theDifficulty, int theLevel, UserProfile *theProfile);
		LevelDesc &GetRandomBoard(int theDifficulty, int theLevel, UserProfile *theProfile, const StringList &theAvoidCurves);
		LevelDesc &GetBackgroundTransition();
		int GetNumBoards() { return (int)mBoardProgression.size() + 1; }

		int GetDifficulty(int theLevel);
		int GetLevelNumByStage(int theStage);
	};

}

#endif
