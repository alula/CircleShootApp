#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/ButtonListener.h>
#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/Widget.h>
#include <SexyAppFramework/ResourceManager.h>
#include <SexyAppFramework/DDImage.h>

#include "CircleShootApp.h"
#include "Res.h"
#include "CircleCommon.h"
#include "CircleButton.h"
#include "CreditsScreen.h"
#include "SpriteMgr.h"

#include <math.h>

using namespace Sexy;

CreditsScreen::CreditsScreen(bool happyEnd)
{
	mScroll = CIRCLE_WINDOW_HEIGHT;
	mSpriteMgr = new SpriteMgr();
	mSpriteMgr->SetupSpace();

	mWhateverButton = MakeButton(0, this, "Whatever", CircleButton::CB_ClickSound, NULL, 3);

	if (happyEnd)
	{
		mBackgroundImage = gSexyAppBase->GetImage("images/happyending", 0);
	}
	else
	{
		mBackgroundImage = NULL;
	}
}

CreditsScreen::~CreditsScreen()
{
	delete mSpriteMgr;
	delete mWhateverButton;
	delete mBackgroundImage;
}

struct CreditsLine
{
	Sexy::Font *mFont;
	const char *mText;
	int mSpace;
};

void CreditsScreen::Draw(Graphics *g)
{
	Widget::Draw(g);
	if (mBackgroundImage != NULL)
	{
		g->DrawImage(mBackgroundImage, 0, 0);
	}
	else
	{
		mSpriteMgr->DrawBackground(g);
	}

	static CreditsLine aCredits[] = {
		{Sexy::FONT_HUGE, "CREDITS", 50},
		{Sexy::FONT_TITLE, "Game Design", 0},
		{Sexy::FONT_BROWNTITLE, "Jason Kapalka", 35},
		{Sexy::FONT_TITLE, "Programming", 0},
		{Sexy::FONT_BROWNTITLE, "Brian \"Ace\" Rothstein", 35},
		{Sexy::FONT_TITLE, "Art", 0},
		{Sexy::FONT_BROWNTITLE, "Walter \"Straitjacket\" Wilson", 35},
		{Sexy::FONT_TITLE, "Sound FX", 0},
		{Sexy::FONT_BROWNTITLE, "Stephane Brault", 35},
		{Sexy::FONT_TITLE, "Music", 0},
		{Sexy::FONT_BROWNTITLE, "Phillipe Charon", 35},
		{Sexy::FONT_TITLE, "Level Design", 0},
		{Sexy::FONT_BROWNTITLE, "Ben Lyon", 35},
		{Sexy::FONT_TITLE, "QA", 0},
		{Sexy::FONT_BROWNTITLE, "Eric Harman", 0},
		{Sexy::FONT_BROWNTITLE, "Shawn Conard", 0},
		{Sexy::FONT_BROWNTITLE, "Brenna Flood", 35},
		{Sexy::FONT_BROWNTITLE, "Chad Zoellner", 35},
		{Sexy::FONT_TITLE, "PopCap Framework", 0},
		{Sexy::FONT_BROWNTITLE, "Brian Fiete", 35},
		{Sexy::FONT_TITLE, "Biz Dev", 0},
		{Sexy::FONT_BROWNTITLE, "Don Walters", 35},
		{Sexy::FONT_TITLE, "Special Thanks To", 0},
		{Sexy::FONT_BROWNTITLE, "Jordin Baugh", 0},
		{Sexy::FONT_BROWNTITLE, "John Vechey", 0},
		{Sexy::FONT_BROWNTITLE, "Shanon Lyon", 0},
		{Sexy::FONT_BROWNTITLE, "Sukhbir Sidhu", 0},
		{Sexy::FONT_BROWNTITLE, "Tysen \"Black Ops\" Henderson", 0},
		{Sexy::FONT_BROWNTITLE, "John Raptis", 0},
		{Sexy::FONT_BROWNTITLE, "Nick Newhard", 0},
		{Sexy::FONT_BROWNTITLE, "Architekt", 0},
		{Sexy::FONT_BROWNTITLE, "Kathy Bacon", 0},
		{Sexy::FONT_BROWNTITLE, "Ian Greenhoe", 0},
		{Sexy::FONT_BROWNTITLE, "Josh Langley", 0},
		{Sexy::FONT_BROWNTITLE, "Alison Kruse", 0},
		{Sexy::FONT_BROWNTITLE, "Dave Haas", 0},
		{Sexy::FONT_BROWNTITLE, "Juho Uuraslahti", 0},
#if 1
		{Sexy::FONT_BROWNTITLE, "Tristan Monnastes", 0},
#else
		{Sexy::FONT_BROWNTITLE, "Tristan Monnastes", 35},
		{Sexy::FONT_TITLE, "Macintosh Port", 0},
		{Sexy::FONT_BROWNTITLE, "Paul Wasmund", 0},
		{Sexy::FONT_TITLE, "Jim O'Connor", 0},
#endif
	};
	static int aNumCredits = sizeof(aCredits) / sizeof(aCredits[0]);

	g->SetColor(Color(0xFFFFFF));
	int aScroll = mScroll;

	for (int i = 0; i < aNumCredits; i++)
	{
		CreditsLine const &aLine = aCredits[i];
		if (aLine.mFont)
		{
			int aY = aScroll + aLine.mFont->GetHeight();
			if (aScroll < mHeight && aY > 0)
			{
				g->SetFont(aLine.mFont);
				g->DrawString(
					aLine.mText,
					(CIRCLE_WINDOW_WIDTH / 2) - aLine.mFont->StringWidth(aLine.mText) / 2,
					aScroll + aLine.mFont->GetAscent());
			}

			aScroll += aLine.mFont->GetHeight();
		}

		aScroll += aLine.mSpace;
	}

	if (aScroll < -100)
	{
		mScroll = CIRCLE_WINDOW_HEIGHT + 40;
	}

	if (mUpdateCnt < 25)
	{
		int aAlpha = 255 - 255 * mUpdateCnt / 25;
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		g->SetColor(Color(aAlpha, aAlpha, aAlpha));
		g->FillRect(0, 0, CIRCLE_WINDOW_WIDTH, CIRCLE_WINDOW_HEIGHT);
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	}
}

void CreditsScreen::Update()
{
	Widget::Update();
	if ((mUpdateCnt % 2) == 0)
	{
		--mScroll;
	}

	MarkDirty();
	mSpriteMgr->Update();
}

void CreditsScreen::AddedToManager(WidgetManager *theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	mWhateverButton->Resize(CIRCLE_WINDOW_WIDTH - 130, CIRCLE_WINDOW_HEIGHT - mWhateverButton->mHeight, 120, mWhateverButton->mHeight);
	theWidgetManager->AddWidget(mWhateverButton);
}

void CreditsScreen::RemovedFromManager(WidgetManager *theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mWhateverButton);
}

void CreditsScreen::ButtonDepress(int theId)
{
	if (theId == 0)
	{
		GetCircleShootApp()->ShowMainMenu();
	}
}

void CreditsScreen::KeyChar(char theChar)
{
	if (tolower(theChar) == 's')
	{
		mSpriteMgr->ToggleSpaceScroll();
	}
}