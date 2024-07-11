#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/Image.h>
#include <SexyAppFramework/MemoryImage.h>
#include <SexyAppFramework/HyperlinkWidget.h>
#include <SexyAppFramework/Widget.h>
#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/SexyVector.h>
#include <SexyAppFramework/SoundManager.h>
#include <SexyAppFramework/SoundInstance.h>
#include <SexyAppFramework/ResourceManager.h>
#include <SexyAppFramework/DDImage.h>

#include "CircleShootApp.h"
#include "CircleButton.h"
#include "LevelParser.h"
#include "ProfileMgr.h"
#include "Res.h"
#include "MainMenu.h"

#include <math.h>

using namespace Sexy;

class MainMenuOverlay : public Widget
{
public:
    MainMenuOverlay(MainMenu *theMainMenu) : Widget()
    {
        mMenu = theMainMenu;
        mMouseVisible = false;
        mHasTransparencies = true;
        mHasAlpha = true;
        Resize(0, 0, CIRCLE_WINDOW_WIDTH, CIRCLE_WINDOW_HEIGHT);
    }

    virtual ~MainMenuOverlay() {}

    virtual void Draw(Graphics *g)
    {
        mMenu->DrawOverlay(g);
    }

    MainMenu *mMenu;
};

bool MainMenu::mMoveEyes = false;

MainMenu::MainMenu()
{
    mEyeCutoutPos = Point(191, 331);
    mUFOImage = NULL;
    mFlash = 0;
    mMainMenuOverlay = new MainMenuOverlay(this);
    mArcadeButton = MakeButton(0, this, "", CircleButton::CB_ClickSound | CircleButton::CB_ChoralSound, Sexy::IMAGE_MM_ARCADE, 3);
    mGauntletButton = MakeButton(1, this, "", CircleButton::CB_ClickSound | CircleButton::CB_ChoralSound, Sexy::IMAGE_MM_GAUNTLET, 3);
    mOptionsButton = MakeButton(2, this, "", CircleButton::CB_ClickSound | CircleButton::CB_ChoralSound, Sexy::IMAGE_MM_OPTIONS, 3);
    mMoreGamesButton = MakeButton(3, this, "", CircleButton::CB_ClickSound | CircleButton::CB_ChoralSound, Sexy::IMAGE_MM_MOREGAMES, 3);
    mQuitButton = MakeButton(4, this, "", CircleButton::CB_ClickSound | CircleButton::CB_ChoralSound, Sexy::IMAGE_MM_QUIT, 3);

    mEyesImage = Sexy::CutoutImageFromAlpha((MemoryImage *)Sexy::IMAGE_MM_BACK, (MemoryImage *)Sexy::IMAGE_MM_EYEMASK, mEyeCutoutPos.mX, mEyeCutoutPos.mY);
    mNotYouLink = new HyperlinkWidget(5, this);
    mNotYouLink->mLabel = "(If this is not you, click here.)";
    mNotYouLink->SetFont(Sexy::FONT_MAIN10);
    mNotYouLink->mUnderlineSize = 0;
    mNotYouLink->mColor = Color(0x5B3700);
    mNotYouLink->mOverColor = Color(0xFFF763);

    int aTextWidth = Sexy::FONT_MAIN10->StringWidth(mNotYouLink->mLabel) + 20;
    mNotYouLink->Resize(
        (CIRCLE_WINDOW_WIDTH - aTextWidth) / 2,
        30,
        aTextWidth,
        (Sexy::FONT_MAIN10->GetHeight() + 20));
    mUFOScale = 1.0f;
    mDoUFOEasterEgg = false;
    mUFOSound = gSexyAppBase->mSoundManager->GetSoundInstance(Sexy::SOUND_UFO);

    SyncProfile();
}

MainMenu::~MainMenu()
{
    delete mArcadeButton;
    delete mGauntletButton;
    delete mOptionsButton;
    delete mMoreGamesButton;
    delete mQuitButton;
    delete mNotYouLink;
    delete mEyesImage;
    delete mUFOImage;
    delete mMainMenuOverlay;
    mUFOSound->Release();
    gSexyAppBase->mResourceManager->DeleteExtraImageBuffers("MainMenu");
}

void MainMenu::KeyChar(char theChar) {}

void MainMenu::DrawOverlay(Graphics *g)
{
    if (mDoUFOEasterEgg)
    {
        g->DrawImageCel(
            mUFOImage,
            mUFOPoint.mX - mUFOImage->GetCelWidth() / 2,
            mUFOPoint.mY - mUFOImage->GetCelHeight() / 2,
            (mUpdateCnt >> 1) % 5);
    }

    if (mFlash != 0)
    {
        int aFlash = mFlash * 255;
        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->SetColor(Color(aFlash / 25, aFlash / 25, aFlash / 25));
        g->FillRect(0, 0, CIRCLE_WINDOW_WIDTH, CIRCLE_WINDOW_HEIGHT);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    }
}

void MainMenu::OrderInManagerChanged()
{
    Widget::OrderInManagerChanged();

    mWidgetManager->PutInfront(mMainMenuOverlay, this);
    mWidgetManager->PutInfront(mNotYouLink, this);
    mWidgetManager->PutInfront(mQuitButton, this);
    mWidgetManager->PutInfront(mOptionsButton, this);
    mWidgetManager->PutInfront(mMoreGamesButton, this);
    mWidgetManager->PutInfront(mGauntletButton, this);
    mWidgetManager->PutInfront(mArcadeButton, this);
}

void MainMenu::RemovedFromManager(WidgetManager *theWidgetManager)
{
    Widget::RemovedFromManager(theWidgetManager);

    theWidgetManager->RemoveWidget(mArcadeButton);
    theWidgetManager->RemoveWidget(mGauntletButton);
    theWidgetManager->RemoveWidget(mMoreGamesButton);
    theWidgetManager->RemoveWidget(mOptionsButton);
    theWidgetManager->RemoveWidget(mQuitButton);
    theWidgetManager->RemoveWidget(mNotYouLink);
    theWidgetManager->RemoveWidget(mMainMenuOverlay);
}

void MainMenu::ButtonDepress(int theId)
{
    CircleShootApp *app = GetCircleShootApp();

    switch (theId)
    {
    case 0:
        app->ShowAdventureScreen(true, false);
        break;
    case 1:
        app->ShowPracticeScreen(true);
        break;
    case 2:
        app->DoOptionsDialog();
        break;
    case 3:
        app->ShowMoreGamesScreen();
        break;
    case 4:
        app->DoConfirmQuitDialog();
        break;
    case 5:
        app->DoUserDialog();
        break;
    default:
        break;
    }
}

void MainMenu::AddedToManager(WidgetManager *theWidgetManager)
{
    Widget::AddedToManager(theWidgetManager);

    mArcadeButton->Layout(48, this, 452, 64, 0, 0);
    mGauntletButton->Layout(48, this, 436, 153, 0, 0);
    mOptionsButton->Layout(48, this, 418, 236, 0, 0);
    mMoreGamesButton->Layout(48, this, 394, 305, 0, 0);
    mQuitButton->Layout(48, this, 496, 315, 0, 0);

    theWidgetManager->AddWidget(mArcadeButton);
    theWidgetManager->AddWidget(mGauntletButton);
    theWidgetManager->AddWidget(mMoreGamesButton);
    theWidgetManager->AddWidget(mOptionsButton);
    theWidgetManager->AddWidget(mQuitButton);
    theWidgetManager->AddWidget(mNotYouLink);
    theWidgetManager->AddWidget(mMainMenuOverlay);
}

FPoint FindEyePos(
    Sexy::MemoryImage *anSrcImage,
    Sexy::MemoryImage *aDestImage,
    Point &aPoint1,
    Point &aPoint2,
    Point &aPoint3,
    FPoint &anEyePoint)
{
    SexyVector3 v23(aPoint1.mX, aPoint1.mY + aPoint2.mY, aPoint1.mX + aPoint2.mX);
    SexyVector3 v25(aPoint3.mX - v23.z, aPoint3.mY - v23.y, 0.0f);
    v25 = v25.Normalize();

    if (fabs(v25.x) > 0.1f || fabs(v25.y) > 0.1f)
    {
        uint *bits = (uint *)aDestImage->GetBits();
        int i = 0;
        for (;; i++)
        {
            float v15 = i * v25.y + aPoint1.mY;
            float v16 = i * v25.x + aPoint1.mX;

            if (v16 < 0.0f)
                break;
            if (v15 < 0.0f)
                break;

            if (v16 >= aDestImage->GetWidth() || v15 >= aDestImage->GetHeight())
                break;

            if ((bits[(int)v15 * aDestImage->GetWidth() + (int)v16] & 0xff000000) > 0x63ffffff)
            {
                break;
            }
        }

        int v18 = (i - anSrcImage->GetWidth() / 2) - 1;
        float v19 = (v18 >= 0) ? v18 : 0.0f;

        return FPoint(
            v25.x * v19 + v23.z,
            v25.y * v19 + v23.y);
    }
    else
    {
        return anEyePoint;
    }
}

void MainMenu::CalcEyePos()
{
    if (!mMoveEyes && !mDoUFOEasterEgg)
    {
        mEyeLeft = FPoint(219.0f, 368.0f);
        mEyeRight = FPoint(276.0f, 355.0f);
    }
    else
    {
        Point aLastMouse(mWidgetManager->mLastMouseX, mWidgetManager->mLastMouseY);

        if (mDoUFOEasterEgg)
        {
            aLastMouse = mUFOPoint;
        }

        Point aPoint1(29, 25);
        Point aPoint2(87, 16);

        FPoint aLeft = FindEyePos((MemoryImage *)Sexy::IMAGE_MM_EYELEFT, mEyesImage, aPoint1, mEyeCutoutPos, aLastMouse, mEyeLeft);
        FPoint aRight = FindEyePos((MemoryImage *)Sexy::IMAGE_MM_EYERIGHT, mEyesImage, aPoint2, mEyeCutoutPos, aLastMouse, mEyeRight);

        FPoint v3 = aLeft - mEyeLeft;
        if (SexyVector2(v3.mX, v3.mY).Magnitude() > 0.0f)
        {
            mEyeLeft = aLeft;
            MarkDirty();
        }

        FPoint v4 = aRight - mEyeRight;
        if (SexyVector2(v4.mX, v4.mY).Magnitude() > 0.0f)
        {
            mEyeRight = aRight;
            MarkDirty();
        }
    }
}

void MainMenu::MouseMove(int x, int y)
{
    Widget::MouseMove(x, y);
    CalcEyePos();
}

void MainMenu::MouseDown(int x, int y, int theClickCount)
{
    Widget::MouseDown(x, y, theClickCount);

    if (x > 62 && x < 327 && y > 275 && y < 470)
    {
        mMoveEyes = !mMoveEyes;
        CalcEyePos();
        MarkDirty();
    }

    if (mDoUFOEasterEgg)
    {
        if (x > mUFOPoint.mX - mUFOImage->mWidth / 2 && x < mUFOPoint.mX + mUFOImage->mWidth / 2 &&
            y > mUFOPoint.mY - mUFOImage->mHeight / 2 && y < mUFOPoint.mY + mUFOImage->mHeight / 2)
        {
            mFlash = 1;
        }
    }
}

void MainMenu::SyncProfile()
{
    mShowHat = false;

    UserProfile *aProfile = GetCircleShootApp()->mProfile;
    if (!aProfile)
        return;

    mShowHat = true;
    LevelParser *aLevelParser = GetCircleShootApp()->mLevelParser;

    int aProgressCount = aLevelParser->mBoardProgression.size() + 1;
    for (int i = 0; i < aProgressCount; i++)
    {
        std::string aName;
        std::string aDisplayName;

        aLevelParser->GetPracticeBoard(i, aName, aDisplayName);
        UserProfile::BoardMap::iterator anItr = aProfile->mMaxBoardLevel.find(aName);
        if (anItr == aProfile->mMaxBoardLevel.end() || anItr->second <= 20)
        {
            mShowHat = false;
            return;
        }
    }
}

void MainMenu::AddUFOMove(IntPoint const &theStartPos, IntPoint const &theEndPos, int theDuration, int theStagger)
{
    mUFOMoveList.push_back(UFOMove());
    UFOMove &aMove = mUFOMoveList.back();
    aMove.mUpdateCnt = -theStagger;
    aMove.mStartPos = theStartPos;
    aMove.mEndPos = theEndPos;
    aMove.mDuration = theDuration;
}

void MainMenu::DoUFO()
{
    if (mDoUFOEasterEgg)
        return;

    mFlash = 0;
    if (mUFOImage == NULL)
    {
        mUFOImage = gSexyAppBase->GetImage("images/mmufo", false);
        if (mUFOImage == NULL)
            return;
        mUFOImage->mNumRows = 6;
    }

    if (mUFOSound)
        mUFOSound->Play(true, false);

    mDoUFOEasterEgg = true;
    mUFOMoveList.clear();
    mUFOScaleList.clear();
    mUFOState = 0;

    IntPoint aEndPoint(300, 210);
    IntPoint aStartPoint(-50, -50);

    AddUFOMove(aStartPoint, aEndPoint, 300, 0);
}

void MainMenu::UpdateUFOMove()
{
    if (mUFOMoveList.empty())
    {
        if (mDoUFOEasterEgg)
        {
            if (mUFOStateCount == 0)
            {
                mUFOPoint2.mX = mUFOPoint.mX;
                mUFOPoint2.mY = mUFOPoint.mY + 15;
            }

            float angle = mUFOStateCount * SEXY_PI / 180.0f;
            mUFOPoint.mX = sinf(angle) * 40.0f + mUFOPoint2.mX;
            mUFOPoint.mY = cosf(angle) * -15.0f + mUFOPoint2.mY;
            mUFOStateCount++;
        }
    }
    else
    {
        UFOMove &move = mUFOMoveList.front();
        move.mUpdateCnt++;
        if (move.mUpdateCnt >= 0)
        {
            mUFOStateCount = 0;
            float t = move.mUpdateCnt / float(move.mDuration);
            mUFOPoint.mX = (move.mStartPos.mX * (1.0 - t)) + (move.mEndPos.mX * t);
            mUFOPoint.mY = (move.mStartPos.mY * (1.0 - t)) + (move.mEndPos.mY * t);

            CalcEyePos();

            if (move.mUpdateCnt >= move.mDuration)
            {
                mUFOMoveList.pop_front();
            }
        }
    }
}

void MainMenu::UpdateUFOScale()
{
    if (mUFOScaleList.empty())
        return;

    UFOScale &scale = mUFOScaleList.front();
    scale.mUpdateCnt++;
    if (scale.mUpdateCnt >= 0)
    {
        float t = scale.mUpdateCnt / scale.mDuration;
        mUFOScale = (1.0 - t) * scale.mStartScale + t * scale.mEndScale;
        if (scale.mUpdateCnt >= scale.mDuration)
            mUFOScaleList.pop_front();
    }
}

void MainMenu::Update()
{
    Widget::Update();
    CalcEyePos();

    if (gButtonSequenceCount > 2)
    {
        DoUFO();
        gButtonSequenceCount = 0;
    }

    if (mDoUFOEasterEgg)
    {
        UpdateUFOMove();
        UpdateUFOScale();

        if (mUFOState != 0)
        {
            if (mUFOState == 1)
            {
                if (mUFOMoveList.empty())
                {
                    mUFOState = 2;
                }
            }
            else
            {
                mUFOState++;
                if (mUFOState == 100)
                {
                    mDoUFOEasterEgg = false;
                    if (mUFOSound)
                        mUFOSound->Stop();
                }
            }
        }
        else if (mUFOStateCount > 1000)
        {
            mUFOState = 1;
            IntPoint a3(-200, -200);
            AddUFOMove(mUFOPoint, a3, 50, 0);
        }

        MarkDirty();
    }

    if (mFlash > 0)
    {
        mFlash++;
        if (mFlash == 25)
        {
            GetCircleShootApp()->ShowCreditsScreen(false);
        }
    }

    if ((mUpdateCnt & 3) == 0)
        MarkDirty();
}

void MainMenu::Draw(Graphics *g)
{
    Widget::Draw(g);
    int skyScroll = (mUpdateCnt / 8) % Sexy::IMAGE_MM_SKY->mWidth;
    g->DrawImage(Sexy::IMAGE_MM_SKY, skyScroll - Sexy::IMAGE_MM_SKY->mWidth, 0);
    g->DrawImage(Sexy::IMAGE_MM_SKY, skyScroll, 0);
    g->DrawImage(Sexy::IMAGE_MM_SKY, skyScroll + Sexy::IMAGE_MM_SKY->mWidth, 0);
    g->DrawImage(Sexy::IMAGE_MM_BACK, 0, 0);

    g->DrawImage(Sexy::IMAGE_MM_EYELEFT,
                 (mEyeLeft.mX - (Sexy::IMAGE_MM_EYELEFT->mWidth / 2)),
                 (mEyeLeft.mY - (Sexy::IMAGE_MM_EYELEFT->mHeight / 2)));
    g->DrawImage(Sexy::IMAGE_MM_EYERIGHT,
                 (mEyeRight.mX - (Sexy::IMAGE_MM_EYERIGHT->mWidth / 2)),
                 (mEyeRight.mY - (Sexy::IMAGE_MM_EYERIGHT->mHeight / 2)));

    if (mShowHat)
    {
        g->DrawImage(Sexy::IMAGE_MM_HAT, 188, 247);
    }

    CircleShootApp *app = GetCircleShootApp();

    g->SetColorizeImages(true);
    g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);

    if (app->Is3DAccelerated())
    {
        double angle = (mUpdateCnt * SEXY_PI / 180.0) * 0.3;
        g->DrawImageRotated(Sexy::IMAGE_SUNGLOW, -69, -63, angle);
    }
    else
    {
        g->DrawImage(Sexy::IMAGE_SUNGLOW, -69, -63);
    }

    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    g->SetColorizeImages(false);
    g->DrawImage(Sexy::IMAGE_MM_SUN, 0, 2);

    g->SetColor(Sexy::Color(0xFFFFFF));
    g->SetFont(Sexy::FONT_BROWNTITLE);

    std::string welcome;
    if (app->mProfile)
    {
        welcome = "Welcome to Zuma,";
        welcome += SexyString(" ") + app->mProfile->mName + '!';
    }
    else
    {
        welcome = "Welcome to Zuma!";
    }

    int textWidth = Sexy::FONT_BROWNTITLE->StringWidth(welcome);
    g->DrawString(welcome, (CIRCLE_WINDOW_WIDTH - textWidth) / 2, 32);
}