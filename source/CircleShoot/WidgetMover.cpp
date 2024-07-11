#include "Zuma_Prefix.pch"

#include "WidgetMover.h"
#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/SexyAppBase.h>

using namespace Sexy;

WidgetMover::WidgetMover() {}

WidgetMover::~WidgetMover() {}

void WidgetMover::Update()
{
	for (WidgetMoverMap::iterator anItr = mWidgetMap.begin(); anItr != mWidgetMap.end();)
    {
        Sexy::Widget *theWidget = anItr->first;
        MoveInfo &moveInfo = anItr->second;

        if (++moveInfo.counter <= 20)
        {
            theWidget->Move(
                ((moveInfo.targetX - moveInfo.x) * moveInfo.counter + 20 * moveInfo.x) / 20,
                ((moveInfo.targetY - moveInfo.y) * moveInfo.counter + 20 * moveInfo.y) / 20);
            anItr++;
        }
        else
        {
            if (moveInfo.delayDelete)
            {
                if (theWidget->mWidgetManager != NULL)
                {
                    theWidget->mWidgetManager->RemoveWidget(theWidget);
                    gSexyAppBase->SafeDeleteWidget(theWidget);
                }
            }
            else
            {
                theWidget->Move(moveInfo.targetX, moveInfo.targetY);
            }

            mWidgetMap.erase(anItr++);
        }
    }
}

void WidgetMover::MoveWidget(Sexy::Widget *theWidget, int x, int y, int targetX, int targetY, bool delayDelete)
{
    WidgetMoverMap::iterator entry = mWidgetMap.insert(mWidgetMap.begin(), std::make_pair(theWidget, MoveInfo()));
    entry->second.counter = 0;
    entry->second.delayDelete = delayDelete;
    entry->second.y = y;
    entry->second.x = x;
    entry->second.targetY = targetY;
    entry->second.targetX = targetX;

    theWidget->Move(x, y);
}

void WidgetMover::DelayDeleteWidget(Sexy::Widget *theWidget)
{
    if (theWidget != NULL)
        MoveWidget(theWidget, theWidget->mX, theWidget->mY, theWidget->mX, theWidget->mY, true);
}

void WidgetMover::RemoveWidget(Sexy::Widget *theWidget)
{
    mWidgetMap.erase(theWidget);
}

void WidgetMover::SafeDeleteWidget(Sexy::Widget *theWidget)
{
    if (theWidget != NULL)
    {
        mWidgetMap.erase(theWidget);
        if (theWidget->mWidgetManager != NULL)
            theWidget->mWidgetManager->RemoveWidget(theWidget);

        gSexyAppBase->SafeDeleteWidget(theWidget);
    }
}
