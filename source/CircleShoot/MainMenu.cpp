#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/Image.h>
#include <SexyAppFramework/Widget.h>
#include <SexyAppFramework/WidgetManager.h>

#include "CircleShootApp.h"
#include "CircleButton.h"
#include "ProfileMgr.h"
#include "Res.h"
#include "MainMenu.h"

#include <cmath>

using namespace Sexy;

class MainMenuOverlay : public Widget
{
public:
    MainMenuOverlay(MainMenu *theMainMenu) : Widget()
    {
        mMenu = theMainMenu;
        mMouseVisible = false;
        // mGap71 = true;
        // mGap72 = true;
        Resize(0, 0, 640, 480);
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
    unkA4 = 331;
    unkA0 = 191;
    // unk78 = 0;
    // unkC4 = 0;
    mMainMenuOverlay = new MainMenuOverlay(this);
    mArcadeButton = MakeButton(0, this, "", 3, Sexy::IMAGE_MM_ARCADE, 3);
    mGauntletButton = MakeButton(1, this, "", 3, Sexy::IMAGE_MM_GAUNTLET, 3);
    mOptionsButton = MakeButton(2, this, "", 3, Sexy::IMAGE_MM_OPTIONS, 3);
    mMoreGamesButton = MakeButton(3, this, "", 3, Sexy::IMAGE_MM_MOREGAMES, 3);
    mQuitButton = MakeButton(4, this, "", 3, Sexy::IMAGE_MM_QUIT, 3);
}

MainMenu::~MainMenu() {}

void MainMenu::KeyChar(char theChar) {}

void MainMenu::DrawOverlay(Graphics *g) {}

void MainMenu::OrderInManagerChanged()
{
    Widget::OrderInManagerChanged();
}

void MainMenu::RemovedFromManager(WidgetManager *theWidgetManager)
{
    Widget::RemovedFromManager(theWidgetManager);

    theWidgetManager->RemoveWidget(mArcadeButton);
    theWidgetManager->RemoveWidget(mGauntletButton);
    theWidgetManager->RemoveWidget(mMoreGamesButton);
    theWidgetManager->RemoveWidget(mOptionsButton);
    theWidgetManager->RemoveWidget(mQuitButton);
    theWidgetManager->RemoveWidget(mMainMenuOverlay);
}

void MainMenu::ButtonDepress(int theId)
{
    CircleShootApp *app = ((CircleShootApp *)gSexyAppBase);

    switch (theId)
    {
    case 0:
        app->StartAdventureGame(0);
        break;
    case 1:
        app->ShowPracticeScreeen(true);
        break;
    case 2:
        app->DoOptionsDialog();
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
    theWidgetManager->AddWidget(mMainMenuOverlay);
}

void MainMenu::CalcEyePos() {}

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
}

void MainMenu::SyncProfile() {}

void MainMenu::AddUFOMove(IntPoint const &unk1, IntPoint const &unk2, int unk3, int unk4) {}

void MainMenu::DoUFO() {}

void MainMenu::UpdateUFOMove() {}

void MainMenu::UpdateUFOScale() {}

void MainMenu::Update()
{
    Widget::Update();
    CalcEyePos();

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
                 (mEyeLeftX - (Sexy::IMAGE_MM_EYELEFT->mWidth / 2)),
                 (mEyeLeftY - (Sexy::IMAGE_MM_EYELEFT->mHeight / 2)));
    g->DrawImage(Sexy::IMAGE_MM_EYERIGHT,
                 (mEyeRightX - (Sexy::IMAGE_MM_EYERIGHT->mWidth / 2)),
                 (mEyeRightY - (Sexy::IMAGE_MM_EYERIGHT->mHeight / 2)));

    CircleShootApp *app = ((CircleShootApp *)gSexyAppBase);

    g->SetColorizeImages(true);
    g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);

    if (app->Is3DAccelerated())
    {
        double angle = (mUpdateCnt * M_PI / 180.0) * 0.3;
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
        welcome += ' ';
        welcome += app->mProfile->mName;
    }
    else
    {
        welcome = "Welcome to Zuma!";
    }

    int textWidth = Sexy::FONT_BROWNTITLE->StringWidth(welcome);
    g->DrawString(welcome, (640 - textWidth) / 2, 32);
}