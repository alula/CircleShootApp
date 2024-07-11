#include "Zuma_Prefix.pch"

#include "CircleCheckbox.h"
#include "CircleCommon.h"

#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/MemoryImage.h>
#include <SexyAppFramework/SoundManager.h>
#include <SexyAppFramework/SoundInstance.h>

using namespace Sexy;

CircleCheckbox::CircleCheckbox(Image *theUncheckedImage, Image *theCheckedImage, int theId, CheckboxListener *theCheckboxListener)
	: Checkbox(theUncheckedImage, theCheckedImage, theId, theCheckboxListener), mClickSound(-1)
{
}

CircleCheckbox::~CircleCheckbox()
{
}

void CircleCheckbox::MouseDown(int x, int y, int theBtnNum, int theClickCount)
{
	Checkbox::MouseDown(x, y, theBtnNum, theClickCount);
	if (mClickSound >= 0)
	{
		gSexyAppBase->PlaySample(mClickSound);
	}
}