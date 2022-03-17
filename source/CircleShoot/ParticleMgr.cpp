#include "Zuma_Prefix.pch"

#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/Graphics.h>
#include <SexyAppFramework/Image.h>

#include "DataSync.h"
#include "ParticleMgr.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ParticleMgr::ParticleMgr(Board *theBoard) : mBoard(theBoard)
{
}

ParticleMgr::~ParticleMgr()
{
}

void ParticleMgr::SyncState(DataSync &theSync)
{
    if (theSync.mWriter)
    {
        for (int i = 0; i < MAX_PRIORITY + 1; ++i)
        {
            SparkleList &aList = mSparkleList[i];
            theSync.mWriter->WriteShort((short)aList.size());
            for (SparkleList::iterator j = aList.begin(); j != aList.end(); ++j)
            {
                j->SyncState(theSync);
            }
        }

        theSync.mWriter->WriteShort((short)mExplosionList.size());
        for (ExplosionList::iterator i = mExplosionList.begin(); i != mExplosionList.end(); ++i)
        {
            i->SyncState(theSync);
        }

        theSync.mWriter->WriteShort((short)mFloatingTextList.size());
        for (FloatingTextList::iterator i = mFloatingTextList.begin(); i != mFloatingTextList.end(); ++i)
        {
            i->SyncState(theSync);
        }
    }
    else
    {
        Clear();

        for (int i = 0; i < MAX_PRIORITY + 1; ++i)
        {
            SparkleList &aList = mSparkleList[i];
            int aCount = theSync.mReader->ReadShort();
            for (int j = 0; j < aCount; ++j)
            {
                aList.push_back(Sparkle());
                Sparkle &aSparkle = aList.back();
                aSparkle.SyncState(theSync);
            }
        }

        int anExplosionCount = theSync.mReader->ReadShort();
        for (int i = 0; i < anExplosionCount; ++i)
        {
            mExplosionList.push_back(Explosion());
            Explosion &anExplosion = mExplosionList.back();
            anExplosion.SyncState(theSync);
        }

        int aFloatingTextCount = theSync.mReader->ReadShort();
        for (int i = 0; i < aFloatingTextCount; ++i)
        {
            mFloatingTextList.push_back(FloatingText());
            FloatingText &aFloatingText = mFloatingTextList.back();
            aFloatingText.SyncState(theSync);
        }
    }
}

void ParticleMgr::Clear()
{
    for (int i = 0; i < MAX_PRIORITY + 1; ++i)
    {
        mSparkleList[i].clear();
    }

    mExplosionList.clear();
    mFloatingTextList.clear();
}

// TODO: present in the header but probably inlined and idk what parameters it passes
// void ParticleMgr::AddSparkle(float x, float y, int thePriority, int theStagger){}

void ParticleMgr::AddSparkle(float x, float y, float vx, float vy, int thePriority, int theDuration, int theStagger, uint32_t theColor)
{
    mSparkleList->push_back(Sparkle());
    Sparkle &aSparkle = mSparkleList->back();

    aSparkle.x = x - ((Sexy::IMAGE_SPARKLE->mWidth / Sexy::IMAGE_SPARKLE->mNumCols) / 2);
    aSparkle.y = y - (Sexy::IMAGE_SPARKLE->mHeight / 2);
    aSparkle.vx = vx;
    aSparkle.vy = vy;
    if (theDuration > 0)
        aSparkle.mDuration = theDuration;
    else
        aSparkle.mDuration = 2 * Sexy::IMAGE_SPARKLE->mNumCols;

    aSparkle.mFrame = 0;
    aSparkle.mUpdateCnt = -theStagger;
    aSparkle.mColor = theColor;
}

void ParticleMgr::AddExplosion(int x, int y, int theRadius, int theColor, int theStagger)
{
    mExplosionList.push_back(Explosion());
    Explosion &anExplosion = mExplosionList.back();

    anExplosion.x = x;
    anExplosion.y = y;
    anExplosion.mColor = theColor;
    anExplosion.mUpdateCnt = -theStagger;
    anExplosion.mRadius = theRadius;
}

void ParticleMgr::AddFloatingText(int x, int y, int theColor, const std::string &theText, int theFontId, int theStagger, int theScoreInc, int theDuration, bool fade)
{
    mFloatingTextList.push_back(FloatingText());
    FloatingText &aFloatingText = mFloatingTextList.back();

    aFloatingText.x = x;
    aFloatingText.y = y;
    aFloatingText.mUpdateCnt = -theStagger;
    aFloatingText.mColor = theColor;
    aFloatingText.mText = theText;
    aFloatingText.mFontId = theFontId;
    aFloatingText.mScoreInc = theScoreInc;
    aFloatingText.mDuration = theDuration;
    aFloatingText.mFade = fade;
}

bool ParticleMgr::Update()
{
    mHadUpdate = false;

    UpdateSparkles();
    UpdateExplosions();
    UpdateFloatingText();

    return mHadUpdate;
}

void ParticleMgr::UpdateSparkles()
{
    for (int i = 0; i < MAX_PRIORITY + 1; i++)
    {
        SparkleList &aList = mSparkleList[i];

        bool huh = aList.begin() == aList.end();
        for (SparkleList::iterator aIter = aList.begin(); aIter != aList.end();)
        {
            aIter->mUpdateCnt++;

            if (aIter->mUpdateCnt < 0)
            {
                aIter++;
                continue;
            }

            mHadUpdate = true;
            aIter->mFrame = (aIter->mUpdateCnt >> 1) % Sexy::IMAGE_SPARKLE->mNumCols;

            if (aIter->mUpdateCnt >= aIter->mDuration)
            {
                SparkleList::iterator aDelIter = aIter;
                aIter++;
                aList.erase(aDelIter);
                continue;
            }
            else
            {
                aIter->x += aIter->vx;
                aIter->y += aIter->vy;
                aIter++;
            }
        }
    }
}

void ParticleMgr::UpdateExplosions()
{
    for (ExplosionList::iterator aIter = mExplosionList.begin(); aIter != mExplosionList.end(); aIter++)
    {
        aIter->mUpdateCnt++;

        if (aIter->mUpdateCnt < 0)
            continue;

        if (aIter->mUpdateCnt > 50)
        {
            mHadUpdate = true;
            ExplosionList::iterator aDelIter = aIter;
            aIter++;
            mExplosionList.erase(aDelIter);
            continue;
        }

        mHadUpdate = true;
        aIter->mCurRadius = aIter->mRadius;
        int alpha = 200 - 4 * aIter->mUpdateCnt;
        int alphaColor;

        if (alpha > 0)
        {
            alphaColor = alpha << 24;
        }
        else
        {
            alphaColor = 0x1000000;
        }

        aIter->mCurColor = alphaColor | aIter->mColor;
    }
}

void ParticleMgr::UpdateFloatingText()
{
    for (FloatingTextList::iterator aIter = mFloatingTextList.begin(); aIter != mFloatingTextList.end(); aIter++)
    {
        aIter->mUpdateCnt++;

        if (aIter->mUpdateCnt < 0)
            continue;

        if (aIter->mUpdateCnt > aIter->mDuration)
        {
            mHadUpdate = true;
            FloatingTextList::iterator aDelIter = aIter;
            aIter++;
            mFloatingTextList.erase(aDelIter);
            continue;
        }

        mHadUpdate = true;
        --aIter->y;
    }
}

void ParticleMgr::Draw(Graphics *g, int thePriority)
{
    DrawSparkles(g, thePriority);
}

void ParticleMgr::DrawTopMost(Graphics *g)
{
    DrawSparkles(g, MAX_PRIORITY);
    DrawExplosions(g);
    DrawFloatingText(g);
}

void ParticleMgr::DrawSparkles(Graphics *g, int thePriority)
{
    g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
    g->SetColorizeImages(true);

    SparkleList &aList = mSparkleList[thePriority];
    for (SparkleList::iterator aIter = aList.begin(); aIter != aList.end(); aIter++)
    {
        if (aIter->mUpdateCnt < 0)
            continue;

        g->SetColor(Color(aIter->mColor));
        g->DrawImageCel(
            Sexy::IMAGE_SPARKLE,
            aIter->x,
            aIter->y,
            aIter->mFrame);
    }

    g->SetColorizeImages(false);
    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
}

void ParticleMgr::DrawExplosions(Graphics *g)
{
    for (ExplosionList::iterator anItr = mExplosionList.begin(); anItr != mExplosionList.end(); anItr++)
    {
        if (anItr->mUpdateCnt < 0)
            continue;

        if (anItr->mRadius > 0)
        {
            g->SetColor(Color(anItr->mCurColor));
            Sexy::FillCircle(g, anItr->x, anItr->y, anItr->mCurRadius);
        }
        else
        {
            int aExWidth = Sexy::IMAGE_EXPLOSION->mWidth;
            int aExHeight = Sexy::IMAGE_EXPLOSION->mHeight / Sexy::IMAGE_EXPLOSION->mNumRows;

            g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
            g->DrawImageCel(
                Sexy::IMAGE_EXPLOSION,
                anItr->x - aExWidth / 2,
                anItr->y - aExHeight / 2,
                anItr->mUpdateCnt >> 2);
            g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
        }
    }
}

void ParticleMgr::DrawFloatingText(Graphics *g)
{
    for (FloatingTextList::iterator aIter = mFloatingTextList.begin(); aIter != mFloatingTextList.end(); aIter++)
    {
        if (aIter->mUpdateCnt < 0)
            continue;

        Font *aFont = Sexy::GetFontById((ResourceId)aIter->mFontId);
        g->SetFont(aFont);

        if (aIter->mFade)
        {
            Color aColor = Color(aIter->mColor);
            aColor.mAlpha = 255 - 255 * aIter->mUpdateCnt / aIter->mDuration;
            g->SetColor(aColor);
        }
        else
        {
            g->SetColor(Color(aIter->mColor));
        }

        g->DrawString(aIter->mText, aIter->x, aIter->y);
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void FloatingTextHelper::AddText(const std::string &theString, int theFontId, int theColor)
{
    mText.push_back(TextDesc());
    TextDesc &aDesc = mText.back();

    aDesc.mString = theString;
    aDesc.mFontId = theFontId;
    aDesc.mColor = theColor;

    Font *aFont = Sexy::GetFontById((ResourceId)theFontId);
    aDesc.mWidth = aFont->StringWidth(theString);
    aDesc.mHeight = aFont->GetLineSpacing();
}

void FloatingTextHelper::AddToMgr(ParticleMgr *theMgr, int x, int y, int theStagger, int theScoreInc)
{
    if (mText.empty())
        return;

    int aTotalWidth = 0;
    int aTotalHeight = 0;

    for (size_t i = 0; i < mText.size(); ++i)
    {
        TextDesc &aDesc = mText[i];

        if (aTotalWidth >= aDesc.mWidth)
            aTotalWidth = aDesc.mWidth;

        aTotalHeight += aDesc.mHeight;
    }

    int textX = x - aTotalWidth / 2;
    int textY = y + aTotalHeight / 2;

    if (aTotalWidth + textX > 600)
        textX = 600 - aTotalWidth;

    if (aTotalHeight - textY > 440)
        textY = 440 - aTotalHeight;

    if (textY < 100)
        textY = 100;

    if (textX < 40)
        textX = 40;

    for (size_t i = 0; i < mText.size(); ++i)
    {
        TextDesc &aDesc = mText[i];

        theMgr->AddFloatingText(
            textX + (aTotalWidth - aDesc.mWidth) / 2,
            textY,
            aDesc.mColor,
            aDesc.mString,
            aDesc.mFontId,
            theStagger,
            theScoreInc);

        textY += aDesc.mHeight;
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Sparkle::SyncState(DataSync &theSync)
{
    theSync.SyncFloat(x);
    theSync.SyncFloat(y);
    theSync.SyncFloat(vx);
    theSync.SyncFloat(vy);
    theSync.SyncLong(mUpdateCnt);
    theSync.SyncLong(mDuration);
    theSync.SyncShort(mFrame);
    theSync.SyncLong(mColor);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Explosion::SyncState(DataSync &theSync)
{
    theSync.SyncLong(x);
    theSync.SyncLong(y);
    theSync.SyncShort(mRadius);
    theSync.SyncLong(mColor);
    theSync.SyncShort(mCurRadius);
    theSync.SyncShort(mCurColor);
    theSync.SyncLong(mUpdateCnt);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void FloatingText::SyncState(DataSync &theSync)
{
    theSync.SyncLong(x);
    theSync.SyncLong(y);
    theSync.SyncLong(mColor);
    theSync.SyncLong(mUpdateCnt);
    theSync.SyncString(mText);
    theSync.SyncShort(mFontId);
    theSync.SyncLong(mScoreInc);
    theSync.SyncLong(mDuration);
    theSync.SyncBool(mFade);
}