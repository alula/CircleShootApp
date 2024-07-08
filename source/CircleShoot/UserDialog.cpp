#include "Zuma_Prefix.pch"

#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/Graphics.h>
#include <SexyAppFramework/ListWidget.h>
#include <SexyAppFramework/ScrollbarWidget.h>

#include "CircleButton.h"
#include "CircleCommon.h"
#include "CircleDialog.h"
#include "CircleShootApp.h"
#include "UserDialog.h"
#include "ProfileMgr.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

UserDialog::UserDialog() : CircleDialog(Sexy::IMAGE_DIALOG_BACK, Sexy::IMAGE_DIALOG_BUTTON, DialogType_User, true,
										"Who are you?", "", "",
										Dialog::BUTTONS_OK_CANCEL, false)
{
	mUserList = new ListWidget(0, Sexy::FONT_DIALOG, this);
	mScrollbar = new ScrollbarWidget(0, mUserList);
	mRenameButton = MakeButton(0, this, "Rename", 1, 0, 3);
	mDeleteButton = MakeButton(1, this, "Delete", 1, 0, 3);

	static int aUserListColors[5][3] = {
		{41, 73, 24},
		{0, 0, 0},
		{206, 227, 33},
		{255, 255, 255},
		{173, 40, 198}};

	mUserList->mScrollbar = mScrollbar;
	mUserList->mJustify = ListWidget::JUSTIFY_CENTER;
	mUserList->mItemHeight = 15;
	mUserList->SetColors(aUserListColors, 5);
	mUserList->mDrawOutline = true;

	int aNewUserLine = mUserList->AddLine("(Create a New User)", false);

	UserProfile *aProfile = GetCircleShootApp()->mProfile;

	if (aProfile)
	{
		int aLine = mUserList->AddLine(aProfile->mName, false);
		mUserList->SetSelect(aLine);
	}

	for (ProfileMap::const_iterator aProfileItr = GetCircleShootApp()->mProfileMgr->GetProfileMap().begin();
		 aProfileItr != GetCircleShootApp()->mProfileMgr->GetProfileMap().end();
		 aProfileItr++)
	{
		if (aProfile && aProfile->mName == aProfileItr->second.mName)
			continue;

		mUserList->AddLine(aProfileItr->second.mName, false);
	}

	if (mUserList->mLines.size() <= 6)
	{
		mScrollbar->SetVisible(false);
	}
}

UserDialog::~UserDialog()
{
	delete mUserList;
	delete mScrollbar;
	delete mRenameButton;
	delete mDeleteButton;
}

void UserDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	CircleDialog::Resize(theX, theY, theWidth, theHeight);

	int aLeft = GetLeft() + 10;
	int aTop = GetTop();
	int aRight = GetWidth() - 20;
	int aScrollbarPad = mScrollbar->mVisible ? 16 : 0;

	mUserList->Resize(aLeft, aTop, aRight - aScrollbarPad, 125);
	mScrollbar->ResizeScrollbar(aRight + aLeft - aScrollbarPad, aTop, aScrollbarPad, 125);

	mRenameButton->Layout(LAY_SameLeft | LAY_Above | LAY_SameSize, mYesButton, 0, 0, 0, 0);
	mDeleteButton->Layout(LAY_SameLeft | LAY_Above | LAY_SameSize, mNoButton, 0, 0, 0, 0);
}

int UserDialog::GetPreferredHeight(int theWidth)
{
	return CircleDialog::GetPreferredHeight(theWidth) + 190;
}

void UserDialog::AddedToManager(WidgetManager *theWidgetManager)
{
	CircleDialog::AddedToManager(theWidgetManager);

	theWidgetManager->AddWidget(mUserList);
	theWidgetManager->AddWidget(mScrollbar);
	theWidgetManager->AddWidget(mDeleteButton);
	theWidgetManager->AddWidget(mRenameButton);
}

void UserDialog::RemovedFromManager(WidgetManager *theWidgetManager)
{
	CircleDialog::RemovedFromManager(theWidgetManager);

	theWidgetManager->RemoveWidget(mUserList);
	theWidgetManager->RemoveWidget(mScrollbar);
	theWidgetManager->RemoveWidget(mDeleteButton);
	theWidgetManager->RemoveWidget(mRenameButton);
}

std::string UserDialog::GetSelName()
{
	if (mUserList->mSelectIdx > 0 && mUserList->mSelectIdx < mUserList->GetLineCount())
	{
		return mUserList->GetStringAt(mUserList->mSelectIdx);
	}

	return "";
}

void UserDialog::FinishDeleteUser()
{
	int aSelectIdx = mUserList->mSelectIdx;
	mUserList->RemoveLine(aSelectIdx);

	aSelectIdx--;

	if (aSelectIdx <= 0)
		aSelectIdx = 1;

	if (mUserList->GetLineCount() > 1)
		mUserList->SetSelect(aSelectIdx);
}

void UserDialog::FinishRenameUser(const std::string &theNewName)
{
	int aSelectIdx = mUserList->mSelectIdx;
	if (aSelectIdx > 0)
	{
		mUserList->SetLine(aSelectIdx, theNewName);
	}
}

void UserDialog::ListClicked(int theId, int theIdx, int theClickCount)
{
	if (theIdx == 0)
	{
		GetCircleShootApp()->DoCreateUserDialog();
	}
	else
	{
		mUserList->SetSelect(theIdx);

		if (theClickCount == 2)
		{
			GetCircleShootApp()->FinishUserDialog(true);
		}
	}
}

void UserDialog::ButtonDepress(int theId)
{
	CircleDialog::ButtonDepress(theId);

	std::string aName = GetSelName();
	if (aName.empty())
		return;

	if (theId == 0)
	{
		GetCircleShootApp()->DoRenameUserDialog(aName);
	}
	else if (theId == 1)
	{
		GetCircleShootApp()->DoConfirmDeleteUserDialog(aName);
	}
}
