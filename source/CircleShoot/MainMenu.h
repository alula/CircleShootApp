#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include <SexyAppFramework/ButtonListener.h>
#include <SexyAppFramework/Widget.h>

class MainMenuOverlay;

namespace Sexy
{
    class HyperlinkWidget;
    class SoundInstance;
    class WidgetManager;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    struct UFOMove
    {
        int mUpdateCnt;
        int mDuration;
        IntPoint mStartPos;
        IntPoint mEndPos;
    };
    typedef std::list<UFOMove> UFOMoveList;

    struct UFOScale
    {
        int mUpdateCnt;
        int mDuration;
        float mStartScale;
        float mEndScale;
    };
    typedef std::list<UFOScale> UFOScaleList;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class MainMenu : public Widget, public ButtonListener
    {
    private:
    public:
        MainMenu();
        virtual ~MainMenu();

        virtual void KeyChar(char theChar);
        virtual void DrawOverlay(Graphics *g);
        virtual void RemovedFromManager(WidgetManager *theWidgetManager);
        virtual void OrderInManagerChanged();
        virtual void ButtonDepress(int theId);
        virtual void AddedToManager(WidgetManager *theWidgetManager);
        void CalcEyePos();
        virtual void MouseMove(int x, int y);
        virtual void MouseDown(int x, int y, int theClickCount);
        void SyncProfile();
        void AddUFOMove(IntPoint const &theStartPos, IntPoint const &theEndPos, int theDuration, int theStagger);
        void DoUFO();
        void UpdateUFOMove();
        void UpdateUFOScale();
        virtual void Update();
        virtual void Draw(Graphics *g);

        CircleButton *mArcadeButton;
        CircleButton *mGauntletButton;
        CircleButton *mOptionsButton;
        CircleButton *mMoreGamesButton;
        CircleButton *mQuitButton;
        HyperlinkWidget *mNotYouLink;
        MainMenuOverlay *mMainMenuOverlay;
        MemoryImage *mEyesImage;
        Image *mUFOImage;
        SoundInstance *mUFOSound;
        FPoint mEyeLeft;
        FPoint mEyeRight;
        Point mEyeCutoutPos;
        bool mShowHat;
        Point mUFOPoint;
        Point mUFOPoint2;
        int mUFOState;
        int mUFOStateCount;
        int mFlash;
        float mUFOScale;
        bool mDoUFOEasterEgg;
        UFOMoveList mUFOMoveList;
        UFOScaleList mUFOScaleList;

        static bool mMoveEyes;
    };

};

#endif