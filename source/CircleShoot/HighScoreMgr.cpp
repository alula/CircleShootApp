#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SexyAppBase.h>

#include "DataSync.h"
#include "HighScoreMgr.h"

using namespace Sexy;

int gHighScoreVersion = 1;

struct DefaultHighScoreInfoEntry
{
    const char *mName;
    int mScore;
};
struct DefaultHighScoreInfo
{
    const char *mMap;
    DefaultHighScoreInfoEntry mScores[3];
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

HighScore::HighScore() : mScore(0)
{
}

void HighScore::SyncState(DataSync &theSync) const
{
    theSync.SyncString((std::string &)mName);
    theSync.SyncLong((int &)mScore);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

HighScoreMgr::HighScoreMgr()
{
}

HighScoreMgr::~HighScoreMgr()
{
}

void HighScoreMgr::Load()
{
    Clear();
    Buffer aBuffer;

    if (!gSexyAppBase->ReadBufferFromFile("userdata/highscores.dat", &aBuffer))
    {
        AddDefaults();
        return;
    }

    DataReader aReader;
    aReader.OpenMemory(aBuffer.GetDataPtr(), aBuffer.GetDataLen(), false);

    DataSync aSync(aReader);
    SyncState(aSync);
}

void HighScoreMgr::Save()
{
    DataWriter aWriter;
    aWriter.OpenMemory(32);

    DataSync aSync(aWriter);
    SyncState(aSync);

    Sexy::MkDir("userdata");
    gSexyAppBase->WriteBytesToFile("userdata/highscores.dat", aWriter.mMemoryHandle, aWriter.mMemoryPosition);
}

void HighScoreMgr::Clear()
{
    mHighScoreMap.clear();
    mLowTimeMap.clear();
}

void HighScoreMgr::CapSize(HighScoreSet &theSet, int theSize)
{
    while ((int)theSet.size() > theSize)
        theSet.erase(--theSet.end());
}

void HighScoreMgr::ClearEmptyScores()
{
    for (HighScoreMap::iterator anItr = mHighScoreMap.begin(); anItr != mHighScoreMap.end(); ++anItr)
    {
        if (anItr->first.empty())
        {
            anItr = mHighScoreMap.erase(anItr);
        }
    }
}

void HighScoreMgr::AddDefaults()
{
    const char *aTimes[] = {
        "Josh",
        "Brian",
        "Juho",
        "Ace",
        "Eric",
        "Ian",
        "Ben",
        "Walter",
        "Nick",
        "Jordin",
        "Scott",
        "Don",
        "Alison",
        "John",
        "Brenna",
        "Kathy",
        "Shanon",
        "Tristan",
        "Dave",
        "Sukhbir",
        "Tysen",
        "Shawn",
        "Jason",
        "Chad",
        "Jeff",
        "Alison"};

    DefaultHighScoreInfo aScores[] = {
        {"adventure", {{"Jordin", 10000}, {"Shanon", 5000}, {"John", 1000}}},
        {"e_random", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"e_spiral", {{"WhatGall", 10000}, {"Arkeri", 5000}, {"Kingdom", 1000}}},
        {"e_claw", {{"Inmost Light", 10000}, {"Wolfpackmama", 5000}, {"jb63", 1000}}},
        {"e_riverbed", {{"Hermitcrab", 10000}, {"Fire", 5000}, {"Dachsx2", 1000}}},
        {"e_targetglyph", {{"Javansdaddy", 10000}, {"WendigoWolf", 5000}, {"Syrinx", 1000}}},
        {"e_blackswirley", {{"Meatboy Dogfood", 10000}, {"MrsDbolt", 5000}, {"Jael", 1000}}},
        {"e_turnaround", {{"Vexrm", 10000}, {"HRACH", 5000}, {"Kane", 1000}}},
        {"e_longrange", {{"Bugsymcd24", 10000}, {"ClaireBear", 5000}, {"Emerald", 1000}}},
        {"e_tiltspiral", {{"Baasi1", 10000}, {"lala", 5000}, {"BlueSky", 1000}}},
        {"e_underover", {{"HomerJay", 10000}, {"FaereMoon", 5000}, {"Cheryl", 1000}}},
        {"e_warshak", {{"Chris", 10000}, {"Numfive", 5000}, {"Darkthought", 1000}}},
        {"e_loopy", {{"RavensTrumpet", 10000}, {"TresserMckay", 5000}, {"Elias", 1000}}},
        {"e_snakepit", {{"Rainsford", 10000}, {"Panther", 5000}, {"Journey", 1000}}},
        {"e_groovefest", {{"Canny", 10000}, {"Caliban", 5000}, {"Reba", 1000}}},
        {"e_spaceinvaders", {{"GrukX", 10000}, {"NPG", 5000}, {"Hottentots", 1000}}},
        {"e_triangle", {{"JustAKidAtHeart", 10000}, {"Illyria", 5000}, {"Mungoon", 1000}}},
        {"e_coaster", {{"Cerebus", 10000}, {"WildKitty", 5000}, {"Deb", 1000}}},
        {"e_squaresville", {{"gregslagel ", 10000}, {"Splam11", 5000}, {"", 1000}}},
        {"e_tunnellevel", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"e_serpents", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"e_overunder", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"e_inversespiral", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"e_space", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"random", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"spiral", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"claw", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"riverbed", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"targetglyph", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"blackswirley", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"turnaround", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"longrange", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"tiltspiral", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"underover", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"warshak", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"loopy", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"snakepit", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"groovefest", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"spaceinvaders", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"triangle", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"coaster", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"squaresville", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"tunnellevel", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"serpents", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"overunder", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"inversespiral", {{"", 10000}, {"", 5000}, {"", 1000}}},
        {"space", {{"", 10000}, {"", 5000}, {"", 1000}}},
    };

    int aNam = 0;
    for (int i = 0; i < sizeof(aScores) / sizeof(DefaultHighScoreInfo); i++)
    {
        HighScoreSet &aSet = mHighScoreMap[aScores[i].mMap];
        for (int j = 0; j < 3; j++)
        {
            HighScore aScore;
            aScore.mName = aScores[i].mScores[j].mName;
            aScore.mScore = aScores[i].mScores[j].mScore;
            if (aScore.mName.empty())
                aScore.mName = aTimes[aNam++ % (sizeof(aTimes) / sizeof(const char *))];
            aSet.insert(aScore);
        }
    }
}

void HighScoreMgr::SyncState(DataSync &theSync)
{
    int aVersion = gHighScoreVersion;
    theSync.SyncLong(aVersion);
    theSync.mVersion = aVersion;

    if (theSync.mReader)
    {
        Clear();
        if (aVersion != gHighScoreVersion)
            return;

        int aNumBoards = theSync.mReader->ReadShort();
        for (int i = 0; i < aNumBoards; i++)
        {
            std::string aBoard;
            theSync.SyncString(aBoard);
            HighScoreSet &aSet = mHighScoreMap[aBoard];

            int aNumScores = theSync.mReader->ReadShort();
            for (int j = 0; j < aNumScores; j++)
            {
                HighScore aScore;
                aScore.SyncState(theSync);
                aSet.insert(aScore);
            }
        }

        int aNumTimes = theSync.mReader->ReadLong();
        for (int i = 0; i < aNumTimes; i++)
        {
            std::string aBoard;
            theSync.SyncString(aBoard);
            LowTime &aTime = mLowTimeMap[aBoard];
            theSync.SyncString(aTime.mName);
            theSync.SyncLong(aTime.mTime);
        }
    }
}

const HighScoreSet &HighScoreMgr::GetHighScores(const std::string &theLevelStr)
{
    return mHighScoreMap[theLevelStr];
}

bool HighScoreMgr::SubmitScore(const std::string &theBoard, const HighScore &theScore, bool needLoad)
{
    HighScoreSet &aSet = mHighScoreMap[theBoard];
    HighScoreSet::iterator anItr = aSet.insert(theScore);

    if (aSet.size() <= 10)
    {
        if (needLoad)
        {
            Load();
            return SubmitScore(theBoard, theScore, false);
        }

        Save();
        return true;
    }

    HighScoreSet::iterator aNextItr = anItr;
    ++aNextItr;

    while (aNextItr != aSet.end())
    {
        if (anItr->mScore != aNextItr->mScore)
            break;

        ++aNextItr;
    }

    if (aNextItr == aSet.end())
    {
        aSet.erase(anItr);
        return false;
    }

    if (needLoad)
    {
        Load();
        return SubmitScore(theBoard, theScore, false);
    }

    CapSize(aSet, 10);
    Save();
    return true;
}

LowTime *HighScoreMgr::GetLowTime(const std::string &theLevelStr)
{
    LowTimeMap::iterator anItr = mLowTimeMap.find(theLevelStr);

    if (anItr == mLowTimeMap.end())
        return NULL;

    return &anItr->second;
}

bool HighScoreMgr::SubmitLowTime(const std::string &theLevelStr, const std::string &theName, unsigned int theTime, bool needLoad)
{
    return false;
}