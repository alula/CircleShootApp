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
#include "LoadingScreen.h"

#include <math.h>

using namespace Sexy;

LoadingScreen::LoadingScreen()
{
    mLoadingProgress = 0.0;
    unk7 = false;
    unk5 = false;
    unk8 = false;

    mClickToPlayLink = new HyperlinkWidget(0, this);
    mClickToPlayLink->mLabel = "Click Here To Play!";
    mClickToPlayLink->SetFont(Sexy::FONT_FLOAT);
    mClickToPlayLink->mColor = Color(0xffec91);
    mClickToPlayLink->mOverColor = Color(0xffffff);
    mClickToPlayLink->mUnderlineSize = 2;
    mClickToPlayLink->SetVisible(false);

    mRegisterLink = new HyperlinkWidget(1, this);
    mRegisterLink->mLabel = "Click Here To Register!";
    mRegisterLink->SetFont(Sexy::FONT_FLOAT);
    mRegisterLink->mColor = Color(0xffec91);
    mRegisterLink->mOverColor = Color(0xffffff);
    mRegisterLink->mUnderlineSize = 2;
    mRegisterLink->SetVisible(false);

    mClickToPlayLink->Resize(0,
                             0,
                             Sexy::FONT_FLOAT->StringWidth(mClickToPlayLink->mLabel) + 20,
                             Sexy::FONT_FLOAT->GetHeight() + 20);

    mRegisterLink->Resize(0,
                          0,
                          Sexy::FONT_FLOAT->StringWidth(mRegisterLink->mLabel) + 20,
                          Sexy::FONT_FLOAT->GetHeight() + 20);
}

LoadingScreen::~LoadingScreen()
{
    delete mClickToPlayLink;
    delete mRegisterLink;

    gSexyAppBase->mResourceManager->ReplaceImage("IMAGE_LOADING_SCREEN", NULL);
    gSexyAppBase->mResourceManager->ReplaceImage("IMAGE_LOADERBAR", NULL);
    gSexyAppBase->mResourceManager->ReplaceImage("IMAGE_LOADERBARBW", NULL);
}

void LoadingScreen::Resize(int theX, int theY, int theWidth, int theHeight)
{
    Widget::Resize(theX, theY, theWidth, theHeight);

    mClickToPlayLink->Resize(
        theX + (mWidth - mClickToPlayLink->mWidth) / 2,
        430,
        mClickToPlayLink->mWidth,
        mClickToPlayLink->mHeight);

    mRegisterLink->Resize(
        theX + (mWidth - mRegisterLink->mWidth) / 2,
        280,
        mRegisterLink->mWidth,
        mRegisterLink->mHeight);
}

void LoadingScreen::Draw(Graphics *g)
{
    g->DrawImage(Sexy::IMAGE_LOADING_SCREEN, 0, 0);

    int barWidth = Sexy::IMAGE_LOADERBAR->mWidth * mLoadingProgress;

    Rect srcRect(0, 0, barWidth, Sexy::IMAGE_LOADERBAR->mHeight);

    g->DrawImage(Sexy::IMAGE_LOADERBAR, 128, 349, srcRect);
}

void LoadingScreen::Update()
{
    Widget::Update();

    double progress = gSexyAppBase->GetLoadingThreadProgress();
    if (progress > mLoadingProgress)
    {
        mLoadingProgress = min(progress, mLoadingProgress + 0.01);
        MarkDirty();

        if (mLoadingProgress >= 1.0f)
        {
            mClickToPlayLink->SetVisible(true);
        }
    }

    // todo register dialog
}

void LoadingScreen::AddedToManager(WidgetManager *theWidgetManager)
{
    Widget::AddedToManager(theWidgetManager);

    theWidgetManager->AddWidget(mClickToPlayLink);
    theWidgetManager->AddWidget(mRegisterLink);
}

void LoadingScreen::RemovedFromManager(WidgetManager *theWidgetManager)
{
    Widget::RemovedFromManager(theWidgetManager);

    theWidgetManager->RemoveWidget(mRegisterLink);
    theWidgetManager->RemoveWidget(mClickToPlayLink);
}

void LoadingScreen::SyncRegistrationStuff()
{
    CircleShootApp *app = ((CircleShootApp *)gSexyAppBase);
    // todo: those apparently belong to SexyAppBase but idc about that atm
    unsigned int unknown1 = 0;
    unsigned int unknown2 = 0;

    if (app->mMaxTime > 0)
    {
    }

    if (unk7)
    {
        unk8 = true;
        mClickToPlayLink->SetVisible(false);
    }
}

void LoadingScreen::ButtonDepress(int theId)
{
    if (theId != 0)
    {
        if (theId == 1)
            unk5 = true;
    }
    else
    {
        CircleShootApp *app = ((CircleShootApp *)gSexyAppBase);
        app->ShowMainMenu();
    }
}

void LoadingScreen::SetRegistered()
{
    unk7 = false;
    mRegisterLink->SetVisible(false);
    mClickToPlayLink->SetVisible(mLoadingProgress >= 1.0);
}