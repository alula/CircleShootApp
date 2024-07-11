#ifndef __SEXY_WIDGET_MOVER_H__
#define __SEXY_WIDGET_MOVER_H__

#include <SexyAppFramework/Widget.h>

#include <map>

#include <windows.h>

namespace Sexy
{
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    class WidgetMover
    {
    public:
		WidgetMover();
        virtual ~WidgetMover();

        void Update();
        void MoveWidget(Sexy::Widget *theWidget, int x, int y, int targetX, int targetY, bool delayDelete);
        void DelayDeleteWidget(Sexy::Widget *theWidget);
        void RemoveWidget(Sexy::Widget *theWidget);
        void SafeDeleteWidget(Sexy::Widget *theWidget);

        class MoveInfo
        {
        public:
            int counter;
            int x;
            int y;
            int targetX;
            int targetY;
            bool delayDelete;

            MoveInfo() : x(0), y(0), targetX(0), targetY(0) {}
        };
		
		typedef std::map<Sexy::Widget *, MoveInfo> WidgetMoverMap;

        WidgetMoverMap mWidgetMap;
    };
};

#endif