#include "Zuma_Prefix.pch"

#include <cstddef>

#include <SexyAppFramework/XMLParser.h>

#include "Board.h"
#include "LevelParser.h"
#include "ProfileMgr.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CurveDesc::CurveDesc()
{
    mSpeed = 0.5;
    mStartDistance = 40;
    mNumColors = 4;
    mNumBalls = 0;
    mBallRepeat = 40;
    mMaxSingle = 10;
    mMergeSpeed = 0.05;
    mDangerDistance = 600;
    mAccelerationRate = 0;
    mMaxSpeed = 100;
    mScoreTarget = 1000;
    mSkullRotation = -1;
    mCurAcceleration = 0;

    for (int i = 0; i < PowerType_Max; i++)
    {
        mPowerUpFreq[i] = 3000;
    }

    mSlowFactor = 4;
    mSlowSpeed = 0;
    mSlowDistance = 500;
    mZumaBack = 300;
    mZumaSlow = 1100;
    mDrawCurve = false;
    mDrawTunnels = true;
    mDestroyAll = true;
    mCurveColor = 0xffffffff;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

LevelDesc::LevelDesc()
{
    mDifficulty = 0;
    mHueRot = 0;
    mGunX = 320;
    mGunY = 240;
    mFireSpeed = 6.0;
    mReloadDelay = 0;
    mBGColor = -1;
    mTreasureFreq = 300;
    mParTime = 0;
    mInSpace = false;
    mLevel = 0;
    mStage = 0;
}

int LevelDesc::GetNumCurves()
{
    int nCurves = 0;

    for (int i = 0; i < 3; i++)
    {
        if (mCurveDesc[i].mPath.length() != 0)
            nCurves++;
    }

    return nCurves;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

LevelParser::LevelParser() : mHasFailed(false), mXMLParser(NULL)
{
}

LevelParser::~LevelParser()
{
}

LevelDesc &LevelParser::GetLevel(int theDifficulty, const StringList &theAvoidCurve, const std::string &theFindCurve)
{
    std::vector<LevelDesc *> aLevelsDifficulty;
    std::vector<LevelDesc *> aLevelList;
    int aDifficulty = 0;
	//int i;
    LevelDesc *aLevel = NULL;

    for (int i = 0; i < mLevels.size(); ++i)
    {
        LevelDesc *theLevel = &mLevels[i];

		if (!theFindCurve.empty() && theFindCurve.compare(theLevel->mName) != 0) continue;
        
        if (theLevel->mDifficulty > theDifficulty && (aLevel == NULL || aLevel->mDifficulty > theLevel->mDifficulty))
        {
            aLevel = theLevel;
        }

        if (theLevel->mDifficulty == aDifficulty)
        {
            aLevelsDifficulty.push_back(theLevel);
        }
        else if (theLevel->mDifficulty > aDifficulty && theLevel->mDifficulty <= theDifficulty)
        {
            aLevelsDifficulty.clear();
            aLevelsDifficulty.push_back(theLevel);
            aDifficulty = theLevel->mDifficulty;
        }
    }

    if (!theAvoidCurve.empty())
    {
        for (int i = 0; i < aLevelsDifficulty.size(); i++)
        {
            LevelDesc *theLevel = aLevelsDifficulty[i];

            if (std::find(theAvoidCurve.begin(), theAvoidCurve.end(), theLevel->mName) == theAvoidCurve.end())
            {
                aLevelList.push_back(theLevel);
            }
        }
    }

    if (aLevelList.empty())
        aLevelList = aLevelsDifficulty;

    if (!aLevelList.empty())
    {
        int aRnd = Sexy::AppRand();
		aRnd %= (int)aLevelList.size();

        std::vector<LevelDesc *>::iterator anItr = aLevelList.begin();

        // what the fuck???
        for (int i = 0; i < aRnd; i++)
            anItr++;

        return **anItr;
    }

    if (aLevel == NULL)
        return mLevels[0];

    return *aLevel;
}

LevelDesc &LevelParser::GetStageLevel(int theLevelNum)
{
    int aLevelCounter = 0;

    for (int i = 0; i < mStageMap.size(); i++)
    {
        LevelDescList &aLevelList = mStageMap[i];

        if (aLevelCounter + aLevelList.size() > theLevelNum)
        {
            return aLevelList[theLevelNum - aLevelCounter];
        }

        aLevelCounter += aLevelList.size();
    }

    return mLevels[0];
}

void LevelParser::GetPracticeBoard(int theNum, std::string &theName, std::string &theDisplayName)
{
    if (mBoardProgression.empty())
    {
        theName = mLevels[0].mName;
        theDisplayName = mLevels[0].mDisplayName;
    }
    else
    {
        int aLevelId = theNum % (mBoardProgression.size() + 1);
        if (aLevelId == 0)
        {
            theName = "random";
            theDisplayName = "Random";
        }
        else
        {
            StringList anAvoidList;
            LevelDesc &aDesc = GetLevel(0, anAvoidList, mBoardProgression[aLevelId - 1]);

            theName = aDesc.mName;
            theDisplayName = aDesc.mDisplayName;
        }
    }
}

LevelDesc &LevelParser::GetLevelById(int theId)
{
    for (int i = 0; i < mLevels.size(); i++)
    {
        LevelDesc &theLevel = mLevels[i];

        if (theLevel.mId == theId)
            return theLevel;
    }

    return mLevels[0];
}

LevelDesc &LevelParser::GetLevelByName(const std::string &theName, int theDifficulty)
{
    return GetLevel(theDifficulty, StringList(), theName);
}

LevelDesc &LevelParser::GetSequenceBoard(int theDifficulty, int theLevel, UserProfile *theProfile)
{
    StringVector v12;

    for (int i = 0; i < mBoardProgression.size(); i++)
    {
        if (theProfile->mMaxBoardLevel.find(mBoardProgression[i]) != theProfile->mMaxBoardLevel.end())
        {
            v12.push_back(mBoardProgression[i]);
        }
    }

    if (v12.empty())
    {
        v12.push_back("spiral");
    }

    StringList anAvoidCurves;

    return GetLevel(theDifficulty, anAvoidCurves, v12[theLevel % v12.size()]);
}

LevelDesc &LevelParser::GetRandomBoard(int theDifficulty, int theLevel, UserProfile *theProfile, const StringList &theAvoidCurves)
{
    UserProfile::BoardMap aMap(theProfile->mMaxBoardLevel);

    aMap.erase("random");
    aMap.erase("sequence");

    if (aMap.empty())
    {
        return mLevels[0];
    }
    else
    {
        StringList anAvoidCurves;
        int aRnd = Sexy::ThreadRand() % aMap.size();

        UserProfile::BoardMap::iterator anItr = aMap.begin();
        for (int i = 0; i < aRnd; i++)
            anItr++;

        LevelDesc *aDesc = &GetLevel(theDifficulty, anAvoidCurves, anItr->first);
        if (!theAvoidCurves.empty())
        {
            aMap.erase(theAvoidCurves.front());

            if (!aMap.empty())
            {
                int aRnd = Sexy::ThreadRand() % aMap.size();

                UserProfile::BoardMap::iterator anItr = aMap.begin();
                for (int i = 0; i < aRnd; i++)
                    anItr++;

                aDesc = &GetLevel(theDifficulty, theAvoidCurves, anItr->first);
            }
        }

        for (StringList::const_iterator anItr = theAvoidCurves.begin(); anItr != theAvoidCurves.end(); anItr++)
        {
            aMap.erase(*anItr);
        }

        if (!aMap.empty())
        {
            int aRnd = Sexy::ThreadRand() % aMap.size();

            UserProfile::BoardMap::iterator anItr = aMap.begin();
            for (int i = 0; i < aRnd; i++)
                anItr++;

            aDesc = &GetLevel(theDifficulty, theAvoidCurves, anItr->first);
        }

        return *aDesc;
    }
}

bool LevelParser::ParseLevelFile(const std::string &theFilename)
{
    mLevels.clear();
    mXMLParser = new XMLParser();
    mXMLParser->OpenFile(theFilename);

    XMLElement anElement;
    while (!mXMLParser->HasFailed())
    {
        if (!mXMLParser->NextElement(&anElement))
        {
            Fail(mXMLParser->GetErrorText());
        }

        if (anElement.mType == XMLElement::TYPE_START)
        {
            if (anElement.mValue == "Levels")
            {
                return DoParseLevels();
            }

            break;
        }
    }

    Fail("Expecting Levels tag");

    return DoParseLevels();
}

bool LevelParser::Fail(const std::string &theErrorText)
{
    if (!mHasFailed)
    {
        mHasFailed = true;
        if (mXMLParser != NULL)
        {
            int aLineNum = mXMLParser->GetCurrentLineNum();

            char aNum[16];
            sprintf(aNum, "%d", aLineNum); // ...not using Sexy::StrFormat()?

            mError = theErrorText;
            if (aLineNum > 0)
            {
                mError += " on Line " + std::string(aNum);
            }

            if (!mXMLParser->GetFileName().empty())
            {
                mError += " in File " + mXMLParser->GetFileName();
            }
        }
        else
        {
            mError = theErrorText;
        }
    }

    return false;
}

std::string LevelParser::GetErrorText()
{
    return mError;
}

std::string LevelParser::GetPath(const std::string &theStr)
{
    if (theStr.find('/', 0) == -1 && theStr.find('\\', 0) == -1)
    {
        return mCurDir + theStr;
    }
    else
    {
        return theStr;
    }
}

void LevelParser::CopyGraphics(LevelDesc &to, LevelDesc &from)
{

    for (int i = 0; i < 3; i++)
    {
        to.mCurveDesc[i].mPath = from.mCurveDesc[i].mPath;
        to.mCurveDesc[i].mDangerDistance = from.mCurveDesc[i].mDangerDistance;
        to.mCurveDesc[i].mDrawCurve = from.mCurveDesc[i].mDrawCurve;
        to.mCurveDesc[i].mDrawTunnels = from.mCurveDesc[i].mDrawTunnels;
        to.mCurveDesc[i].mCurveColor = from.mCurveDesc[i].mCurveColor;
        to.mCurveDesc[i].mSkullRotation = from.mCurveDesc[i].mSkullRotation;
    }

    to.mGunX = from.mGunX;
    to.mGunY = from.mGunY;
    to.mBGColor = from.mBGColor;
    to.mImagePath = from.mImagePath;
    to.mSprites = from.mSprites;
    to.mBackgroundAlpha = from.mBackgroundAlpha;
    to.mTreasurePoints = from.mTreasurePoints;
    to.mName = from.mName;
    to.mDisplayName = from.mDisplayName;
    to.mInSpace = from.mInSpace;
}

bool LevelParser::DoParseLevels()
{
    if (!mXMLParser->HasFailed())
    {
        bool aInGraphics = false;

        for (;;)
        {
            XMLElement anElement;

            if (!mXMLParser->NextElement(&anElement))
                break;

            if (anElement.mType == XMLElement::TYPE_START)
            {
                if (aInGraphics)
                {
                    if (anElement.mValue == "Sprite")
                    {
                        LevelDesc &aDesc = mLevels.back();
                        aDesc.mSprites.push_back(SpriteDesc());

                        if (!DoParseSprite(anElement, aDesc.mSprites.back()))
                            return false;

                        aInGraphics = true;
                        continue;
                    }
                    else if (anElement.mValue == "BackgroundAlpha")
                    {
                        LevelDesc &aDesc = mLevels.back();
                        aDesc.mBackgroundAlpha.push_back(SpriteDesc());

                        if (!DoParseSprite(anElement, aDesc.mBackgroundAlpha.back()))
                            return false;

                        aInGraphics = true;
                        continue;
                    }
                    else if (anElement.mValue == "Cutout")
                    {
                        LevelDesc &aDesc = mLevels.back();
                        aDesc.mSprites.push_back(SpriteDesc());

                        SpriteDesc &aSprite = aDesc.mSprites.back();
                        aSprite.mCutout = true;
                        if (!DoParseSprite(anElement, aSprite))
                            return false;

                        aInGraphics = true;
                        continue;
                    }
                    else if (anElement.mValue == "TreasurePoint")
                    {
                        LevelDesc &aDesc = mLevels.back();
                        aDesc.mTreasurePoints.push_back(TreasurePoint());

                        if (!DoParseTreasure(anElement, aDesc.mTreasurePoints.back()))
                            return false;
                    }
                    else
                    {
                        Fail("Invalid Section '" + anElement.mValue + "'");
                        break;
                    }
                }
                else if (anElement.mValue == "Level" || anElement.mValue == "Graphics" || anElement.mValue == "Settings")
                {
                    bool isLevel = anElement.mValue == "Level";
                    if (!DoParseLevel(anElement, isLevel))
                        return false;

                    aInGraphics = true;
                }
                else if (anElement.mValue == "LevelProgression")
                {
                    if (!DoParseLevelProgression(anElement))
                        return false;
                }
                else if (anElement.mValue == "StageProgression")
                {
                    for (int i = 0; i < 14; i++)
                    {
                        if (!DoParseStageProgression(anElement, i))
                            return false;
                    }

                    CreateBoardProgression();
                }
                else
                {
                    Fail("Invalid Section '" + anElement.mValue + "'");
                    break;
                }
            }
            else if (anElement.mType == XMLElement::TYPE_END)
            {
                if (!aInGraphics)
                    continue;

                if (anElement.mValue == "Level")
                {
                    aInGraphics = false;
                }
                else if (anElement.mValue == "Settings")
                {
                    aInGraphics = false;
                    mSettingsMap[mCurLevelId] = mLevels.back();
                    mLevels.pop_back();
                }
                else if (anElement.mValue == "Graphics")
                {
                    aInGraphics = false;
                    mGraphicsMap[mCurLevelId] = mLevels.back();
                    mLevels.pop_back();
                }
            }
            else if (anElement.mType == XMLElement::TYPE_ELEMENT)
            {
                Fail("Element Not Expected '" + anElement.mValue + "'");
                break;
            }
        }
    }

    for (int i = 0; i < mLevels.size(); i++)
    {
        mLevels[i].mId = i;
    }

    if (mXMLParser->HasFailed())
    {
        Fail(mXMLParser->GetErrorText());
    }

    delete mXMLParser;
    mXMLParser = NULL;

    return !mHasFailed;
}

bool GetAttribute(XMLElement &theElem, const std::string &theName, std::string &theValue)
{
    XMLParamMap::iterator anItr = theElem.mAttributes.find(theName);
    if (anItr != theElem.mAttributes.end())
    {
        theValue = anItr->second;
        return true;
    }

    return false;
}

void ParseFloat(const char *theBuf, void *theValue)
{
    sscanf(theBuf, "%f", (float *)theValue);
}

void ParseInt(const char *theBuf, void *theValue)
{
    sscanf(theBuf, "%d", (int *)theValue);
}

void ParseHex(const char *theBuf, void *theValue)
{
    sscanf(theBuf, "%x", (int *)theValue);
}

void ParseBool(const char *theBuf, void *theValue)
{
    *(bool *)theValue = (stricmp(theBuf, "true") == 0);
}

void GetCurveAttribute(
    LevelDesc &theDesc,
    XMLElement &theElem,
    const std::string &theName,
    int theOffset,
    void (*theParser)(const char *, void *))
{
    XMLParamMap::iterator anItr = theElem.mAttributes.lower_bound(theName);
    if (anItr == theElem.mAttributes.end())
        return;

    while (anItr != theElem.mAttributes.end())
    {
        std::string aName = anItr->first;
        if (Sexy::StrFindNoCase(aName.c_str(), theName.c_str()) != 0)
        {
            return;
        }

        int aNameSize = aName.size();
        int aDigit;
        int aStartCurve;
        int aEndCurve;

        if (aNameSize != 0 && (aDigit = aName[aNameSize - 1] - '0', aDigit <= 9))
        {
            aEndCurve = aDigit;
            aName.resize(aNameSize - 1);
            aStartCurve = aEndCurve;
        }
        else
        {
            aStartCurve = 0;
            aEndCurve = 2;
        }

        for (int i = aStartCurve; i < aEndCurve; i++)
        {
            char *aPtr = ((char *)&theDesc.mCurveDesc[i]) + theOffset;
            theParser(anItr->second.c_str(), (void *)aPtr);
        }

        anItr++;
    }
}

bool LevelParser::DoParseLevel(XMLElement &theElem, bool isLevel)
{
    LevelDesc aDesc;
    mCurLevelId = theElem.mAttributes["id"];

    std::string aDir;
    if (GetAttribute(theElem, "dir", aDir))
    {
        mCurDir = aDir;
    }
    else
    {
        mCurDir = "levels/" + mCurLevelId;
    }

    mCurDir = Sexy::RemoveTrailingSlash(mCurDir);

    if (!mCurDir.empty())
        mCurDir.push_back('/');

    std::string aName;
    GetAttribute(theElem, "name", aName);

    std::string aDisplayName;
    GetAttribute(theElem, "dispname", aDisplayName);

    std::string aVal;
    if (GetAttribute(theElem, "settings", aVal))
    {
        LevelDescMap::iterator anItr = mSettingsMap.find(aVal);

        if (anItr == mSettingsMap.end())
            return Fail("Settings not found: " + aVal);

        aDesc = anItr->second;
    }

    for (int i = 0; i < 3; i++)
    {
        if (GetAttribute(theElem, Sexy::StrFormat("settings%d", i + 1), aVal))
        {
            LevelDescMap::iterator anItr = mSettingsMap.find(aVal);

            if (anItr == mSettingsMap.end())
                return Fail("Settings not found: " + aVal);

            aDesc.mCurveDesc[i] = anItr->second.mCurveDesc[i];
        }
    }

    if (GetAttribute(theElem, "graphics", aVal))
    {
        if (aName.empty())
            aName = aVal;

        if (aDisplayName.empty())
            aDisplayName = aName;

        LevelDescMap::iterator anItr = mGraphicsMap.find(aVal);

        if (anItr == mGraphicsMap.end())
            return Fail("Graphics not found: " + aVal);

        CopyGraphics(aDesc, anItr->second);
    }

    if (GetAttribute(theElem, "bgcolor", aVal))
        sscanf(aVal.c_str(), "%x", &aDesc.mBGColor);

    if (GetAttribute(theElem, "gx", aVal))
        sscanf(aVal.c_str(), "%d", &aDesc.mGunX);

    if (GetAttribute(theElem, "gy", aVal))
        sscanf(aVal.c_str(), "%d", &aDesc.mGunY);

    if (GetAttribute(theElem, "firespeed", aVal))
        sscanf(aVal.c_str(), "%f", &aDesc.mFireSpeed);

    if (GetAttribute(theElem, "reloaddelay", aVal))
        sscanf(aVal.c_str(), "%d", &aDesc.mReloadDelay);

    if (GetAttribute(theElem, "difficulty", aVal))
        sscanf(aVal.c_str(), "%d", &aDesc.mDifficulty);

    if (GetAttribute(theElem, "huerot", aVal))
        sscanf(aVal.c_str(), "%d", &aDesc.mHueRot);

    if (GetAttribute(theElem, "treasurefreq", aVal))
        sscanf(aVal.c_str(), "%d", &aDesc.mTreasureFreq);

    if (GetAttribute(theElem, "partime", aVal))
        sscanf(aVal.c_str(), "%d", &aDesc.mParTime);

    if (GetAttribute(theElem, "image", aVal))
        aDesc.mImagePath = GetPath(aVal);

    if (GetAttribute(theElem, "space", aVal))
        aDesc.mInSpace = stricmp(aVal.c_str(), "true") == 0;

    int aPowerFreq = 0;
    if (GetAttribute(theElem, "powerfreq", aVal) && sscanf(aVal.c_str(), "%d", &aPowerFreq) == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (aDesc.mCurveDesc[i].mPowerUpFreq[j] > 0)
                    aDesc.mCurveDesc[i].mPowerUpFreq[j] = aPowerFreq;
            }
        }
    }

    if (GetAttribute(theElem, "curve", aVal))
        aDesc.mCurveDesc[0].mPath = GetPath(aVal);

    if (GetAttribute(theElem, "curve1", aVal))
        aDesc.mCurveDesc[0].mPath = GetPath(aVal);

    if (GetAttribute(theElem, "curve2", aVal))
        aDesc.mCurveDesc[1].mPath = GetPath(aVal);

    if (GetAttribute(theElem, "curve3", aVal))
        aDesc.mCurveDesc[2].mPath = GetPath(aVal);

    GetCurveAttribute(aDesc, theElem, "curvecolor", offsetof(CurveDesc, mCurveColor), ParseHex);
    GetCurveAttribute(aDesc, theElem, "drawcurve", offsetof(CurveDesc, mDrawCurve), ParseBool);
    GetCurveAttribute(aDesc, theElem, "drawtunnels", offsetof(CurveDesc, mDrawTunnels), ParseBool);
    GetCurveAttribute(aDesc, theElem, "destroyall", offsetof(CurveDesc, mDestroyAll), ParseBool);
    GetCurveAttribute(aDesc, theElem, "speed", offsetof(CurveDesc, mSpeed), ParseFloat);
    GetCurveAttribute(aDesc, theElem, "maxspeed", offsetof(CurveDesc, mMaxSpeed), ParseFloat);
    GetCurveAttribute(aDesc, theElem, "start", offsetof(CurveDesc, mStartDistance), ParseInt);
    GetCurveAttribute(aDesc, theElem, "colors", offsetof(CurveDesc, mNumColors), ParseInt);
    GetCurveAttribute(aDesc, theElem, "balls", offsetof(CurveDesc, mNumBalls), ParseInt);
    GetCurveAttribute(aDesc, theElem, "score", offsetof(CurveDesc, mScoreTarget), ParseInt);
    GetCurveAttribute(aDesc, theElem, "skullrot", offsetof(CurveDesc, mSkullRotation), ParseInt);
    GetCurveAttribute(aDesc, theElem, "accrate", offsetof(CurveDesc, mAccelerationRate), ParseFloat);
    GetCurveAttribute(aDesc, theElem, "bombfreq", offsetof(CurveDesc, mPowerUpFreq[PowerType_Bomb]), ParseInt);
    GetCurveAttribute(aDesc, theElem, "slowfreq", offsetof(CurveDesc, mPowerUpFreq[PowerType_SlowDown]), ParseInt);
    GetCurveAttribute(aDesc, theElem, "accfreq", offsetof(CurveDesc, mPowerUpFreq[PowerType_Accuracy]), ParseInt);
    GetCurveAttribute(aDesc, theElem, "backfreq", offsetof(CurveDesc, mPowerUpFreq[PowerType_MoveBackwards]), ParseInt);
    GetCurveAttribute(aDesc, theElem, "repeat", offsetof(CurveDesc, mBallRepeat), ParseInt);
    GetCurveAttribute(aDesc, theElem, "single", offsetof(CurveDesc, mMaxSingle), ParseInt);
    GetCurveAttribute(aDesc, theElem, "mergespeed", offsetof(CurveDesc, mMergeSpeed), ParseFloat);
    GetCurveAttribute(aDesc, theElem, "danger", offsetof(CurveDesc, mDangerDistance), ParseInt);
    GetCurveAttribute(aDesc, theElem, "slowdistance", offsetof(CurveDesc, mSlowDistance), ParseInt);
    GetCurveAttribute(aDesc, theElem, "slowfactor", offsetof(CurveDesc, mSlowFactor), ParseFloat);
    GetCurveAttribute(aDesc, theElem, "slowspeed", offsetof(CurveDesc, mSlowSpeed), ParseFloat);
    GetCurveAttribute(aDesc, theElem, "zumaslow", offsetof(CurveDesc, mZumaSlow), ParseInt);
    GetCurveAttribute(aDesc, theElem, "zumaback", offsetof(CurveDesc, mZumaBack), ParseInt);

    if (aDesc.mParTime == 0)
    {
        aDesc.mParTime = 35 * aDesc.mCurveDesc->mScoreTarget / 1000;
        int aTimeOffset = 0;

        for (int i = 0; i < 3; i++)
        {
            aTimeOffset -= (int)aDesc.mCurveDesc[i].mPath.empty() - 1;
        }

        if (aTimeOffset > 1)
        {
            aTimeOffset = 0;
            for (int i = 0; i < 3; i++)
            {
                aTimeOffset -= (int)aDesc.mCurveDesc[i].mPath.empty() - 1;
            }

            aDesc.mParTime = (int)((float)(aDesc.mParTime) * (aTimeOffset * 0.95f));
        }

        aDesc.mParTime = 5 * ((aDesc.mParTime + 4) / 5);
    }

    for (int i = 0; i < 3; i++)
    {
        if (aDesc.mCurveDesc[i].mSlowSpeed != 0.0f)
            aDesc.mCurveDesc[i].mSlowFactor = aDesc.mCurveDesc[i].mSpeed / aDesc.mCurveDesc[i].mSlowSpeed;
    }

    if (isLevel && GetAttribute(theElem, "progression", aVal))
    {
        int aDifficultyDiff = 0;
        int aParDiff = 0;

        if (GetAttribute(theElem, "difficultydiff", aVal))
            ParseInt(aVal.c_str(), &aDifficultyDiff);

        if (GetAttribute(theElem, "pardiff", aVal))
            ParseInt(aVal.c_str(), &aParDiff);

        LevelProgression &aProgression = mLevelProgressionMap[aVal];
        if (aProgression.empty())
        {
            Fail("LevelProgression not found: " + aVal);
            return false;
        }

        for (LevelProgression::iterator anItr = aProgression.begin(); anItr != aProgression.end(); anItr++)
        {
            mLevels.push_back(mSettingsMap[anItr->first]);

            LevelDesc &aProgDesc = mLevels.back();
            CopyGraphics(aProgDesc, aDesc);

            aProgDesc.mDifficulty = anItr->second + aDifficultyDiff;
            aProgDesc.mParTime += aParDiff;

            if (aProgDesc.mName.empty())
                aProgDesc.mName = aName;

            if (aProgDesc.mDisplayName.empty())
                aProgDesc.mDisplayName = aDisplayName;
        }
    }
    else
    {
        if (aDesc.mName.empty())
            aDesc.mName = aName;

        if (aDesc.mDisplayName.empty())
            aDesc.mDisplayName = aDisplayName;

        mLevels.push_back(aDesc);
    }

    if (isLevel)
        mBoardSet.insert(aName);

    return true;
}

bool LevelParser::DoParseSprite(XMLElement &theElem, SpriteDesc &theSprite)
{
    std::string aVal;

    GetAttribute(theElem, "image", aVal);
    theSprite.mImagePath = GetPath(aVal);

    if (GetAttribute(theElem, "pri", aVal))
        ParseInt(aVal.c_str(), &theSprite.mPriority);

    if (GetAttribute(theElem, "x", aVal))
        ParseInt(aVal.c_str(), &theSprite.x);

    if (GetAttribute(theElem, "y", aVal))
        ParseInt(aVal.c_str(), &theSprite.y);

    if (GetAttribute(theElem, "vx", aVal))
        ParseFloat(aVal.c_str(), &theSprite.vx);

    if (GetAttribute(theElem, "vy", aVal))
        ParseFloat(aVal.c_str(), &theSprite.vy);

    if (GetAttribute(theElem, "cutout", aVal))
        ParseBool(aVal.c_str(), &theSprite.mCutout);

    return true;
}

bool LevelParser::DoParseTreasure(XMLElement &theElem, TreasurePoint &thePoint)
{
    std::string aVal;

    if (GetAttribute(theElem, "x", aVal))
        ParseInt(aVal.c_str(), &thePoint.x);

    if (GetAttribute(theElem, "y", aVal))
        ParseInt(aVal.c_str(), &thePoint.y);

    for (int i = 0; i < 3; i++)
    {
        if (GetAttribute(theElem, Sexy::StrFormat("dist%d", i + 1), aVal))
            ParseInt(aVal.c_str(), &thePoint.mCurveDist[i]);
        else
            thePoint.mCurveDist[i] = 0;
    }

    return true;
}

const char *TOK_SEP = ", \r\n\t";

bool LevelParser::DoParseLevelProgression(XMLElement &theElem)
{
    std::string anId = theElem.mAttributes["id"];

    LevelProgression &aProgression = mLevelProgressionMap[anId];
    if (!aProgression.empty())
    {
        Fail("LevelProgression already defined.");
        return false;
    }

    char aSettingsBuf[1024];
    char aDifficultyBuf[1024];

    strcpy(aSettingsBuf, theElem.mAttributes["settings"].c_str());
    strcpy(aDifficultyBuf, theElem.mAttributes["difficulty"].c_str());

    for (const char *i = strtok(aSettingsBuf, TOK_SEP); i != NULL; i = strtok(NULL, TOK_SEP))
    {
        LevelDescMap::iterator anItr = mSettingsMap.find(i);
        if (anItr == mSettingsMap.end())
        {
            Fail("Setting not found: " + std::string(i));
            return false;
        }

        aProgression.push_back(std::make_pair(i, 0));
    }

    LevelProgression::iterator anItr = aProgression.begin();
    for (const char *i = strtok(aDifficultyBuf, TOK_SEP); i != NULL; i = strtok(NULL, TOK_SEP))
    {
        if (anItr != aProgression.end())
        {
            anItr->second = atoi(i) - 1;
            ++anItr;
        }
    }

    if (anItr != aProgression.end())
    {
        Fail("LevelProgression size mismatch.");
        return false;
    }

    return true;
}

bool LevelParser::DoParseStageProgression(XMLElement &theElem, int theStage)
{

    if (mStageMap.size() <= theStage)
        mStageMap.resize(theStage + 1);

    LevelDescList &aProgression = mStageMap[theStage];
    if (!aProgression.empty())
    {
        Fail("StageProgression already defined.");
        return false;
    }

    char aSettingsBuf[1024];
    char aDifficultyBuf[1024];

    strcpy(aSettingsBuf, theElem.mAttributes[Sexy::StrFormat("stage%d", theStage + 1)].c_str());
    strcpy(aDifficultyBuf, theElem.mAttributes[Sexy::StrFormat("diffi%d", theStage + 1)].c_str());

    for (const char *i = strtok(aSettingsBuf, TOK_SEP); i != NULL; i = strtok(NULL, TOK_SEP))
    {
        BoardSet::iterator anItr = mBoardSet.find(i);
        if (anItr == mBoardSet.end())
        {
            Fail("Level not found: " + std::string(i));
            return false;
        }

        aProgression.push_back(LevelDesc());
        aProgression.back().mName = i;
    }

    LevelDescList::iterator aProgItr = aProgression.begin();
    int aLevel = 0;
    for (const char *i = strtok(aDifficultyBuf, TOK_SEP); i != NULL; i = strtok(NULL, TOK_SEP))
    {

        if (aProgItr != aProgression.end())
        {

            LevelDescList::iterator anItr = aProgItr++;
            int aDiff = atoi(i) - 1;
            if (aDiff >= 0)
            {

                *anItr = GetLevelByName(anItr->mName, aDiff);
            }
            else
            {

                LevelDescMap::iterator aDescItr = mSettingsMap.find(i);
                if (aDescItr == mSettingsMap.end())
                {

                    Fail("Setting not found: " + std::string(i));
                    return false;
                }

                std::string aName = anItr->mName;
                *anItr = aDescItr->second;
                anItr->mName = aName;
                CopyGraphics(*anItr, GetLevelByName(anItr->mName, 0));
            }

            anItr->mStage = theStage;
            anItr->mLevel = aLevel++;
        }
    }

    if (aProgItr != aProgression.end())
    {
        Fail("LevelProgression size mismatch.");
        return false;
    }

    return true;
}

void LevelParser::CreateBoardProgression()
{
    mBoardProgression.clear();
    std::set<std::string> aBoardSet;

    for (int i = 0; i < 9; i++)
    {
        if (mStageMap.size() <= i)
        {
            break;
        }

        LevelDescVector &aLevelDescs = mStageMap[i / 3 + 3 * (i % 3)];
        for (int j = 0; j < aLevelDescs.size(); j++)
        {
            std::pair<BoardSet::iterator, bool> anItr = aBoardSet.insert(aLevelDescs[j].mName);
            if (anItr.second)
            {
                mBoardProgression.push_back(aLevelDescs[j].mName);
            }
        }
    }

    for (int i = 9; i < mStageMap.size(); i++)
    {
        LevelDescVector &aLevelDescs = mStageMap[i];
        for (int j = 0; j < aLevelDescs.size(); j++)
        {
            std::pair<BoardSet::iterator, bool> anItr = aBoardSet.insert(aLevelDescs[j].mName);
            if (anItr.second)
            {
                mBoardProgression.push_back(aLevelDescs[j].mName);
            }
        }
    }
}

LevelDesc &LevelParser::GetBackgroundTransition()
{
    std::vector<LevelDesc *> aDescs;

    for (int i = 0; i < mLevels.size(); i++)
    {
        LevelDesc *aDesc = &mLevels[i];
        if (!aDesc->mBackgroundAlpha.empty())
            aDescs.push_back(aDesc);
    }

    if (aDescs.empty())
    {
        return mLevels[0];
    }

    // huh?
    int aRand = rand() % aDescs.size();
    std::vector<LevelDesc *>::iterator anItr = aDescs.begin();
    for (int i = 0; i < aRand; i++)
        ++anItr;

    return **anItr;
}

// int LevelParser::GetDifficulty(int theLevel)
// {
// }

int LevelParser::GetLevelNumByStage(int theStage)
{
    int ret = 0;
    for (int i = 0; i < theStage; i++)
    {
        ret += mStageMap[i].size();
    }

    return ret;
}