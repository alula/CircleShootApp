#ifndef __HIGHSCOREMGR_H__
#define __HIGHSCOREMGR_H__

#include "CircleCommon.h"

#include <string>
#include <map>
#include <set>
#include <string>

namespace Sexy
{

class DataSync;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class HighScore
{
public:
	std::string mName;
	int mScore;

	HighScore();
	void SyncState(DataSync &theSync) const;	// not really const, see code

	bool operator<(const HighScore &h) const { return mScore > h.mScore; }
	bool operator!=(const HighScore &h) const { return mScore != h.mScore; }
};
typedef std::multiset<HighScore> HighScoreSet;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class LowTime
{
public:
	std::string mName;
	unsigned int mTime;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class HighScoreMgr
{
protected:
	typedef std::map<std::string,HighScoreSet> HighScoreMap;
	HighScoreMap mHighScoreMap;

	typedef std::map<std::string,LowTime> LowTimeMap;
	LowTimeMap mLowTimeMap;

	void CapSize(HighScoreSet &theSet, int theSize);
	void SyncState(DataSync &theSync);
	void ClearEmptyScores();
	void AddDefaults();

public:
	HighScoreMgr();
	virtual ~HighScoreMgr();

	void Clear();
	void Load();
	void Save();

	const HighScoreSet& GetHighScores(const std::string &theBoard);
	bool SubmitScore(const std::string &theBoard, const HighScore &theScore, bool needLoad = true);

	LowTime* GetLowTime(const std::string &theLevelStr);
	bool SubmitLowTime(const std::string &theLevelStr, const std::string &theName, unsigned int theTime, bool needLoad = true);
};


} // namespace Sexy

#endif