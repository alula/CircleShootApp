#include "Zuma_Prefix.pch"

#include <SexyAppFramework/Graphics.h>
#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/SexyVector.h>
#include <SexyAppFramework/Image.h>

#include "Ball.h"
#include "BlendedImage.h"
#include "DataSync.h"
#include "Res.h"

#include <cmath>

using namespace Sexy;

BlendedImage *gBlendedBombLights[6];
BlendedImage *gBlendedPowerupLights[4];
BlendedImage *gBlendedPowerups[4][6];
BlendedImage *gBlendedBalls[6];

int Ball::mIdGen = 0;
bool gSpeedUp = false;
bool gWasReset = false;
bool gBallBlink = false;
bool gForceTreasure = false;
int gForceTreasureNum = 0;
int gColorOverride = 0;
int gNumColors = 4;
bool gCheckCollision = true;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BlendedImage *CreateBlendedBombLight(int theType, Image *theImage)
{
    if (gBlendedBombLights[theType] == NULL)
    {
        Rect srcRect(0, 0, theImage->GetWidth(), theImage->GetHeight());
        gBlendedBombLights[theType] = new BlendedImage(theImage, srcRect);
    }

    return gBlendedBombLights[theType];
}

BlendedImage *CreateBlendedPowerupLight(int thePowerType, Image *theImage)
{
    if (gBlendedPowerupLights[thePowerType] == NULL)
    {
        Rect srcRect(0, 0, theImage->GetWidth(), theImage->GetHeight());
        gBlendedPowerupLights[thePowerType] = new BlendedImage(theImage, srcRect);
    }

    return gBlendedPowerupLights[thePowerType];
}

BlendedImage *CreateBlendedPowerup(int thePowerupType, int theType, Image *theImage)
{
    if (gBlendedPowerups[thePowerupType][theType] == NULL)
    {
        Rect srcRect(0, 0, theImage->GetWidth(), theImage->GetHeight());
        gBlendedPowerups[thePowerupType][theType] = new BlendedImage(theImage, srcRect);
    }

    return gBlendedPowerups[thePowerupType][theType];
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Ball::Ball()
{
    mId = ++mIdGen;
    mx = my = 0.0f;
    mType = 0;
    mBullet = NULL;
    mList = NULL;
    mCollidesWithNext = false;
    mSuckCount = 0;
    mClearCount = 0;
    mBackwardsCount = 0;
    mBackwardsSpeed = 0.0f;
    mComboCount = 0;
    mComboScore = 0;
    mRotation = 0.0f;
    mRotationInc = 0.0f;
    mNeedCheckCollision = false;
    mSuckPending = false;
    mStartFrame = 0;
    mWayPoint = 0.0f;
    mPowerType = PowerType_Max;
    mDestPowerType = PowerType_Max;
    mPowerCount = 0;
    mPowerFade = 0;
    mGapBonus = 0;
    mNumGaps = 0;
    mParticles = NULL;
}

Ball::~Ball()
{
}

void Ball::SetPos(float x, float y)
{
    mx = x;
    my = y;
}

void Ball::SetWayPoint(float thePoint)
{
    mWayPoint = thePoint;
}

void Ball::SetRotation(float theRot, bool immediate)
{
    if (immediate)
    {
        mRotation = theRot;
        return;
    }

    while (fabs(theRot - mRotation) > M_PI)
    {
        if (theRot > mRotation)
        {
            theRot -= M_PI * 2;
        }
        else
        {
            theRot += M_PI * 2;
        }
    }

    mDestRotation = theRot;
    mRotationInc = 0.104719669f;

    if (theRot < mRotation)
    {
        mRotationInc = -mRotationInc;
    }
}

void Ball::Draw(Graphics *g)
{
    if (mClearCount != 0)
    {
        DrawExplosion(g);
    }
    else
    {
        DoDraw(g);
        if (mPowerFade && (mPowerFade & 0x10) != 0)
        {
            g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
            DoDraw(g);
        }

        if (gBallBlink)
        {
            g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
            DoDraw(g);
        }
    }

    g->SetColorizeImages(false);
    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
}

void Ball::DrawShadow(Graphics *g)
{
    if (!gSexyAppBase->Is3DAccelerated())
        return;

    if (mClearCount == 0)
    {
        g->DrawImageF(Sexy::IMAGE_BALL_SHADOW,
                      (mx - Sexy::IMAGE_BALL_SHADOW->GetWidth() / 2) - 3.0f,
                      (my - Sexy::IMAGE_BALL_SHADOW->GetHeight() / 2) + 5.0f);
    }
}

int Ball::GetRadius()
{
    return Sexy::GetDefaultBallRadius();
}

bool Ball::CollidesWithPhysically(Ball *theBall, int thePad)
{
    float dx = theBall->GetX() - this->GetX();
    float dy = theBall->GetY() - this->GetY();
    float r = (float)theBall->GetRadius() + (float)(thePad * 2) + (float)this->GetRadius();

    return dx * dx + dy * dy < r * r;
}

bool Ball::CollidesWith(Ball *theBall, int thePad)
{
    return (2 * (thePad + Sexy::GetDefaultBallRadius())) > fabs(((int)this->mWayPoint - (int)theBall->mWayPoint));
}

bool Ball::Intersects(const SexyVector3 &p1, const SexyVector3 &v1, float &t)
{
    SexyVector3 v2(p1.x - mx, p1.y - my, 0.0f);
    float r = GetRadius() - 1.0f;
    float a = v1.Dot(v1);
    float b = 2.0f * v2.Dot(v1);
    float c = v2.Dot(v2) - r * 2.0f * (r * 2.0f);
    float disc = b * b - 4.0f * a * c;
    if (disc < 0.0f)
    {
        return false;
    }
    disc = sqrtf(disc);
    t = (-b - disc) / (2 * a);
    return true;
}

void Ball::SetBullet(Bullet *theBullet)
{
    mBullet = theBullet;
}

void Ball::SetCollidesWithPrev(bool collidesWithPrev)
{
    Ball *aPrevBall = GetPrevBall();

    if (aPrevBall != NULL)
    {
        aPrevBall->SetCollidesWithNext(collidesWithPrev);
    }
}

bool Ball::GetCollidesWithPrev()
{
    Ball *aPrevBall = GetPrevBall();

    if (aPrevBall != NULL)
    {
        return aPrevBall->GetCollidesWithNext();
    }

    return false;
}

void Ball::UpdateCollisionInfo(int thePad)
{
    Ball *aPrevBall = GetPrevBall();
    Ball *aNextBall = GetNextBall();

    if (aPrevBall != NULL)
    {
        aPrevBall->SetCollidesWithNext(aPrevBall->CollidesWith(this, thePad));
    }

    if (aNextBall != NULL)
    {
        SetCollidesWithNext(aNextBall->CollidesWith(this, thePad));
    }
    else
    {
        SetCollidesWithNext(false);
    }
}

void Ball::StartClearCount(bool inTunnel)
{
    if (mClearCount != 0)
        return;

    mClearCount = 1;

    if (!inTunnel)
    {
        if (mParticles == NULL)
            mParticles = new Particle[60];

        for (int i = 0; i < 60; i++)
        {
            Particle &ptcl = mParticles[i];
            float angle = (float)(Sexy::AppRand() % 360) * M_PI / 180.0f;
            float speed = (float)(Sexy::AppRand() % 500) / 500.0f;
            ptcl.vx = sinf(angle) * speed;
            ptcl.vy = cosf(angle) * speed;

            int rnd = Sexy::AppRand();
            ptcl.x = (rnd % 30) * ptcl.vx + mx;
            ptcl.y = (rnd % 30) * ptcl.vy + my;
            ptcl.mSize = (int)(Sexy::AppRand() % 10 <= 1) + 1;
        }
    }
}

void Ball::SetClearCount(int theCount)
{
    mClearCount = theCount;
}

void Ball::SetPowerType(PowerType theType, bool delay)
{
    if (mPowerType == theType)
        return;

    if (delay)
    {
        mDestPowerType = theType;
        mPowerFade = 100;
    }
    else
    {
        mDestPowerType = PowerType_None;
        mPowerType = theType;
    }
}

PowerType Ball::GetPowerTypeWussy()
{
    if (mPowerType == PowerType_None)
        return mDestPowerType;

    return mPowerType;
}

void Ball::RemoveFromList()
{
    if (mList != NULL)
    {
        mList->erase(mListItr);
        mList = NULL;
    }
}

const BallList::iterator &Ball::InsertInList(BallList &theList, const BallList::iterator &theInsertItr)
{
    mList = &theList;
    mListItr = theList.insert(theInsertItr, this);
    return mListItr;
}

Ball *Ball::GetPrevBall(bool mustCollide)
{
    if (mList == NULL)
    {
        return NULL;
    }

    BallList::iterator anItr = GetListItr();
    if (anItr == mList->begin())
    {
        return NULL;
    }

    BallList::iterator aPrevItr = anItr;
    aPrevItr--;

    Ball *aBall = *aPrevItr;

    if (!mustCollide)
        return aBall;

    if (!aBall->GetCollidesWithNext())
        return NULL;

    return aBall;
}

Ball *Ball::GetNextBall(bool mustCollide)
{
    if (mList == NULL)
    {
        return NULL;
    }

    BallList::iterator anItr = mListItr;
    anItr++;
    if (anItr == mList->end())
    {
        return NULL;
    }

    if (!mustCollide || GetCollidesWithNext())
    {
        return *anItr;
    }

    return NULL;
}

void Ball::UpdateRotation()
{
    if (mPowerFade > 0)
    {
        --mPowerFade;
        if (mPowerFade == 0)
        {
            mPowerType = mDestPowerType;
            mDestPowerType = PowerType_None;

            if (mPowerType != PowerType_None)
            {
                mPowerCount = 2000;
            }
        }
    }

    if (mPowerCount > 0)
    {
        --mPowerCount;
        if (mPowerCount <= 0 && mPowerType != PowerType_None)
        {
            mDestPowerType = PowerType_None;
            mPowerFade = 100;
        }
    }

    if (mRotationInc != 0.0f)
    {
        mRotation += mRotationInc;

        if (mRotationInc > 0.0f && mRotation > mDestRotation)
        {
            mRotation = mDestRotation;
            mRotationInc = 0.0f;
        }
        else if (mRotationInc < 0.0f && mRotation < mDestRotation)
        {
            mRotation = mDestRotation;
            mRotationInc = 0.0f;
        }
    }
}

void Ball::SetFrame(int theFrame)
{
    Image *anImage = Sexy::GetImageById((ResourceId)((int)Sexy::IMAGE_BLUE_BALL_ID + mType));
    mStartFrame = anImage->mNumRows - (int)((float)theFrame + mWayPoint) % anImage->mNumRows;
}

void Ball::IncFrame(int theInc)
{
    Image *anImage = Sexy::GetImageById((ResourceId)((int)Sexy::IMAGE_BLUE_BALL_ID + mType));
    mStartFrame = (mStartFrame + theInc) % anImage->mNumRows;

    if (mStartFrame < 0)
        mStartFrame += anImage->mNumRows;
}

void Ball::RandomizeFrame()
{
    mStartFrame = Sexy::AppRand() % 50;
}

void Ball::SyncState(DataSync &theSync)
{
    theSync.RegisterPointer(this);
    theSync.SyncLong(mId);
    theSync.SyncByte(mType);
    theSync.SyncFloat(mWayPoint);
    theSync.SyncFloat(mRotation);
    theSync.SyncFloat(mDestRotation);
    theSync.SyncFloat(mRotationInc);
    theSync.SyncFloat(mx);
    theSync.SyncFloat(my);
    theSync.SyncBool(mCollidesWithNext);
    theSync.SyncBool(mNeedCheckCollision);
    theSync.SyncBool(mSuckPending);
    theSync.SyncShort(mClearCount);
    theSync.SyncShort(mSuckCount);
    theSync.SyncShort(mBackwardsCount);
    theSync.SyncFloat(mBackwardsSpeed);
    theSync.SyncByte(mComboCount);
    theSync.SyncLong(mComboScore);
    theSync.SyncByte(mStartFrame);
    theSync.SyncShort(mPowerCount);
    theSync.SyncShort(mPowerFade);
    theSync.SyncShort(mGapBonus);
    theSync.SyncByte(mNumGaps);

    if (theSync.mReader)
    {
        theSync.SyncBytes(&mPowerType, 4u);
        mPowerType = (PowerType)ByteSwap((unsigned int)mPowerType);
    }
    else
    {
        mPowerType = (PowerType)ByteSwap((unsigned int)mPowerType);
        theSync.SyncBytes(&mPowerType, 4u);
    }

    if (theSync.mReader)
    {
        theSync.SyncBytes(&mDestPowerType, 4u);
        mDestPowerType = (PowerType)ByteSwap((unsigned int)mDestPowerType);
    }
    else
    {
        mDestPowerType = (PowerType)ByteSwap((unsigned int)mDestPowerType);
        theSync.SyncBytes(&mDestPowerType, 4u);
    }

    theSync.SyncPointer((void **)&mBullet);
}

void Ball::DrawBomb(Graphics *g)
{
    Image *image = Sexy::GetImageById((ResourceId)((int)Sexy::IMAGE_BLUE_BOMB_ID + mType));
    int width = mx - image->GetWidth() / 2;
    int height = my - image->GetHeight() / 2;

    if (gSexyAppBase->Is3DAccelerated())
    {
        g->DrawImageF(image, width, height);
    }
    else
    {
        BlendedImage *blendedImage = CreateBlendedPowerup(0, mType, image);
        blendedImage->Draw(g, width, height);
    }

    int alpha = Sexy::GetBoardStateCount();

    if (alpha % 50 <= 9)
    {
        alpha = 0;
    }
    else if (alpha % 50 <= 24)
    {
        alpha = (200 * (alpha % 50) - 2000) / 15;
    }
    else if (alpha % 50 <= 34)
    {
        alpha = 200;
    }
    else if (alpha % 50 <= 49)
    {
        alpha = 200 - (200 * (alpha % 50) - 7000) / 15;
    }

    g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
    g->SetColorizeImages(true);
    g->SetColor(Color(alpha, alpha, alpha));

    if (gSexyAppBase->Is3DAccelerated())
    {
        Image *lightImage = Sexy::GetImageById((ResourceId)((int)Sexy::IMAGE_BLUE_LIGHT_ID + mType));
        g->DrawImageF(lightImage, width + 7.0f, height + 9.0f);
    }
    else
    {
        Image *lightImage = Sexy::GetImageById((ResourceId)((int)Sexy::IMAGE_BLUE_LIGHT_ID + mType));
        BlendedImage *blendedImage = CreateBlendedBombLight(mType, lightImage);
        blendedImage->Draw(g, width + 7.0f, height + 9.0f);
    }

    g->SetColorizeImages(false);
    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
}

void Ball::DrawStandardPower(Graphics *g, int theBallImageId, int theBlinkImageId, int thePowerType)
{
    Image *ballImage = Sexy::GetImageById((ResourceId)(theBallImageId + mType));
    Image *blinkImage = Sexy::GetImageById((ResourceId)theBlinkImageId);
    int width = mx - ballImage->GetWidth() / 2;
    int height = my - ballImage->GetHeight() / 2;

    if (gSexyAppBase->Is3DAccelerated())
    {
        g->DrawImageRotatedF(ballImage, width, height, mRotation + M_PI / 2);
    }
    else
    {
        BlendedImage *blendedImage = CreateBlendedPowerup(thePowerType, mType, ballImage);
        blendedImage->Draw(g, width, height);
    }

    int time = Sexy::GetBoardStateCount() % 100;
    int alpha;

    if (time <= 19)
    {
        alpha = 0;
    }
    else if (time <= 49)
    {
        alpha = (255 * time - 5100) / 30;
    }
    else if (time <= 69)
    {
        alpha = 255;
    }
    else if (time <= 99)
    {
        alpha = 255 - (255 * time - 17850) / 30;
    }

    Color ballColor = Sexy::gDarkBallColors[mType];
    ballColor.mAlpha = alpha;
    g->SetColorizeImages(true);
    g->SetColor(ballColor);

    if (gSexyAppBase->Is3DAccelerated())
    {
        g->DrawImageRotatedF(blinkImage, width, height, mRotation + M_PI / 2);
    }
    else
    {
        BlendedImage *blendedImage = CreateBlendedPowerupLight(thePowerType, blinkImage);
        blendedImage->Draw(g, width, height);
    }

    g->SetColorizeImages(false);
    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
}

void Ball::DrawPower(Graphics *g)
{
    switch (mPowerType)
    {
    case PowerType_Bomb:
        DrawBomb(g);
        break;
    case PowerType_SlowDown:
        DrawStandardPower(g, Sexy::IMAGE_BLUE_SLOW_ID, Sexy::IMAGE_SLOW_LIGHT_ID, PowerType_SlowDown);
        break;
    case PowerType_Accuracy:
        DrawStandardPower(g, Sexy::IMAGE_BLUE_ACCURACY_ID, Sexy::IMAGE_ACCURACY_LIGHT_ID, PowerType_Accuracy);
        break;
    case PowerType_MoveBackwards:
        DrawStandardPower(g, Sexy::IMAGE_BLUE_BACKWARDS_ID, Sexy::IMAGE_BACKWARDS_LIGHT_ID, PowerType_MoveBackwards);
        break;
    }
}

void Ball::DrawExplosion(Graphics *g)
{
    g->SetColorizeImages(true);

    // maybe there should be Sexy::IMAGE_GRAY_EXPLOSION_ID->GetCelHeight(), but doesn't seem like it's inlinable?
    int aCelHeight = Sexy::IMAGE_GRAY_EXPLOSION->mHeight / Sexy::IMAGE_GRAY_EXPLOSION->mNumRows;
    int aImgX = mx - Sexy::IMAGE_GRAY_EXPLOSION->GetWidth() / 2;
    int aImgY = my - aCelHeight / 2;

    g->SetColor(Color(gBrightBallColors[mType]));

    if (gSexyAppBase->Is3DAccelerated())
    {
        int aCel = mClearCount / 3;
        if (aCel < Sexy::IMAGE_GRAY_EXPLOSION->mNumRows)
        {
            float anAngle = (mStartFrame * M_PI) / 25.0f;
            int aImgWidth = Sexy::IMAGE_GRAY_EXPLOSION->mWidth;
            Rect aRect(0, aCel * aCelHeight, aImgWidth, aCelHeight);
            g->DrawImageRotatedF(Sexy::IMAGE_GRAY_EXPLOSION, aImgX, aImgY, anAngle, &aRect);
        }
    }
    else
    {
        g->DrawImageCel(Sexy::IMAGE_GRAY_EXPLOSION, aImgX, aImgY, mClearCount / 3);
    }

    if (mParticles != NULL)
    {
        int aRed = std::min((gBrightBallColors[mType] >> 16) & 0xff + 200, 255);
        int aGreen = std::min((gBrightBallColors[mType] >> 8) & 0xff + 200, 255);
        int aBlue = std::min((gBrightBallColors[mType]) & 0xff + 200, 255);
        int anAlpha = mClearCount > 20 ? 255 - (255 * mClearCount - 5100) / 20 : 255;
        g->SetColor(Color(aRed, aGreen, aBlue, anAlpha));

        for (int i = 0; i != 60; ++i)
        {
            Particle *aParticle = &mParticles[i];
            g->FillRect(
                mClearCount * aParticle->vx + aParticle->x,
                mClearCount * aParticle->vy + aParticle->y,
                aParticle->mSize,
                aParticle->mSize);
        }
    }
}

void Ball::DoDraw(Graphics *g)
{
    if (mPowerType != PowerType_None)
    {
        DrawPower(g);
    }
    else
    {
        Image *image = Sexy::GetImageById((ResourceId)(mType + Sexy::IMAGE_BLUE_BALL_ID));
        float aBallX = mx - 16.0f;
        float aBallY = my - 16.0f;
        int aFrame = (mStartFrame + (int)mWayPoint) % image->mNumRows;

        if (gSexyAppBase->Is3DAccelerated())
        {
            int aCelHeight = image->mHeight / image->mNumRows;
            Rect aRect(0, aFrame * aCelHeight, image->mWidth, aCelHeight);
            g->DrawImageRotatedF(image, aBallX, aBallY, mRotation, &aRect);
        }
        else
        {
            if (gBlendedBalls[mType] == NULL)
            {
                Image *image = Sexy::GetImageById((ResourceId)(mType + Sexy::IMAGE_BLUE_BALL_ID));
                int aCelHeight = image->mHeight / image->mNumRows;
                Rect aRect(0, aFrame * aCelHeight, image->mWidth, aCelHeight);
                gBlendedBalls[mType] = new BlendedImage(image, aRect);
            }

            gBlendedBalls[mType]->Draw(g, aBallX, aBallY);
        }
    }
}