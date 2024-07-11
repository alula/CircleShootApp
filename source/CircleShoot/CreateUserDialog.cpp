#include "Zuma_Prefix.pch"

#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/Graphics.h>
#include <SexyAppFramework/EditWidget.h>

#include "CircleButton.h"
#include "CircleCommon.h"
#include "CircleDialog.h"
#include "CircleShootApp.h"
#include "CreateUserDialog.h"
#include "ProfileMgr.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CreateUserDialog::CreateUserDialog(bool rename) : CircleDialog(IMAGE_DIALOG_BACK, IMAGE_DIALOG_BUTTON,
															   rename ? DialogType_RenameUser : DialogType_CreateUser, true,
															   "NEW USER", "Please enter your name.", "",
															   Dialog::BUTTONS_OK_CANCEL, 0)
{
	mNameInput = new EditWidget(0, this);
	mNameInput->SetFont(FONT_DIALOG);
	mNameInput->mMaxChars = 12;
	mNameInput->mMaxPixels = 200;
	Sexy::SetupEditWidget(mNameInput);
}

CreateUserDialog::~CreateUserDialog()
{
	delete mNameInput;
}

void CreateUserDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	CircleDialog::Resize(theX, theY, theWidth, theHeight);

	mNameInput->Resize(GetLeft() + 10, GetTop() + 20, GetWidth() - 36, 24);
}

int CreateUserDialog::GetPreferredHeight(int theWidth)
{
	return CircleDialog::GetPreferredHeight(theWidth) + 40;
}

void CreateUserDialog::AddedToManager(WidgetManager *theWidgetManager)
{
	CircleDialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mNameInput);
	theWidgetManager->SetFocus(mNameInput);
}

void CreateUserDialog::RemovedFromManager(WidgetManager *theWidgetManager)
{
	CircleDialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mNameInput);
}

void CreateUserDialog::EditWidgetText(int theId, const std::string &theString)
{
	ButtonDepress(mId + 2000);
}

bool CreateUserDialog::AllowChar(int theId, char theChar)
{
	gLastTypeFrame = mUpdateCnt;

	return isalnum(theChar) || theChar == ' ';
}

void CreateUserDialog::SetName(const std::string &theName)
{
	mNameInput->SetText(theName);
	mNameInput->mCursorPos = theName.length();
	mNameInput->mHilitePos = 0;
}

std::string CreateUserDialog::GetName()
{
	std::string aName;

	char pc = ' ';
	char c;
	for (int i = 0; i < (int)mNameInput->mString.length(); i++, pc = c)
	{
		c = mNameInput->mString[i];
		if (c == ' ')
		{
			if (pc == ' ')
				continue;
			c = ' ';
		}
		aName.push_back(c);
	}

	if (aName.empty())
		return aName;

	if (aName[aName.size() - 1] == ' ')
		aName.resize(aName.size() - 1);

	return aName;
}
