#ifndef __PROFILEMGR_H__
#define __PROFILEMGR_H__

#include "CircleCommon.h"

#include <string>
#include <map>

namespace Sexy
{
class DataSync;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class UserProfile
{
public:
	std::string mName;
	unsigned int mUseSeq;
	unsigned int mId;
	int mLastPracticeBoard;
	bool mPracticeEndless;
	int mPracticeDifficulty;
	int mMaxStage;
	int mMaxLevel;
	bool mShowHelpScreen;
	bool mHasWon;
	bool mNeedGauntletBlink;

	typedef std::map<std::string,int> BoardMap;
	BoardMap mMaxBoardLevel;

	
	UserProfile();
	void SyncSummary(DataSync &theSync);
	void SyncDetails(DataSync &theSync);
	void ClearDetails();
	void DeleteUserFiles();

	void LoadDetails();
	void SaveDetails();
	bool UpdateMaxLevel(const std::string &theBoard, int theLevel);
};

typedef std::map<std::string,UserProfile,StringLessNoCase> ProfileMap;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class ProfileMgr
{
protected:
	ProfileMap mProfileMap;
	unsigned int mNextProfileId;
	unsigned int mNextProfileUseSeq;

	void SyncState(DataSync &theSync);

	void DeleteOldestProfile();
	void DeleteOldProfiles();
	void DeleteProfile(ProfileMap::iterator theItr);

public:
	ProfileMgr();
	virtual ~ProfileMgr();

	void Clear();
	void Load();
	void Save();

	int GetNumProfiles() { return (int)mProfileMap.size(); }

	UserProfile* GetProfile(const std::string &theName);
	UserProfile* AddProfile(const std::string &theName);
	UserProfile* GetAnyProfile();
	bool DeleteProfile(const std::string &theName);
	bool RenameProfile(const std::string &theOldName, const std::string &theNewName);

	const ProfileMap& GetProfileMap() { return mProfileMap; }
};

} // namespace Sexy

#endif