#include "Zuma_Prefix.pch"

#include "HighScoreMgr.h"

using namespace Sexy;

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