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
        void AddUFOMove(IntPoint const &unk1, IntPoint const &unk2, int unk3, int unk4);
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
        Image *mEyesImage;
        Image *unk78;
        SoundInstance *mUfoSound;
        double mEyeLeftX;
        double mEyeLeftY;
        double mEyeRightX;
        double mEyeRightY;
        int unkA0;
        int unkA4;
        bool mShowHat;

        static bool mMoveEyes;
    };

};

#endif