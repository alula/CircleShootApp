#ifndef __PRACTICESCREEN_H__
#define __PRACTICESCREEN_H__

#include <SexyAppFramework/Widget.h>
#include <SexyAppFramework/ButtonListener.h>

#include <set>

namespace Sexy
{
    class CircleButton;
    class Image;
    class Graphics;
    class MemoryImage;
    class HyperlinkWidget;
    class HighScore;
    class WidgetManager;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class PracticeScreen : public Widget, public ButtonListener
    {
    private:
        struct DoorInfo
        {
            MemoryImage *mImage;
            Rect mRect;
        };

        typedef std::multiset<HighScore> HighScoreSet;

        CircleButton *mNextButton;
        CircleButton *mBackButton;
        CircleButton *mGauntPlayButton;
        CircleButton *mMainMenuButton;
        CircleButton *mPracticeButton;
        CircleButton *mSurvivalButton;
        MemoryImage *mThumbnail;
        MemoryImage *mGradientImageLocked;
        MemoryImage *mGradientImageQuestionMark;
        bool mIsEndless;
        int mCurrentDoor;
        int mHighlightDoor;
        int mMaxStage;
        int mDifficulty;
        int mLastClickCnt;
        int mGauntletBlinkCount;
        DoorInfo mDoorInfo[4];
        int mCurrentBoard;
        std::string mStage;
        std::string mDisplayName;
        HighScoreSet *mScoreSet;
        bool mLocked;

    public:
        PracticeScreen();
        virtual ~PracticeScreen();

        virtual void Update();
        virtual void Resize(int theX, int theY, int theWidth, int theHeight);
        virtual void AddedToManager(WidgetManager *theWidgetManager);
        virtual void RemovedFromManager(WidgetManager *theWidgetManager);
        virtual void ButtonDepress(int theId);
        virtual void Draw(Graphics *g);
        virtual void KeyChar(char);
        virtual void MouseLeave();
        virtual void MouseMove(int x, int y);
        virtual void MouseDown(int x, int y, int theButton);
        virtual void MouseUp(int x, int y);

        void SyncModeButtons();
        MemoryImage *CreateGradientImage(std::string const &theName, int theAlpha);
        int GetDoorAt(int x, int y);
        MemoryImage *GetThumbnail(std::string const &theName);
        void GetHighscores();
        void LoadBoard();
    };

};

#endif