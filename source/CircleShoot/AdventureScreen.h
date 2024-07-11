#ifndef __ADVENTURESCREEN_H__
#define __ADVENTURESCREEN_H__

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
    class AdventureScreen : public Widget, public ButtonListener
    {
    private:
        struct DoorInfo
        {
            MemoryImage *mImage;
            Rect mRect;
            Point mUnknown;
        };

        typedef std::multiset<HighScore> HighScoreSet;

        CircleButton *mMainMenuButton;
        CircleButton *mPlayButton;
        int mMaxStage;
        DoorInfo mDoorInfo[12];
        int mCurrentDoor;
        int mHighlightDoor;
        int mLastClickCnt;
        int mTempleXOffsets[4];
        int mTemples;
        bool mStartNextTempleOnReveal;
        int mStagger;
        HighScoreSet* mScoreSet;

    public:
        AdventureScreen();
        virtual ~AdventureScreen();

        virtual void Update();
        virtual void Resize(int theX, int theY, int theWidth, int theHeight);
        virtual void AddedToManager(WidgetManager *theWidgetManager);
        virtual void RemovedFromManager(WidgetManager *theWidgetManager);
        virtual void ButtonDepress(int theId);
        virtual void Draw(Graphics *g);
        virtual void KeyChar(char theChar);
        virtual void MouseLeave();
        virtual void MouseMove(int x, int y);
        virtual void MouseDown(int x, int y, int theButton);
        virtual void MouseUp(int x, int y);

        void RevealTemple(int x, int y);
        void SetStartNextTempleOnRevel(bool start);
        void DrawDoors(Graphics *g, int x, int y);
        void DrawTemple1(Graphics *g);
        void DrawTemple2(Graphics *g);
        void DrawTemple3(Graphics *g);
        void DrawTemple4(Graphics *g);
        int GetDoorAt(int x, int y);
    };

};

#endif