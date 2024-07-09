#include "Zuma_Prefix.pch"

#include <SexyAppFramework/Common.h>
#include <SexyAppFramework/SexyAppBase.h>

#include "ProfileMgr.h"
#include "DataSync.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int gProfileVersion = 4;

UserProfile::UserProfile() : mId(0), mUseSeq(0)
{
    ClearDetails();
}

void UserProfile::SyncSummary(DataSync &theSync)
{
    theSync.SyncString(mName);
    theSync.SyncLong(mUseSeq);
    theSync.SyncLong(mId);
}

void UserProfile::SyncDetails(DataSync &theSync)
{
    unsigned int aVersion = gProfileVersion;
    theSync.SyncLong(aVersion);

    if (aVersion != gProfileVersion)
        return;

    theSync.SyncShort(mLastPracticeBoard);
    theSync.SyncByte(mPracticeDifficulty);
    theSync.SyncShort(mMaxStage);
    theSync.SyncShort(mMaxLevel);
    theSync.SyncBool(mShowHelpScreen);
    theSync.SyncBool(mPracticeEndless);
    theSync.SyncBool(mHasWon);
    theSync.SyncBool(mNeedGauntletBlink);

    if (theSync.mReader)
    {
        mMaxBoardLevel.clear();

        int aCount = theSync.mReader->ReadShort();

        for (int i = 0; i < aCount; i++)
        {
            std::string aLevel;
            theSync.mReader->ReadString(aLevel);
            mMaxBoardLevel[aLevel] = theSync.mReader->ReadShort();
        }
    }
    else
    {
        theSync.mWriter->WriteShort(mMaxBoardLevel.size());

        for (std::map<std::string, int>::iterator i = mMaxBoardLevel.begin(); i != mMaxBoardLevel.end(); i++)
        {
            theSync.mWriter->WriteString(i->first);
            theSync.mWriter->WriteShort(i->second);
        }
    }
}

void UserProfile::ClearDetails()
{
    mLastPracticeBoard = 1;
    mPracticeDifficulty = 0;
    mMaxStage = 0;
    mMaxLevel = 0;
    mShowHelpScreen = true;
    mPracticeEndless = true;
    mHasWon = false;
    mNeedGauntletBlink = false;

    mMaxBoardLevel.clear();
    mMaxBoardLevel["random"] = 0;
    mMaxBoardLevel["sequence"] = 0;
    mMaxBoardLevel["spiral"] = 0;
    mMaxBoardLevel["claw"] = 0;
}

void UserProfile::DeleteUserFiles()
{
    // TODO: Mac version uses Application Support folder

    gSexyAppBase->EraseFile(GetSaveGameName(false, mId));
    gSexyAppBase->EraseFile(GetSaveGameName(true, mId));
    gSexyAppBase->EraseFile(Sexy::StrFormat("userdata/user%d.dat", mId));
}

void UserProfile::LoadDetails()
{
    // TODO: Mac version uses Application Support folder
    Buffer aBuf;

    // yes, windows version used forward slashes
    std::string aFileName = Sexy::StrFormat("userdata/user%d.dat", mId);
    if (gSexyAppBase->ReadBufferFromFile(aFileName, &aBuf))
    {
        DataReader aReader;
        aReader.OpenMemory((void *)aBuf.GetDataPtr(), aBuf.GetDataLen(), false);

        DataSync aSync(aReader);
        SyncDetails(aSync);
    }
}

void UserProfile::SaveDetails()
{
    DataWriter aWriter;
    aWriter.OpenMemory(32);
    DataSync aSync(aWriter);
    SyncDetails(aSync);

    // TODO: Mac version uses Application Support folder
    Sexy::MkDir("userdata");

    std::string aFileName = Sexy::StrFormat("userdata/user%d.dat", mId);
    gSexyAppBase->WriteBytesToFile(aFileName, aWriter.mMemoryHandle, aWriter.mMemoryPosition);
}

bool UserProfile::UpdateMaxLevel(const std::string &theBoard, int theLevel)
{
    std::pair<BoardMap::iterator, bool> anEntry = mMaxBoardLevel.insert(std::make_pair(theBoard, theLevel));
    if (!anEntry.second)
    {
        if (anEntry.first->second >= theLevel)
            return false;

        anEntry.first->second = theLevel;

        if (theLevel > 0 && (theLevel - 1) / 7 < theLevel / 7 && theLevel % 7 <= 3)
            mNeedGauntletBlink = true;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ProfileMgr::ProfileMgr()
{
    Clear();
}

ProfileMgr::~ProfileMgr()
{
}

void ProfileMgr::Clear()
{
    mProfileMap.clear();
    mNextProfileId = 1;
    mNextProfileUseSeq = 1;
}

void ProfileMgr::Load()
{
    // TODO: Mac version uses Application Support folder
    Buffer aBuf;

    if (gSexyAppBase->ReadBufferFromFile("userdata/users.dat", &aBuf))
    {
        DataReader aReader;
        aReader.OpenMemory((void *)aBuf.GetDataPtr(), aBuf.GetDataLen(), false);

        DataSync aSync(aReader);
        SyncState(aSync);
    }
}

void ProfileMgr::Save()
{
    DataWriter aWriter;
    aWriter.OpenMemory(32);

    DataSync aSync(aWriter);
    SyncState(aSync);

    // TODO: Mac version uses Application Support folder
    gSexyAppBase->WriteBytesToFile("userdata/users.dat", aWriter.mMemoryHandle, aWriter.mMemoryPosition);
}

UserProfile *ProfileMgr::GetProfile(const std::string &theName)
{
    ProfileMap::iterator iter = mProfileMap.find(theName);

    if (iter == mProfileMap.end())
    {
        return NULL;
    }

    UserProfile *profile = &iter->second;

    profile->LoadDetails();
    profile->mUseSeq = mNextProfileUseSeq;

    ++mNextProfileUseSeq;

    return profile;
}

UserProfile *ProfileMgr::AddProfile(const std::string &theName)
{
    std::pair<ProfileMap::iterator, bool> anEntry = mProfileMap.insert(std::make_pair(theName, UserProfile()));
    if (!anEntry.second)
    {
        return NULL;
    }

    UserProfile *profile = &anEntry.first->second;
    profile->mName = theName;
    profile->mId = mNextProfileId++;
    profile->mUseSeq = mNextProfileUseSeq++;

    DeleteOldProfiles();

    return profile;
}

UserProfile *ProfileMgr::GetAnyProfile()
{
    if (mProfileMap.size() == 0)
    {
        return NULL;
    }

    UserProfile *profile = &mProfileMap.begin()->second;

    profile->LoadDetails();
    profile->mUseSeq = mNextProfileUseSeq;

    ++mNextProfileUseSeq;

    return profile;
}

void ProfileMgr::SyncState(DataSync &theSync)
{
    unsigned int aVersion = gProfileVersion;
    theSync.SyncLong(aVersion);

    if (aVersion != gProfileVersion)
        return;

    if (theSync.mReader)
    {
        mProfileMap.clear();

        int aCount = theSync.mReader->ReadShort();
        int aUseSeq = 0;
        int aId = 0;

        for (int i = 0; i < aCount; i++)
        {
            UserProfile aProfile;
            aProfile.SyncSummary(theSync);

            if (aUseSeq < aProfile.mUseSeq)
                aUseSeq = aProfile.mUseSeq;

            if (aId < aProfile.mId)
                aId = aProfile.mId;

            mProfileMap[aProfile.mName] = aProfile;
        }

        mNextProfileId = aId + 1;
        mNextProfileUseSeq = aUseSeq + 1;
    }
    else
    {
        theSync.mWriter->WriteShort(mProfileMap.size());

        for (ProfileMap::iterator i = mProfileMap.begin(); i != mProfileMap.end(); i++)
        {
            i->second.SyncSummary(theSync);
        }
    }
}

void ProfileMgr::DeleteOldestProfile()
{
    if (mProfileMap.empty())
        return;

    ProfileMap::iterator anItr = mProfileMap.begin();
    for (ProfileMap::iterator i = mProfileMap.begin(); i != mProfileMap.end(); i++)
    {
        if (i->second.mUseSeq < anItr->second.mUseSeq)
        {
            anItr = i;
        }
    }

    anItr->second.DeleteUserFiles();
    mProfileMap.erase(anItr);
}

void ProfileMgr::DeleteOldProfiles()
{
    while (mProfileMap.size() > 200)
    {
        DeleteOldestProfile();
    }
}

void ProfileMgr::DeleteProfile(ProfileMap::iterator theItr)
{
    theItr->second.DeleteUserFiles();
    mProfileMap.erase(theItr);
}

bool ProfileMgr::DeleteProfile(const std::string &theName)
{
    ProfileMap::iterator anItr = mProfileMap.find(theName);

    if (anItr == mProfileMap.end())
    {
        return false;
    }

    DeleteProfile(anItr);
    return true;
}

bool ProfileMgr::RenameProfile(const std::string &theOldName, const std::string &theNewName)
{
    ProfileMap::iterator anItr = mProfileMap.find(theOldName);

    if (anItr == mProfileMap.end())
    {
        return false;
    }

    if (stricmp(theOldName.c_str(), theNewName.c_str()) != 0)
    {
        std::pair<ProfileMap::iterator, bool> anInsert = mProfileMap.insert(std::make_pair(theNewName, anItr->second));
        if (!anInsert.second)
        {
            return false;
        }

        mProfileMap.erase(anItr);
        anItr = anInsert.first;
    }

    anItr->second.mName = theNewName;
    return true;
}