#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/ButtonListener.h>
#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/Widget.h>
#include <SexyAppFramework/ResourceManager.h>
#include <SexyAppFramework/HyperlinkWidget.h>

#include "CircleShootApp.h"
#include "Res.h"
#include "HelpScreen.h"
#include "CircleButton.h"
#include "CircleCheckbox.h"
#include "Gun.h"
#include "Bullet.h"
#include "Ball.h"
#include "ProfileMgr.h"

#include <math.h>

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class HelpWidget1 : public Widget
{
public:
    HelpWidget1();
    virtual ~HelpWidget1();

    virtual void Update();
    virtual void Draw(Graphics *g);
    void UpdateMouse();
    void UpdateGunAngle();
    void UpdateBullets();

    Gun *mGun;
    Point mGunTarget;
    Point mUnk60;
    Point mLastGunShotPoint;
    int mLastGunShotCnt;
    typedef std::list<Bullet *> BulletList;
    BulletList mBulletList;
};

class HelpWidget2 : public Widget
{
public:
    HelpWidget2();
    virtual ~HelpWidget2();

    virtual void Update();
    virtual void Draw(Graphics *g);
    void Reset();

    Bullet *mBullet;
    Ball *mBalls[6];
    int mAnimCnt;
    bool mDiverseColors;
};

class HelpWidget3 : public Widget
{
public:
    HelpWidget3();
    virtual ~HelpWidget3();

    virtual void Update();
    virtual void Draw(Graphics *g);

    Gun *mGun;
};

class HelpBack : public Widget
{
public:
    HelpBack();
    virtual ~HelpBack();

    virtual void Draw(Graphics *g);
};

HelpWidget1::HelpWidget1() : Widget()
{
    mHasTransparencies = true;
    mHasAlpha = true;
    mGun = new Gun();
    mGun->SetFireSpeed(3.0);
    mGun->Reload(Sexy::AppRand() % 4);
    mGun->Reload(Sexy::AppRand() % 4);

    mGunTarget = Point(240, 160);
    mUnk60 = Point(240, 160);
    mLastGunShotCnt = -100000;
    mGun->SetPos(mGun->GetWidth() / 2 + 10, mGun->GetHeight() / 2 + 60);
    UpdateGunAngle();
}

HelpWidget1::~HelpWidget1()
{
    delete mGun;

    for (BulletList::iterator aBulletItr = mBulletList.begin();
         aBulletItr != mBulletList.end(); aBulletItr++)
        delete *aBulletItr;
}

void HelpWidget1::Update()
{
    Widget::Update();
    if (mUpdateCnt >= 100)
    {
        mGun->Update();
        Bullet *aFiredBullet = mGun->GetFiredBullet();
        if (aFiredBullet)
        {
            mBulletList.push_back(aFiredBullet);
            if (mGun->NeedsReload())
            {
                int rand = Sexy::AppRand() % 4;
                mGun->Reload(rand, 1, PowerType_None);
            }
        }
        if ((mUpdateCnt % 200) == 0)
        {
            mGun->StartFire(1);
            mLastGunShotCnt = mUpdateCnt;
            mLastGunShotPoint = mGunTarget;
        }
        UpdateMouse();
        UpdateBullets();
        MarkDirty();
    }
}

void HelpWidget1::Draw(Graphics *g)
{
    Widget::Draw(g);
    mGun->Draw(g);
    for (BulletList::iterator aBulletItr = mBulletList.begin();
         aBulletItr != mBulletList.end(); aBulletItr++)
        (*aBulletItr)->Draw(g);

    g->SetFont(Sexy::FONT_DIALOG);
    Rect aTopTextRect = Rect(30, 40, mWidth - 38, 2 * Sexy::FONT_DIALOG->GetHeight());
    Rect aBottomTextRect = Rect(4, mHeight - 2 * Sexy::FONT_DIALOG->GetHeight() - 12,
                                mWidth - 8, 2 * Sexy::FONT_DIALOG->GetHeight());

    g->SetColor(Color(0xFFFF00));
    WriteWordWrapped(g, aTopTextRect, "Shoot balls by clicking on screen where you want the ball to go.", -1, 0);

    g->SetColor(Color(0xFFA500));
    WriteWordWrapped(g, aBottomTextRect, "The gem in the back of the frog shows the color of the next ball.", -1, 0);

    if (mUpdateCnt - mLastGunShotCnt < 100)
    {
        Sexy::Color aColor = Color(0xFFFFFF);
        g->SetColor(aColor);
        g->SetFont(Sexy::FONT_PLAIN);
        g->DrawString("CLICK!", mLastGunShotPoint.mX - 65, mLastGunShotPoint.mY - 10);
        g->SetFont(Sexy::FONT_DIALOG);
        g->DrawLine(mLastGunShotPoint.mX - 3, mLastGunShotPoint.mY - 3,
                    mLastGunShotPoint.mX - 8, mLastGunShotPoint.mY - 8);
        g->DrawLine(mLastGunShotPoint.mX + 3, mLastGunShotPoint.mY + 3,
                    mLastGunShotPoint.mX + 8, mLastGunShotPoint.mY + 8);
        g->DrawLine(mLastGunShotPoint.mX - 3, mLastGunShotPoint.mY + 3,
                    mLastGunShotPoint.mX - 8, mLastGunShotPoint.mY + 8);
        g->DrawLine(mLastGunShotPoint.mX + 3, mLastGunShotPoint.mY - 3,
                    mLastGunShotPoint.mX + 8, mLastGunShotPoint.mY - 8);
        g->DrawLine(mLastGunShotPoint.mX - 3, mLastGunShotPoint.mY,
                    mLastGunShotPoint.mX - 8, mLastGunShotPoint.mY);
        g->DrawLine(mLastGunShotPoint.mX + 3, mLastGunShotPoint.mY,
                    mLastGunShotPoint.mX + 8, mLastGunShotPoint.mY);
        g->DrawLine(mLastGunShotPoint.mX, mLastGunShotPoint.mY + 3,
                    mLastGunShotPoint.mX, mLastGunShotPoint.mY + 8);
        g->DrawLine(mLastGunShotPoint.mX, mLastGunShotPoint.mY - 3,
                    mLastGunShotPoint.mX, mLastGunShotPoint.mY - 8);
    }

    g->DrawImage(Sexy::IMAGE_CURSOR_POINTER,
                 mGunTarget.mX - IMAGE_CURSOR_POINTER->mWidth / 2,
                 mGunTarget.mY - IMAGE_CURSOR_POINTER->mHeight / 2);
}

void HelpWidget1::UpdateMouse()
{
    if (mGunTarget == mUnk60)
    {
        mUnk60.mY = mGunTarget.mY > 90 ? 90 : 180;
        return;
    }

    if (mUpdateCnt - mLastGunShotCnt > 50)
    {
        if (mGunTarget.mX < mUnk60.mX)
            mGunTarget.mX++;
        else if (mGunTarget.mX > mUnk60.mX)
            mGunTarget.mX--;

        if (mGunTarget.mY < mUnk60.mY)
            mGunTarget.mY++;
        else if (mGunTarget.mY > mUnk60.mY)
            mGunTarget.mY--;

        UpdateGunAngle();
    }
}

void HelpWidget1::UpdateGunAngle()
{
    float anAngle;
    if (mGunTarget.mX - mGun->GetCenterX() != 0)
    {
        anAngle = atan2(float(mGun->GetCenterY() - mGunTarget.mY) /
                            float(mGunTarget.mX - mGun->GetCenterX()),
                        1.0);
    }
    else if (mGunTarget.mY >= mGun->GetCenterY())
    {
        anAngle = -SEXY_PI / 2;
    }
    else
    {
        anAngle = SEXY_PI / 2;
    }

    mGun->SetAngle(anAngle + SEXY_PI / 2);
}

void HelpWidget1::UpdateBullets()
{
    for (BulletList::iterator aBulletItr = mBulletList.begin();
         aBulletItr != mBulletList.end();)
    {
        Bullet *aBullet = *aBulletItr;
        aBullet->Update();

        if (
            aBullet->GetX() < 0 ||
            aBullet->GetY() < 0 ||
            (aBullet->GetX() - aBullet->GetRadius() > mWidth) ||
            (aBullet->GetY() - aBullet->GetRadius() > mHeight))
        {
            delete aBullet;
            aBulletItr = mBulletList.erase(aBulletItr);
        }
        else
        {
            aBulletItr++;
        }
    }
}

HelpWidget2::HelpWidget2() : Widget()
{
    mHasTransparencies = true;
    mHasAlpha = true;
    mBullet = new Bullet();
    for (int i = 0; i < 6; i++)
    {
        mBalls[i] = new Ball();
        mBalls[i]->SetRotation(SEXY_PI / 2, 1);
        mBalls[i]->SetFrame(Sexy::AppRand() % 50);
    }

    mDiverseColors = false;
    Reset();
}

HelpWidget2::~HelpWidget2()
{
    delete mBullet;
    for (int i = 0; i < 6; i++)
        delete mBalls[i];
}

void HelpWidget2::Update()
{
    Widget::Update();
    if (mUpdateCnt < 100)
        return;

    MarkDirty();
    if (mAnimCnt != 0)
    {
        if (mUpdateCnt > mAnimCnt)
        {
            mDiverseColors = !mDiverseColors;
            Reset();
        }

        Ball *aBall = mBalls[5];
        if (aBall->GetClearCount() != 0 && aBall->GetClearCount() < 40)
        {
            aBall->SetClearCount(aBall->GetClearCount() + 1);
            mBalls[0]->SetClearCount(aBall->GetClearCount() + 1);
            mBalls[1]->SetClearCount(aBall->GetClearCount() + 1);
        }
        else if (aBall->GetSuckCount() != 0)
        {
            int suckCount = aBall->GetSuckCount() / -8;
            float x = suckCount + aBall->GetX();
            aBall->SetPos(x, aBall->GetY());
            aBall->IncFrame(suckCount);
            float ball1X = mBalls[1]->GetX() + 32.0;

            if (aBall->GetX() > ball1X)
            {
                aBall->SetSuckCount(aBall->GetSuckCount() + 1);
            }
            else
            {
                aBall->SetPos(ball1X, aBall->GetY());
                aBall->SetSuckCount(0);
                aBall->StartClearCount(0);
                mBalls[0]->StartClearCount(0);
                mBalls[1]->StartClearCount(0);
            }
        }
    }
    else if (mBullet->GetClearCount() != 0)
    {
        if (mBullet->GetClearCount() < 40)
        {
            mBullet->SetClearCount(mBullet->GetClearCount() + 1);
            for (int i = 2; i < 5; i++)
                mBalls[i]->SetClearCount(mBalls[i]->GetClearCount() + 1);
        }
        else
        {
            mAnimCnt = mUpdateCnt + 250;
            if (mDiverseColors)
                mBalls[5]->SetSuckCount(1);
        }
    }
    else
    {
        mBullet->Update();
        if (mBalls[0]->GetY() - 16.0 <= mBullet->GetY())
        {
            if (mBullet->GetY() < mBalls[0]->GetY())
            {
                int diff = (int)(16.0 - (mBalls[0]->GetY() - mBullet->GetY()));
                float rot = diff * (SEXY_PI / 32);
                mBullet->SetRotation(rot, 1);

                int offset = 80 - diff;
                for (int i = 0; i < 3; i++)
                {
                    float x = mBalls[i]->GetX();
                    mBalls[i]->SetPos(offset, mBalls[i]->GetY());
                    mBalls[i]->IncFrame((int)(mBalls[i]->GetX() - x));
                    offset += 32;
                }

                offset = diff + 176;
                for (int i = 3; i < 6; i++)
                {
                    float x = mBalls[i]->GetX();
                    mBalls[i]->SetPos(offset, mBalls[i]->GetY());
                    mBalls[i]->IncFrame((int)(mBalls[i]->GetX() - x));
                    offset += 32;
                }
            }
            else
            {
                mBullet->SetPos(mBullet->GetX(), mBalls[0]->GetY());
                mBullet->SetVelocity(0.0, 0.0);
                mBullet->SetRotation(SEXY_PI / 2, 1);
                mBullet->StartClearCount(0);
                for (int i = 2; i < 5; i++)
                    mBalls[i]->StartClearCount(0);
            }
        }
    }
}

void HelpWidget2::Draw(Graphics *g)
{
    Widget::Draw(g);
    for (int i = 0; i < 6; i++)
    {
        if (mBalls[i]->GetClearCount() < 40)
            mBalls[i]->Draw(g);
    }

    if (mUpdateCnt >= 100)
    {
        if (mBullet->GetClearCount() < 40)
            mBullet->Draw(g);
    }

    g->SetColor(Color(0xFFFFFF));
    g->SetFont(Sexy::FONT_DIALOG);

    Rect aTopTextRect = Rect(8, 40, mWidth - 38, 2 * Sexy::FONT_DIALOG->GetHeight());
    Rect aBottomTextRect = Rect(4, mHeight - 2 * Sexy::FONT_DIALOG->GetHeight() - 12,
                                mWidth - 8, 2 * Sexy::FONT_DIALOG->GetHeight());

    g->SetColor(Color(0xFFFF00));
    WriteWordWrapped(g, aTopTextRect, "Clear balls by getting three or more of the same color in a row.", -1, 0);

    g->SetColor(Color(0xFFA500));
    WriteWordWrapped(g, aBottomTextRect, "Like colored balls attract and can create chain reactions!", -1, 0);
}

void HelpWidget2::Reset()
{
    mAnimCnt = 0;
    mBullet->SetPos(-10.0, 20.0);
    mBullet->SetVelocity(1.3, 1.0);
    mBullet->SetClearCount(0);
    float theRot = atan2(mBullet->GetVelY(), mBullet->GetVelX());
    mBullet->SetRotation(theRot, 1);

    int aBallTypes[6] = {1, 1, 0, 0, 0, 1};
    if (!mDiverseColors)
        aBallTypes[5] = 2;

    int anOffset = 80;
    for (int i = 0; i < 6; i++)
    {
        mBalls[i]->SetPos(anOffset, 150.0);
        mBalls[i]->SetClearCount(0);
        mBalls[i]->SetSuckCount(0);
        mBalls[i]->SetType(aBallTypes[i]);
        anOffset += 32;
    }
}

HelpWidget3::HelpWidget3() : Widget()
{
    mHasTransparencies = true;
    mHasAlpha = true;

    mGun = new Gun();
    mGun->Reload(3);
    mGun->Reload(2);
}

HelpWidget3::~HelpWidget3()
{
    delete mGun;
}

void HelpWidget3::Update()
{
    Widget::Update();

    if ((mUpdateCnt % 200) == 0)
    {
        mGun->SwapBullets(false);
    }

    MarkDirty();
}

void HelpWidget3::Draw(Graphics *g)
{
    Widget::Draw(g);
    g->SetFont(Sexy::FONT_DIALOG);
    g->SetColor(Color(0xFFFFFF));
    g->DrawImage(Sexy::IMAGE_HOLE, 65, 20);
    g->DrawImageCel(Sexy::IMAGE_HOLE_COVER, 65, 20, 0);

    Rect aTopTextRect = Rect(30, mHeight - 2 * Sexy::FONT_DIALOG->GetHeight() - 15, 160, 0);
    g->SetColor(Color(0xFFFF00));
    WriteWordWrapped(g, aTopTextRect, "Don't let the balls reach the skull!", -1, 0);

    g->DrawImageCel(Sexy::IMAGE_COIN, 282, 45, (mUpdateCnt / 4) % 30);

    Rect aBottomTextRect = Rect(220, mHeight - 2 * Sexy::FONT_DIALOG->GetHeight() - 15, 160, 0);
    g->SetColor(Color(0xFFFF00));
    WriteWordWrapped(g, aBottomTextRect, "Shoot coins for extra points!", -1, 0);

    mGun->SetPos(500, 65);
    mGun->Draw(g);

    Rect aRightTextRect = Rect(430, mHeight - 2 * Sexy::FONT_DIALOG->GetHeight() - 15, 170, 0);
    int frame = 1;

    if ((mUpdateCnt % 200) < 80 && mUpdateCnt > 100)
    {
        frame = 0;
        g->SetFont(Sexy::FONT_PLAIN);
        g->SetColor(Color(0xFFFFFF));
        g->DrawString("CLICK!", 560, 55);
        g->SetFont(Sexy::FONT_DIALOG);
    }

    g->DrawImageCel(Sexy::IMAGE_RIGHT_MOUSE, 580, 60, frame);

    g->SetColor(Color(0xFFFF00));
    WriteWordWrapped(g, aRightTextRect, "Right mouse button will swap the balls.", -1, 0);
}

HelpBack::HelpBack() : Widget()
{
    Resize(0, 0, CIRCLE_WINDOW_WIDTH, CIRCLE_WINDOW_HEIGHT);
}

HelpBack::~HelpBack()
{
}

void HelpBack::Draw(Graphics *g)
{
    g->DrawImage(Sexy::IMAGE_HELP_BACK, 11, 51);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

HelpScreen::HelpScreen() : Widget()
{
    LoadResourceGroup("HelpScreen");
    mHelpWidget1 = new HelpWidget1();
    mHelpWidget2 = new HelpWidget2();
    mHelpWidget3 = new HelpWidget3();
    mHelpBack = new HelpBack();
    mShowHelpCheckbox = Sexy::MakeCheckbox(0, this);
    mOKButton = MakeButton(0, this, "OK", CircleButton::CB_ClickSound, NULL, 3);
    mShowHelpCheckbox->mChecked = !GetCircleShootApp()->mProfile->mShowHelpScreen;
}

HelpScreen::~HelpScreen()
{
    delete mHelpWidget1;
    delete mHelpWidget2;
    delete mHelpWidget3;
    delete mHelpBack;
    delete mShowHelpCheckbox;
    delete mOKButton;
    FreeResourceGroup("HelpScreen");
}

void HelpScreen::Resize(int theX, int theY, int theWidth, int theHeight)
{
    Widget::Resize(theX, theY, theWidth, theHeight);
    mHelpWidget1->Resize(mX + 10, mY + 25, 305, 220);
    mHelpWidget2->Layout(LAY_SameTop | LAY_Right | LAY_SameSize, mHelpWidget1, 10, 0, 0, 0);
    mHelpWidget3->Layout(LAY_SameLeft | LAY_Below | LAY_SetHeight, mHelpWidget1, 0, 10, 0, 160);
    mHelpWidget3->Layout(LAY_GrowToRight, mHelpWidget2, 0, 0, 0, 0);
    mOKButton->Resize(mX + mWidth - 160, mY + mHeight - mOKButton->mHeight + 5, 150, mOKButton->mHeight);
    mShowHelpCheckbox->Resize(mX + 10, mOKButton->mY, mShowHelpCheckbox->mWidth, mShowHelpCheckbox->mHeight);
}

void HelpScreen::Draw(Graphics *g)
{
    Widget::Draw(g);
    g->DrawImage(Sexy::IMAGE_HELP_FRONT, 0, 0);
    g->SetFont(Sexy::FONT_DIALOG);
    g->SetColor(Color(0xFFFF00));
    DrawCheckboxText(g, "Don't show this screen at startup.", mShowHelpCheckbox);
}

void HelpScreen::AddedToManager(WidgetManager *theWidgetManager)
{
    Widget::AddedToManager(theWidgetManager);
    theWidgetManager->AddWidget(mHelpBack);
    theWidgetManager->AddWidget(mHelpWidget1);
    theWidgetManager->AddWidget(mHelpWidget2);
    theWidgetManager->AddWidget(mHelpWidget3);
    theWidgetManager->AddWidget(mOKButton);
    theWidgetManager->AddWidget(mShowHelpCheckbox);
    theWidgetManager->PutBehind(mHelpWidget1, this);
    theWidgetManager->PutBehind(mHelpWidget2, mHelpWidget1);
    theWidgetManager->PutBehind(mHelpWidget3, mHelpWidget2);
    theWidgetManager->PutBehind(mHelpBack, mHelpWidget3);
}

void HelpScreen::RemovedFromManager(WidgetManager *theWidgetManager)
{
    Widget::RemovedFromManager(theWidgetManager);
    theWidgetManager->RemoveWidget(mHelpBack);
    theWidgetManager->RemoveWidget(mHelpWidget1);
    theWidgetManager->RemoveWidget(mHelpWidget2);
    theWidgetManager->RemoveWidget(mHelpWidget3);
    theWidgetManager->RemoveWidget(mOKButton);
    theWidgetManager->RemoveWidget(mShowHelpCheckbox);

    CircleShootApp *app = (CircleShootApp *)gSexyAppBase;

    bool aIsShowHelpScreen = !mShowHelpCheckbox->mChecked;
    if (aIsShowHelpScreen != app->mProfile->mShowHelpScreen)
    {
        app->mProfile->mShowHelpScreen = aIsShowHelpScreen;
        app->SaveProfile();
    }
}

void HelpScreen::ButtonDepress(int theId)
{
    GetCircleShootApp()->EndHelpScreen();
}

void HelpScreen::CheckboxChecked(int theId, bool checked)
{
}

void HelpScreen::KeyChar(char theChar)
{
    Widget::KeyChar(theChar);
}
