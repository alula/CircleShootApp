#ifndef __PARTICLEMGR_H__
#define __PARTICLEMGR_H__

#include "CircleCommon.h"
#include <list>
#include <vector>

namespace Sexy
{

	class DataSync;
	class Graphics;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	struct Sparkle
	{
		float x, y;
		float vx, vy;
		int mUpdateCnt;
		int mFrame;
		int mColor;
		int mDuration;

		void SyncState(DataSync &theSync);
	};

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	struct Explosion
	{
		int x, y;
		int mRadius;
		int mColor;
		int mCurRadius;
		int mCurColor;
		int mUpdateCnt;

		void SyncState(DataSync &theSync);
	};

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	struct FloatingText
	{
		int x, y;
		int mUpdateCnt;
		int mColor;
		int mFontId;
		std::string mText;
		int mScoreInc;
		int mDuration;
		bool mFade;

		void SyncState(DataSync &theSync);
	};

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class ParticleMgr;
	class FloatingTextHelper
	{
	protected:
		struct TextDesc
		{
			std::string mString;
			int mFontId;
			int mWidth;
			int mHeight;
			int mColor;
		};

		std::vector<TextDesc> mText;

	public:
		void AddText(const std::string &theString, int theFontId, int theColor);
		void AddToMgr(ParticleMgr *theMgr, int x, int y, int theStagger = 0, int theScoreInc = 0);
		void Clear() { mText.clear(); }
	};

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class Board;
	class ParticleMgr
	{
	protected:
		Board *mBoard;
		bool mHadUpdate;

		typedef std::list<Sparkle> SparkleList;
		SparkleList mSparkleList[MAX_PRIORITY + 1];

		typedef std::list<Explosion> ExplosionList;
		ExplosionList mExplosionList;

		typedef std::list<FloatingText> FloatingTextList;
		FloatingTextList mFloatingTextList;

		void UpdateSparkles();
		void UpdateExplosions();
		void UpdateFloatingText();

		void DrawSparkles(Graphics *g, int thePriority);
		void DrawExplosions(Graphics *g);
		void DrawFloatingText(Graphics *g);

	public:
		ParticleMgr(Board *theBoard);
		virtual ~ParticleMgr();

		void SyncState(DataSync &theSync);

		void Clear();
		void AddSparkle(float x, float y, int thePriority, int theStagger = 0); // inlined but idk what parameters it passes
		void AddSparkle(float x, float y, float vx, float vy, int thePriority, int theDuration = 0, int theStagger = 0, uint theColor = 0xFFFF00);
		void AddExplosion(int x, int y, int theRadius, int theColor, int theStagger = 0);
		void AddFloatingText(int x, int y, int theColor, const std::string &theText, int theFontId, int theStagger = 0, int theScoreInc = 0, int theDuration = 100, bool fade = false);

		bool Update();
		void Draw(Graphics *g, int thePriority);
		void DrawTopMost(Graphics *g);

		int GetSparkleCount();
	};

} // namespace Sexy

#endif