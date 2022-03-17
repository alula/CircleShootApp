#include "Zuma_Prefix.pch"

#include <SexyAppFramework/ButtonListener.h>
#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/DDImage.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/Image.h>
#include <SexyAppFramework/MemoryImage.h>
#include <SexyAppFramework/Widget.h>
#include <SexyAppFramework/WidgetManager.h>

#include "CircleButton.h"
#include "CircleCommon.h"
#include "CircleShootApp.h"
#include "LevelParser.h"
#include "HighScoreMgr.h"
#include "ProfileMgr.h"
#include "PracticeScreen.h"
#include "Res.h"

#include <cmath>

using namespace Sexy;

bool gUnlocked = false;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

PracticeScreen::PracticeScreen()
{
	mNextButton = Sexy::MakeButton(0, this, "", 1, Sexy::IMAGE_GAUNTNEXTBUTTON, 4);
	mBackButton = Sexy::MakeButton(1, this, "", 2, Sexy::IMAGE_GAUNTBACKBUTTON, 4);
	mGauntPlayButton = Sexy::MakeButton(2, this, "", 3, Sexy::IMAGE_GAUNTPLAYBUTTON, 4);
	mMainMenuButton = Sexy::MakeButton(3, this, "", 4, Sexy::IMAGE_GAUNTMAINMENUBUTTON, 3);
	mPracticeButton = Sexy::MakeButton(5, this, "", 5, Sexy::IMAGE_GAUNTPRACTICEBUTTON, 3);
	mSurvivalButton = Sexy::MakeButton(6, this, "", 6, Sexy::IMAGE_GAUNTSURVIVALBUTTON, 3);

	mThumbnail = NULL;
	mScoreSet = new std::multiset<HighScore>();
	mGradientImageLocked = CreateGradientImage("LoCKED", 0x85);
	mGradientImageQuestionMark = CreateGradientImage("?", 0xFF);

	mCurrentBoard = ((CircleShootApp *)gSexyAppBase)->mProfile->mLastPracticeBoard;
	mIsEndless = ((CircleShootApp *)gSexyAppBase)->mProfile->mPracticeEndless;
	mDifficulty = ((CircleShootApp *)gSexyAppBase)->mProfile->mPracticeDifficulty;
	mCurrentDoor = 0;
	mHighlightDoor = -1;
	mGem = 0;
	mLastClickCnt = 0;

	IntPoint aDoors[12] = {
		{209, 179},
		{232, 129},
		{249, 82},
		{265, 28},
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
	};
	for (int i = 0; i < 4; i++)
	{
		mDoors[i].mImage = (MemoryImage *)Sexy::GetImageById((ResourceId)(i + Sexy::IMAGE_GAUNTDOOR1_ID));
		int aWidth = mDoors[i].mImage->mWidth;
		if (aWidth < -0)
		{
			aWidth += 3;
		}
		mDoors[i].mRect.mHeight = mDoors[i].mImage->mHeight;
		mDoors[i].mRect.mWidth = aWidth >> 2;
		mDoors[i].mRect.mY = aDoors[i].mY;
		mDoors[i].mRect.mX = aDoors[i].mX;
	}

	mLocked = false;
	mGauntletBlinkCount = 0;
	SyncModeButtons();
}

PracticeScreen::~PracticeScreen() {}

void PracticeScreen::Update()
{
	Widget::Update();

	if (mGauntletBlinkCount != 0)
	{
		mGauntletBlinkCount--;
	}

	if ((mUpdateCnt & 3) == 0)
	{
		MarkDirty();
	}
}

void PracticeScreen::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Widget::Resize(theX, theY, theWidth, theHeight);

	mBackButton->Resize(theX + 177, theY + 444, mBackButton->mWidth, mBackButton->mHeight);
	mGauntPlayButton->Resize(theX + 277, theY + 444, mGauntPlayButton->mWidth, mGauntPlayButton->mHeight);
	mNextButton->Resize(theX + 390, theY + 444, mNextButton->mWidth, mNextButton->mHeight);
	mMainMenuButton->Resize(theX + 10, theY + 422, mMainMenuButton->mWidth, mMainMenuButton->mHeight);
	mSurvivalButton->Resize(theX + 506, theY + 413, mSurvivalButton->mWidth, mSurvivalButton->mHeight);
	mPracticeButton->Resize(theX + 505, theY + 446, mPracticeButton->mWidth, mPracticeButton->mHeight);
}

void PracticeScreen::AddedToManager(WidgetManager *theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);

	LoadBoard();

	theWidgetManager->AddWidget(mBackButton);
	theWidgetManager->AddWidget(mNextButton);
	theWidgetManager->AddWidget(mGauntPlayButton);
	theWidgetManager->AddWidget(mMainMenuButton);
	theWidgetManager->AddWidget(mSurvivalButton);
	theWidgetManager->AddWidget(mPracticeButton);
}

void PracticeScreen::RemovedFromManager(WidgetManager *theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);

	theWidgetManager->RemoveWidget(mBackButton);
	theWidgetManager->RemoveWidget(mNextButton);
	theWidgetManager->RemoveWidget(mGauntPlayButton);
	theWidgetManager->RemoveWidget(mMainMenuButton);
	theWidgetManager->RemoveWidget(mSurvivalButton);
	theWidgetManager->RemoveWidget(mPracticeButton);

	((CircleShootApp *)gSexyAppBase)->mProfile->mLastPracticeBoard = mCurrentBoard;
	((CircleShootApp *)gSexyAppBase)->mProfile->mPracticeEndless = mIsEndless;
	((CircleShootApp *)gSexyAppBase)->mProfile->mPracticeDifficulty = mDifficulty;

	((CircleShootApp *)gSexyAppBase)->SaveProfile();
}

void PracticeScreen::ButtonDepress(int theId)
{
	switch (theId)
	{
	case 0:
		mCurrentBoard++;
		LoadBoard();
		break;
	case 1:
		mCurrentBoard--;
		LoadBoard();
		break;
	case 2:
		if (mCurrentDoor >= 0)
		{
			((CircleShootApp *)gSexyAppBase)->StartPracticeGame(mStage, 7 * mCurrentDoor, mIsEndless);
		}
		break;
	case 3:
		((CircleShootApp *)gSexyAppBase)->ShowMainMenu();
		break;
	case 4:
		((CircleShootApp *)gSexyAppBase)->DoOptionsDialog();
		break;
	case 5:
		mIsEndless = false;
		SyncModeButtons();
		GetHighscores();
		break;
	case 6:
		mIsEndless = true;
		SyncModeButtons();
		GetHighscores();
		break;
	default:
		break;
	}
}

void PracticeScreen::Draw(Graphics *g)
{
	Widget::Draw(g);

	int aScroll = (mUpdateCnt >> 2) % Sexy::IMAGE_GAUNTSKY->mWidth;
	g->DrawImage(Sexy::IMAGE_GAUNTSKY, aScroll - Sexy::IMAGE_GAUNTSKY->mWidth, 0);
	g->DrawImage(Sexy::IMAGE_GAUNTSKY, aScroll, 0);
	g->DrawImage(Sexy::IMAGE_GAUNTSKY, aScroll + Sexy::IMAGE_GAUNTSKY->mWidth, 0);

	g->DrawImage(mThumbnail, 170, 220);
	g->DrawImage(Sexy::IMAGE_GAUNTSCREEN, 0, 0);
	g->DrawImage(Sexy::IMAGE_GAUNTTITLE, 0, 0);
	g->DrawImage(Sexy::IMAGE_GAUNTHIGHSCORE, 434, 0);

	for (int i = 0; i < 4; i++)
	{
		int aFrame = 1;

		if (i == mCurrentDoor)
		{
			aFrame = 3;
		}
		if (i == mHighlightDoor)
		{
			aFrame = 2;
		}
		if (i == mGem)
		{
			if (((mGauntletBlinkCount >> 4) & 1) != 0)
			{
				aFrame = 2;
			}
		}
		else if (i > mGem)
		{
			aFrame = 0;
		}

		g->DrawImageCel(mDoors[i].mImage, mDoors[i].mRect.mX, mDoors[i].mRect.mY, aFrame);
	}

	g->SetColor(Color(0xFFFFFF));
	g->SetFont(Sexy::FONT_MAIN10OUTLINE);

	std::string aText(mDisplayName);
	aText[0] = toupper(aText[0]);
	g->DrawString(aText,
				  170 + (mThumbnail->mWidth - Sexy::FONT_MAIN10OUTLINE->StringWidth(aText)) / 2,
				  427);

	g->SetColor(Color(0xFFFF00));
	aText = mIsEndless ? "Survival" : "Practice";
	g->DrawString(aText,
				  150 + (mThumbnail->mWidth - Sexy::FONT_MAIN10OUTLINE->StringWidth(aText)),
				  260);

	if (mGem > 2)
	{
		int aPosX = 560 - Sexy::IMAGE_SUNGLOW->mWidth / 2;
		int aPosY = 220 - (Sexy::IMAGE_SUNGLOW->mHeight + Sexy::IMAGE_GAUNTSUNGEM->mHeight) / 2;

		g->SetColorizeImages(true);
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);

		int aGlow = mUpdateCnt % 500;
		if (aGlow > 250)
		{
			aGlow = 500 - aGlow;
		}

		g->SetColor(Color(255, 255, 0, 75 * aGlow / 250 + 75));

		if (gSexyAppBase->Is3DAccelerated())
		{
			float aRotation = (mUpdateCnt * M_PI / 180.0f) * 0.3f;
			g->DrawImageRotated(Sexy::IMAGE_SUNGLOW, aPosX, aPosY, aRotation);
		}
		else
		{
			g->DrawImage(Sexy::IMAGE_SUNGLOW, aPosX, aPosY);
		}

		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
		g->SetColorizeImages(false);
	}

	switch (mGem)
	{
	case 1:
		g->DrawImage(Sexy::IMAGE_GAUNTEAGLEGEM, 560 - Sexy::IMAGE_GAUNTEAGLEGEM->mWidth / 2, 220 - Sexy::IMAGE_GAUNTEAGLEGEM->mHeight);
		break;
	case 2:
		g->DrawImage(Sexy::IMAGE_GAUNTJAGUARGEM, 560 - Sexy::IMAGE_GAUNTJAGUARGEM->mWidth / 2, 220 - Sexy::IMAGE_GAUNTJAGUARGEM->mHeight);
		break;
	case 3:
		g->DrawImage(Sexy::IMAGE_GAUNTSUNGEM, 560 - Sexy::IMAGE_GAUNTSUNGEM->mWidth / 2, 220 - Sexy::IMAGE_GAUNTSUNGEM->mHeight);
		break;
	default:
		break;
	}

	if (mScoreSet != NULL)
	{
		Graphics sg(*g);
		sg.SetColor(Color(0x5B010A));
		sg.SetFont(Sexy::FONT_MAIN10OUTLINE);

		HighScoreSet::iterator anItr = mScoreSet->begin();
		for (int i = 0; i != 3 && anItr != mScoreSet->end(); anItr++, i++)
		{
			g->DrawString(
				Sexy::StrFormat("%d", anItr->mScore),
				575,
				70 + Sexy::FONT_MAIN10OUTLINE->GetLineSpacing() * i);
		}

		sg.ClipRect(0, 0, 567, 480);

		anItr = mScoreSet->begin();
		for (int i = 0; i != 3 && anItr != mScoreSet->end(); anItr++, i++)
		{
			g->DrawString(
				anItr->mName,
				445,
				70 + Sexy::FONT_MAIN10OUTLINE->GetLineSpacing() * i);
		}
	}

	g->SetColor(Color(0x5B010A));
	g->SetFont(Sexy::FONT_MAIN8OUTLINE);

	const char *aDescription;
	if (mLocked)
	{
		aDescription = "YOU MUST FIRST UNLOCK THIS LEVEL IN ADVENTURE MODE.";
	}
	else if (mCurrentBoard != 0)
	{
		aDescription = "HOW LONG CAN YOU SURVIVE AN ENDLESS STREAM OF ZUMAIC INSANITY?";
		if (!this->mIsEndless)
			aDescription = "PRACTICE A SINGLE BOARD UNTIL MASTERED.  CAN YOU BECOME A SUN GOD?";
	}
	else
	{
		aDescription = "PLAY A RANDOM LEVEL THAT YOU HAVE UNLOCKED IN ADVENTURE MODE.";
		if (!this->mIsEndless)
			aDescription = "PLAY THROUGH ALL THE LEVELS YOU HAVE UNLOCKED IN ADVENTURE MODE IN RANDOM ORDER!";
	}

	Widget::WriteWordWrapped(g, Rect(20, 60, 150, 100), aDescription, 14, -1);
}

void PracticeScreen::KeyChar(char theChar)
{
	Widget::KeyChar(theChar);
}

void PracticeScreen::MouseLeave()
{
	Widget::MouseLeave();
	gSexyAppBase->SetCursor(0);
}

void PracticeScreen::MouseMove(int x, int y)
{
	Widget::MouseMove(x, y);

	int aDoor = GetDoorAt(x, y);
	if (aDoor != mHighlightDoor)
	{
		gSexyAppBase->SetCursor(aDoor >= 0 ? CURSOR_HAND : CURSOR_POINTER);
		mHighlightDoor = aDoor;
		MarkDirty();
	}
}

void PracticeScreen::MouseDown(int x, int y, int theButton)
{
	Widget::MouseDown(x, y, theButton);

	int aDoor = GetDoorAt(x, y);
	if (aDoor >= 0)
	{
		if (aDoor == mCurrentDoor)
		{
			ButtonDepress(2);
		}
		else
		{
			mCurrentDoor = aDoor;
			mDifficulty = aDoor;
			MarkDirty();
		}
	}

	mLastClickCnt = mUpdateCnt;
}

void PracticeScreen::MouseUp(int x, int y)
{
	Widget::MouseUp(x, y);
}

void PracticeScreen::SyncModeButtons() {
	CircleButton* aButton1 = mSurvivalButton;
	CircleButton* aButton2 = mPracticeButton;

	if (!mIsEndless) {
		aButton1 = mPracticeButton;
		aButton2 = mSurvivalButton;
	}

	aButton1->mInverted = true;
	aButton1->SetDisabled(true);

	aButton2->mInverted = false;
	aButton2->SetDisabled(false);
}

MemoryImage *PracticeScreen::CreateGradientImage(std::string const &theName, int theAlpha)
{
	MemoryImage *anImage = new MemoryImage(gSexyAppBase);
	anImage->Create(320, 240);

	Graphics g(anImage);
	int aHalfHeight = anImage->mHeight / 2;
	for (int i = 0; i < anImage->mHeight; i++)
	{
		int v5 = aHalfHeight - i;
		if (v5 < 0)
			v5 = -v5;

		int aRed = 255 - 255 * v5 / aHalfHeight;
		if (aRed >= 0)
		{
			if (255 - 255 * v5 / aHalfHeight >= 256)
			{
				aRed = 255;
			}
		}
		else
		{
			aRed = 0;
		}

		g.SetColor(Color(aRed, 0, 0, theAlpha));
		g.FillRect(0, i, anImage->mWidth, 1);
	}

	g.SetFont(Sexy::FONT_HUGE);
	g.SetColor(Color(0xFFFFFF));
	g.DrawString(
		theName,
		(anImage->mWidth - Sexy::FONT_HUGE->StringWidth(theName)) / 2,
		anImage->mHeight / 2 + 25);

	return anImage;
}

int PracticeScreen::GetDoorAt(int theX, int theY)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		Door &aDoor = mDoors[i];
		if (mGem >= i && theX >= aDoor.mRect.mX &&
			theX < aDoor.mRect.mX + aDoor.mRect.mWidth &&
			theY >= aDoor.mRect.mY &&
			theY < aDoor.mRect.mY + aDoor.mRect.mHeight)
		{
			int aPosX = theX - aDoor.mRect.mX;
			int aPosY = theY - aDoor.mRect.mY;

			ulong *aBits = aDoor.mImage->GetBits();
			if ((aBits[aPosX + aPosY * aDoor.mImage->GetWidth()] & 0xff000000) != 0)
				break;
		}
	}

	if (i == 4)
		return -1;

	return i;
}

DDImage *PracticeScreen::GetThumbnail(std::string const &theName)
{
	// TODO:
	// there's also a base folder argument passed into arg 2 of GetImage in some versions
	// the SexyApp version we're using doesn't support that and we don't support custom save folders yet.
	DDImage *anImage = gSexyAppBase->GetImage("levels/cached_thumbnails/" + theName);
	if (anImage != NULL)
	{
		return anImage;
	}

	anImage = gSexyAppBase->GetImage("levels/perm_thumbnails/" + theName);

	return anImage;
}

void PracticeScreen::GetHighscores()
{
}

void PracticeScreen::LoadBoard()
{
	delete mThumbnail;
	mThumbnail = NULL;

	UserProfile *aProfile = ((CircleShootApp *)gSexyAppBase)->mProfile;
	if (aProfile->mNeedGauntletBlink)
	{
		aProfile->mNeedGauntletBlink = false;
		mGauntletBlinkCount = 160;
	}
	else
	{
		mGauntletBlinkCount = 0;
	}

	LevelParser *aLevelParser = ((CircleShootApp *)gSexyAppBase)->mLevelParser;
	int aBoards = aLevelParser->mBoardProgression.size();
	int aMaxBoards = aBoards + 1;
	if (((CircleShootApp *)gSexyAppBase)->mProfile->mMaxStage < 12)
	{
		aMaxBoards = aBoards;
	}

	if (aMaxBoards <= mCurrentBoard)
	{
		mCurrentBoard = 0;
	}

	if (mCurrentBoard < 0)
	{
		mCurrentBoard = aMaxBoards - 1;
	}

	aLevelParser->GetPracticeBoard(mCurrentBoard, mStage, mDisplayName);

	UserProfile::BoardMap::iterator anItr = aProfile->mMaxBoardLevel.find(mStage);
	bool aUnlocked;

	if (anItr == aProfile->mMaxBoardLevel.end())
	{
		mGem = -1;
		aUnlocked = false;
	}
	else
	{
		mGem = anItr->second / 7;
		if (mGem > 3)

			mGem = 3;

		aUnlocked = true;
	}

	mCurrentDoor = mDifficulty >= 0 ? mDifficulty : 0;
	if (mCurrentDoor > mGem)
	{
		mCurrentDoor = mGem;
	}

	mHighlightDoor = -1;

	if (gUnlocked)
	{
		mGem = 3;
		aUnlocked = true;
	}

	mGauntPlayButton->SetVisible(aUnlocked);

	mThumbnail = GetThumbnail(mStage);
	if (mThumbnail == NULL)
	{
		mThumbnail = new MemoryImage(gSexyAppBase);
		mThumbnail->Create(320, 240);
		Graphics g(mThumbnail);
		g.SetColor(Color(0));
		g.FillRect(0, 0, 320, 240);
	}

	mLocked = !aUnlocked;

	Graphics g(mThumbnail);
	if (mLocked)
	{
		g.DrawImage(mGradientImageLocked, 0, 0);
	}
	else if (mCurrentBoard == 0)
	{
		g.DrawImage(mGradientImageQuestionMark, 0, 0);
	}

	GetHighscores();
	MarkDirty();
}