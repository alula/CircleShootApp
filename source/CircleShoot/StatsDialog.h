#ifndef __STATSDIALOG_H__
#define __STATSDIALOG_H__

#include "CircleDialog.h"

namespace Sexy
{
    class Board;
    class WidgetManager;
    class CircleButton;
    class Graphics;

    class StatsDialog : public CircleDialog
    {
    public:
        StatsDialog(Board* theBoard, bool doCounter);
        virtual ~StatsDialog();

        virtual void Resize(int theX, int theY, int theWidth, int theHeight);
        virtual int GetPreferredHeight(int theWidth);

        virtual void AddedToManager(WidgetManager *theWidgetManager);
        virtual void RemovedFromManager(WidgetManager *theWidgetManager);

        virtual void Update();
        virtual void ButtonDepress(int theId);
        virtual void Draw(Graphics *g);

        void FinishScoreInc();

        Board *mBoard;
        int mTargetTimeBonus;
        int mTimeBonus;
        int mScore;
        bool mIsGameOver;
        bool mIsWinning;
        bool mHighScoreChecked;
        bool unkDF;
        std::string mCurrentTip;
        std::string mBestTimeLeft;
        std::string mBestTimeRight;
    };
};

#endif