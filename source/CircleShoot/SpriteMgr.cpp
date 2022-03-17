#include "Zuma_Prefix.pch"

#include <SexyAppFramework/Graphics.h>
#include <SexyAppFramework/Image.h>
#include <SexyAppFramework/DDImage.h>
#include <SexyAppFramework/MemoryImage.h>
#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/ResourceManager.h>

#include "CircleShootApp.h"
#include "CurveMgr.h"
#include "SpriteMgr.h"
#include "LevelParser.h"
#include "Ball.h"
#include "Res.h"

#include <cmath>

using namespace Sexy;

class ImageCutter
{
public:
    std::list<SpriteImage> mImages;

    void Draw(Graphics *g)
    {
        for (std::list<SpriteImage>::iterator it = mImages.begin(); it != mImages.end(); ++it)
        {
            SpriteImage &image = *it;
            g->DrawImage(image.mImage, image.x, image.y);
        }
    }

    void DoCut(Image *theImage, int left, int top, int right, int bottom)
    {
        int aTop = 0;
        int aLeft = left;
        if (left < 0)
            aLeft = 0;
        if (top >= 0)
            aTop = top;
        int mWidth = theImage->mWidth;
        int aRight = right;
        if (mWidth < right)
            aRight = theImage->mWidth;
        int mHeight = theImage->mHeight;
        int aBottom = bottom;
        if (mHeight < bottom)
            aBottom = theImage->mHeight;

        if (aLeft < aRight && aTop < aBottom)
        {
            ulong *v21 = &reinterpret_cast<MemoryImage *>(theImage)->GetBits()[aLeft + theImage->mWidth * aTop];
            int v19 = mHeight;
            int v12 = aTop;
            int v20 = 0;
            int v13 = 0;

            while (v12 != aBottom)
            {
                ulong *v16 = v21;
                for (int i = aLeft; aRight != i; ++i)
                {
                    if ((*v16 & 0xFF000000) != 0)
                    {
                        if (mWidth > i)
                            mWidth = i;
                        int v14 = v20;
                        if (v20 < i)
                            v14 = i;
                        v20 = v14;
                        int v15 = v19;
                        if (v19 > v12)
                            v15 = v12;
                        v19 = v15;
                        if (v13 < v12)
                            v13 = v12;
                    }
                    ++v16;
                }
                v21 += theImage->mWidth;
                ++v12;
            }

            if (mWidth <= v20 && v19 <= v13)
            {
                MemoryImage *theDestImage = new MemoryImage(gSexyAppBase);
                theDestImage->Create(v20 - mWidth + 1, v13 - v19 + 1);
                Graphics g(theDestImage);
                g.DrawImage(theImage, -mWidth, -v19);

                mImages.push_back(SpriteImage());

                SpriteImage &aSprite = mImages.back();
                aSprite.x = mWidth;
                aSprite.y = v19;
                aSprite.mImage = theDestImage;
            }
        }
    }

    void Cut(Image *theImage, int a3, int a4, int a5, int a6)
    {
        int aWidth = theImage->mWidth;
        int aHeight = theImage->mHeight;

        DoCut(theImage, 0, 0, a3, a4);
        DoCut(theImage, a3, 0, aWidth - a5, a4);
        DoCut(theImage, aWidth - a5, 0, aWidth, a4);
        DoCut(theImage, 0, a4, a3, aHeight - a4);
        DoCut(theImage, a3, a4, aWidth - a5, aHeight - a4);
        DoCut(theImage, aWidth - a5, a4, aWidth, aHeight - a4);
        DoCut(theImage, 0, aHeight - a4, a3, aHeight);
        DoCut(theImage, a3, aHeight - a4, aWidth - a5, aHeight);
        DoCut(theImage, aWidth - a5, aHeight - a4, aWidth, aHeight);
    }
};

ImageCutter *gImageCutter;
int gPrevColor = 0xffffffff;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SpriteMgr::SpriteMgr()
{
    mHasBackground = false;
    mNumHoles = 0;
    mInSpace = false;
    mSpaceScroll = false;
    mNebulaImage = NULL;
    mBackgroundImage = new DDImage(gSexyAppBase->mDDInterface);
    mBackgroundImage->Create(640, 480);
}

SpriteMgr::~SpriteMgr()
{
    delete mBackgroundImage;
    delete mNebulaImage;
}

void SpriteMgr::AddSprite(Image *theImage, int x, int y, int thePriority)
{
    if ((unsigned int)thePriority < MAX_PRIORITY)
    {
        mSprites[thePriority].push_back(SpriteImage());

        SpriteImage &theSprite = mSprites[thePriority].back();
        theSprite.x = x;
        theSprite.y = y;
        theSprite.mImage = theImage;
    }
}

// void SpriteMgr::GenBackgroundForLevel(const LevelDesc &theLevel)
// {
// }

void SpriteMgr::SetupLevel(const LevelDesc &theLevel, MirrorType theMirror)
{
    mStarList.clear();
    mNumHoles = 0;
    mUpdateCnt = 0;

    int aStarColors[] = {
        0x808080,
        0xFFFF00,
        0x80C0C0,
        0xA0FFFF,
        0x94B1C6,
        0x000000,
        0xC6A994,
        0x8000FF,
        0x80C0FF,
        0x4040A0,
        0x94C6A7,
        0x0000FF,
    };

    if (!gWasReset)
    {
        int c;
        do
        {
            c = Sexy::ThreadRand() % 6;
        } while (c == gPrevColor);

        gPrevColor = c;
        gBoardColor = aStarColors[c * 2];
        gCurveColor = aStarColors[c * 2 + 1];
    }

    mBackgroundSprites.clear();

    for (int i = 0; i < MAX_PRIORITY; i++)
        mSprites[i].clear();

    mHoleFlashList.clear();

    Graphics g(mBackgroundImage);
    MemoryImage *anImage;

    if (!theLevel.mImagePath.empty() && (anImage = gSexyAppBase->GetImage(theLevel.mImagePath), ImageMirror(anImage, theMirror), anImage))
    {
        mHasBackground = true;
        g.DrawImage(anImage, 0, 0);
        delete anImage;
    }
    else
    {
        mHasBackground = false;

        if (theLevel.mBGColor == -1)
        {
            g.SetColor(Color(0));
        }
        else
        {
            g.SetColor(Color(theLevel.mBGColor));
        }

        g.SetColor(Color(gBoardColor));
        g.FillRect(0, 0, 640, 480);
    }

    if (gImageCutter == NULL)
    {
        gImageCutter = new ImageCutter();
        gImageCutter->Cut(Sexy::IMAGE_MENU_BAR, 200, 150, 50, 100);
        gSexyAppBase->mResourceManager->ReplaceImage("IMAGE_MENU_BAR", NULL);
    }

    mInSpace = theLevel.mInSpace;
    if (mInSpace)
    {
        SetupSpace();
    }
    else
    {
        delete mNebulaImage;
        mNebulaImage = NULL;

        for (SpriteDescList::const_iterator anItr = theLevel.mSprites.begin(); anItr != theLevel.mSprites.end(); anItr++)
        {
            // this is UB
            SpriteDesc &aDesc = (SpriteDesc &)*anItr;
            MemoryImage *anImage = gSexyAppBase->GetImage(aDesc.mImagePath, false);

            if (aDesc.mCutout)
            {
                MemoryImage *im = anImage;
                Sexy::MirrorRect(aDesc.x, aDesc.y, anImage->mWidth, anImage->mHeight, theMirror);
                Sexy::ImageMirror(anImage, theMirror);
                anImage = Sexy::CutoutImageFromAlpha(mBackgroundImage, anImage, aDesc.x, aDesc.y);
                delete im;
            }

            if (anImage != NULL)
            {
                int aPriority = aDesc.mPriority;
                if (aPriority < 0)
                {
                    g.DrawImage(anImage, aDesc.x, aDesc.y);
                    delete anImage;
                }
                else
                {
                    if (aPriority > 4)
                        aPriority = 4;

                    mSprites[aPriority].push_back(SpriteImage());

                    SpriteImage &theSprite = mSprites[aPriority].back();
                    theSprite.x = aDesc.x;
                    theSprite.y = aDesc.y;
                    theSprite.mImage = anImage;
                }
            }
        }
    }
}

void SpriteMgr::GenerateBackgroundTransitionSprites(const LevelDesc &theLevel)
{
    mBackgroundSprites.clear();

    MemoryImage anImage(gSexyAppBase);
    anImage.Create(640, 480);
    Graphics g(&anImage);
    DrawBackground(&g);

    for (SpriteDescList::const_iterator anItr = theLevel.mBackgroundAlpha.begin();
         anItr != theLevel.mBackgroundAlpha.end();
         anItr++)
    {
        MemoryImage *aMemImg = gSexyAppBase->GetImage(anItr->mImagePath, false);
        GenerateTransitionSprite(&anImage, aMemImg, *anItr);
        delete aMemImg;
    }

    gSexyAppBase->ClearUpdateBacklog();
}

void SpriteMgr::DrawSprites(Graphics *g, const SpriteList &theList)
{
    for (SpriteList::const_iterator iter = theList.begin(); iter != theList.end(); ++iter)
    {
        g->DrawImage(iter->mImage, iter->x, iter->y);
    }
}

void SpriteMgr::DrawSpace(Graphics *g)
{
    if (mNebulaImage)
    {

        int scrollY = (mUpdateCnt / 4) % mNebulaImage->mHeight;
        g->DrawImage(mNebulaImage, 0, scrollY - mNebulaImage->mHeight);
        g->DrawImage(mNebulaImage, 0, scrollY);
        g->DrawImage(mNebulaImage, 0, scrollY + mNebulaImage->mHeight);

        for (StarList::const_iterator iter = mStarList.begin(); iter != mStarList.end(); ++iter)
        {
            g->SetColor(Sexy::Color(iter->mColor));
            g->FillRect(iter->x, iter->y, 1, 1);
        }
    }
    else
    {
        g->SetColor(Sexy::Color(0));
        g->FillRect(0, 0, 640, 480);
    }
}

void SpriteMgr::GenerateTransitionSprite(MemoryImage *theBackgroundImage, MemoryImage *theAlpha, const SpriteDesc &theDesc)
{
    MemoryImage *cutout = Sexy::CutoutImageFromAlpha(theBackgroundImage, theAlpha, theDesc.x, theDesc.y);

    int v5 = (int)(float)(theDesc.vx * -10.0);
    int v12 = v5;
    int v13 = (int)(float)(-10.0 * theDesc.vy);
    int v6 = 0;
    if (v5 < 0)
        v5 = 0;
    int v14 = v5;
    if (v13 >= 0)
        v6 = (int)(float)(-10.0 * theDesc.vy);
    int a4 = v6;
    int v16;
    int v17;
    if (v12 <= 0)
        v16 = -v12;
    else
        v16 = 0;
    if (v13 <= 0)
        v17 = -v13;
    else
        v17 = 0;
    int v7 = v12;
    if (v12 < 0)
        v7 = -v12;
    int v18 = cutout->mWidth + v7;
    int v8 = (int)(float)(-10.0 * theDesc.vy);
    if (v13 < 0)
        v8 = -v13;
    int v9 = cutout->mHeight + v8;

    MemoryImage *v10 = new MemoryImage(gSexyAppBase);
    v10->Create(v18, v9);

    Graphics g(v10);
    g.SetColor(Sexy::Color(80, 80, 80));
    g.SetColorizeImages(true);
    g.DrawImage(cutout, v14, a4);
    g.SetColorizeImages(false);
    g.DrawImage(cutout, v16, v17);

    delete cutout;
    mBackgroundSprites.push_back(SpriteImage());
    SpriteImage &aSprite = mBackgroundSprites.back();
    aSprite.x = theDesc.x;
    aSprite.y = theDesc.y;
    aSprite.vx = theDesc.vx;
    aSprite.vy = theDesc.vy;
    aSprite.mImage = v10;

    if (!gSexyAppBase->Is3DAccelerated())
    {
        gSexyAppBase->PrecacheAlpha(v10);
        gSexyAppBase->PrecacheNative(v10);
    }

    if (v12 < 0)
    {
        aSprite.x += v12;
    }

    if (v13 < 0)
    {
        aSprite.y += v13;
    }
}

void SpriteMgr::AddStar(int y)
{
    mStarList.push_back(StarInfo());

    StarInfo &aStar = mStarList.back();
    aStar.y = y;
    aStar.x = rand() % 640;

    switch (rand() % 3)
    {
    case 0:
        aStar.vy = 0.2;
        aStar.mColor = 0x404040;
        break;

    case 1:
        aStar.vy = 0.6;
        aStar.mColor = 0x909090;
        break;

    case 2:
        aStar.vy = 0.9;
        aStar.mColor = 0xFFFFFF;
        break;
    }
}

void SpriteMgr::UpdateHoles()
{
    for (HoleFlashList::iterator anItr = mHoleFlashList.begin(); anItr != mHoleFlashList.end(); ++anItr)
    {
        HoleFlash &aFlash = *anItr;

        aFlash.mUpdateCount++;
        if (aFlash.mUpdateCount < 0)
            continue;

        int aBrightness;
        bool v10 = false;
        if (aFlash.mUpdateCount < 30)
        {
            aBrightness = 255 * aFlash.mUpdateCount / 30;
        }
        else if (aFlash.mUpdateCount < 60)
        {
            aBrightness = 255 - (aFlash.mUpdateCount - 30) * 255 / 30;
        }
        else
        {
            v10 = true;
            aBrightness = 0;
        }

        HoleInfo &aHole = mHoleInfo[mHoleMappings[aFlash.mCurveNum]];
        aHole.mBrightness[aFlash.mCurveNum] = aBrightness;

        for (int i = 0; i < 3; i++)
        {
            if (aBrightness < aHole.mBrightness[i])
                aBrightness = aHole.mBrightness[i];
        }

        aHole.mTotalBrightness = aBrightness;
    }
}

void SpriteMgr::UpdateStars() {}

void SpriteMgr::InitStars() {}

void SpriteMgr::UpdateHole(int theCurveNum, float thePercentOpen)
{
    HoleInfo &aHole = mHoleInfo[mHoleMappings[theCurveNum]];
    aHole.mPercentOpen[theCurveNum] = thePercentOpen;

    float v4 = thePercentOpen;
    for (int i = 0; i < 3; i++)
    {
        thePercentOpen = fmaxf(aHole.mPercentOpen[i], thePercentOpen);
    }

    aHole.mFrame = (int)(Sexy::IMAGE_HOLE_COVER->mNumRows * thePercentOpen);
    if (aHole.mFrame >= Sexy::IMAGE_HOLE_COVER->mNumRows)
        aHole.mFrame = Sexy::IMAGE_HOLE_COVER->mNumRows - 1;
}

void SpriteMgr::UpdateHoleBrightness(int theCurveNum, int theBrightness)
{
}

void SpriteMgr::PlaceHole(int theCurveNum, int theX, int theY, float theRotation)
{
    printf("hole x: %d y: %d\n", theX, theY);
    int aCornerX = theX - Sexy::IMAGE_HOLE->mWidth / 2;
    int aCornerY = theY - Sexy::IMAGE_HOLE->mHeight / 2;

    while (theRotation < 0.0)
        theRotation += M_PI * 2;

    while (theRotation > M_PI * 2)
        theRotation -= M_PI * 2;

    if (fabs(theRotation) < 0.2)
        theRotation = 0.0;

    if (fabs(theRotation - M_PI / 2) < 0.2)
        theRotation = M_PI / 2;

    if (fabs(theRotation - M_PI) < 0.2)
        theRotation = M_PI;

    if (fabs(theRotation - M_PI * 1.5) < 0.2)
        theRotation = M_PI * 1.5;

    if (fabs(theRotation - M_PI * 2) < 0.2)
        theRotation = 0.0;

    int i;
    for (i = 0; i < mNumHoles; i++)
    {
        HoleInfo &aHole = mHoleInfo[i];

        if ((aHole.my - aCornerY) * (aHole.my - aCornerY) + (aHole.mx - aCornerX) * (aHole.mx - aCornerX) < 400)
        {
            break;
        }
    }

    if (i == mNumHoles)
    {
        HoleInfo &aHole = mHoleInfo[i];

        aHole.mx = aCornerX;
        aHole.my = aCornerY;
        aHole.mFrame = 0;
        aHole.mTotalBrightness = 0;
        aHole.mImageFrame = -1;
        aHole.mRotation = theRotation;

        for (int j = 0; j < 3; j++)
        {
            aHole.mPercentOpen[j] = 0.0;
            aHole.mBrightness[j] = 0;
        }

        mNumHoles++;
    }

    mHoleMappings[theCurveNum] = i;
}

void SpriteMgr::ClearHoleFlashes()
{
    mHoleFlashList.clear();

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            mHoleInfo[i].mBrightness[j] = 0;
        }

        mHoleInfo[i].mTotalBrightness = 0;
    }
}

void SpriteMgr::AddHoleFlash(int theCurveNum, int theStagger)
{
    mHoleFlashList.push_back(HoleFlash());

    HoleFlash &aFlash = mHoleFlashList.back();
    aFlash.mCurveNum = theCurveNum;
    aFlash.mUpdateCount = -theStagger;
}

void SpriteMgr::Update()
{
    ++mUpdateCnt;
    UpdateHoles();

    if (mSpaceScroll)
        UpdateStars();
}

void SpriteMgr::SetupSpace()
{
}

void SpriteMgr::DrawBackgroundTransition(Graphics *g)
{
    if (mBackgroundSprites.empty())
    {
        GenerateBackgroundTransitionSprites(((CircleShootApp *)gSexyAppBase)->mLevelParser->GetBackgroundTransition());
    }

    for (SpriteList::iterator anItr = mBackgroundSprites.begin(); anItr != mBackgroundSprites.end(); anItr++)
    {
        g->SetColorizeImages(true);
        g->SetColor(Color(0, 0, 0, 80));
        g->DrawImage(anItr->mImage,
                     anItr->x - (anItr->vx * 50.0f),
                     anItr->y - (anItr->vy * 50.0f));
        g->SetColorizeImages(false);
    }

    for (SpriteList::iterator anItr = mBackgroundSprites.begin(); anItr != mBackgroundSprites.end(); anItr++)
    {
        g->DrawImage(anItr->mImage, anItr->x, anItr->y);
    }
}

void SpriteMgr::UpdateBackgroundTransition(int theStep)
{
    for (SpriteList::iterator anItr = mBackgroundSprites.begin(); anItr != mBackgroundSprites.end(); anItr++)
    {
        int x = anItr->x;
        int y = anItr->y;

        if (theStep > 100)
        {
            anItr->x += anItr->vx * 5.0f;
            anItr->y += anItr->vy * 5.0f;
        }
        else if (theStep > 50)
        {
            anItr->x += Sexy::AppRand() % 5 - 2;
            anItr->y += Sexy::AppRand() % 5 - 2;
        }
        else
        {
            anItr->x += Sexy::AppRand() % 3 - 1;
            anItr->y += Sexy::AppRand() % 3 - 1;
        }

        if (x <= 0 && anItr->x >= 0 || x + anItr->mImage->mWidth >= 630 || anItr->x + anItr->mImage->mWidth < 640)
        {
            anItr->x = x;
        }

        if (y <= 0 && anItr->y >= 0 || y + anItr->mImage->mHeight >= 470 || anItr->y + anItr->mImage->mHeight < 480)
        {
            anItr->y = y;
        }
    }
}

void SpriteMgr::DrawBackground(Graphics *g)
{
    if (mInSpace)
    {
        DrawSpace(g);
    }
    else
    {
        g->DrawImage(mBackgroundImage, 0, 0);
    }

    for (int i = 0; i < mNumHoles; i++)
    {
        DrawHole(g, i);

        if (mHoleInfo[i].mTotalBrightness > 0)
        {
            g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
            g->SetColorizeImages(true);

            g->SetColor(Sexy::Color(mHoleInfo[i].mTotalBrightness, mHoleInfo[i].mTotalBrightness, mHoleInfo[i].mTotalBrightness));
            DrawHole(g, i);

            g->SetColorizeImages(false);
            g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
        }
    }
}

void SpriteMgr::DrawLevel(Graphics *g)
{
    DrawBackground(g);
}

void SpriteMgr::DrawSprites(Graphics *g, int thePriority)
{
    DrawSprites(g, mSprites[thePriority]);
}

void SpriteMgr::DrawBorder(Graphics *g)
{
    if (gImageCutter != NULL)
    {
        gImageCutter->Draw(g);
    }
}

void SpriteMgr::DrawHole(Graphics *g, int theHoleNum)
{
    int aHeight = Sexy::IMAGE_HOLE_COVER->mHeight / Sexy::IMAGE_HOLE_COVER->mNumRows;
    int aWidth = Sexy::IMAGE_HOLE_COVER->mWidth;
    Rect aRect(0, mHoleInfo[theHoleNum].mFrame * aHeight, aWidth, aHeight);

    if (gSexyAppBase->Is3DAccelerated())
    {
        g->DrawImageRotated(Sexy::IMAGE_HOLE, mHoleInfo[theHoleNum].mx, mHoleInfo[theHoleNum].my, mHoleInfo[theHoleNum].mRotation);
        g->DrawImageRotated(Sexy::IMAGE_HOLE_COVER, mHoleInfo[theHoleNum].mx, mHoleInfo[theHoleNum].my, mHoleInfo[theHoleNum].mRotation, &aRect);
    }
    else
    {
        g->DrawImage(Sexy::IMAGE_HOLE, mHoleInfo[theHoleNum].mx, mHoleInfo[theHoleNum].my);
        g->DrawImageCel(Sexy::IMAGE_HOLE_COVER, mHoleInfo[theHoleNum].mx, mHoleInfo[theHoleNum].my, mHoleInfo[theHoleNum].mFrame);
    }
}

// wasn't present in executable, but was in headers
void SpriteMgr::DrawHoleForCurve(Graphics *g, int theCurveNum)
{
    DrawHole(g, theCurveNum);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SpriteImage::~SpriteImage() {}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SpriteMgr::HoleInfo::~HoleInfo() {}