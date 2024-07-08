#ifndef __CIRCLESHOOTAPP_H__
#define __CIRCLESHOOTAPP_H__

#include <SexyAppFramework/SexyAppBase.h>

namespace Sexy
{
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class WidgetMover;
    class Widget;
    class Board;
    class WorkerThread;
    class LevelParser;
    class LoadingScreen;
    class MainMenu;
    class AdventureScreen;
    class PracticeScreen;
    class CreditsScreen;
    class HelpScreen;
    class MoreGamesScreen;
    class ProfileMgr;
    class HighScoreMgr;
    class UserProfile;
    class Buffer;

    class CircleShootApp : public SexyAppBase
    {
    public:
        WidgetMover *mWidgetMover;
        Board *mBoard;
        MainMenu *mMainMenu;
        PracticeScreen *mPracticeScreen;
        AdventureScreen *mAdventureScreen;
        HelpScreen *mHelpScreen;
        MoreGamesScreen *mMoreGamesScreen;
        LoadingScreen *mLoadingScreen;
        CreditsScreen *mCreditsScreen;
        WorkerThread *mWorkerThread;
        LevelParser *mLevelParser;
        ProfileMgr *mProfileMgr;
        HighScoreMgr *mHighScoreMgr;
        UserProfile *mProfile;
        Buffer mSaveGameBuffer;
        bool mDidNextTempleDialog;
        int mSongId;
        int mLastSong;
        int mLastSongSwitchTime;
        int mUnk28;
        int mUnk29;
        bool mDoPlayCount;
        int mPlayCount;
        int mMaxExecutions;
        int mMaxPlays;
        int mMaxTime;
        bool mIsPractice;
        int mUnk36;

        CircleShootApp();
        virtual ~CircleShootApp();

        virtual void Init();
        virtual void Shutdown();
        virtual void UpdateFrames();
        virtual void ButtonDepress(int theId);

        virtual Dialog *NewDialog(int theDialogId, bool isModal, const SexyString &theDialogHeader, const SexyString &theDialogLines, const SexyString &theDialogFooter, int theButtonMode);
        virtual bool KillDialog(int theDialogId);

        virtual void GotFocus();
        virtual void LostFocus();

        Board *GetBoard() { return mBoard; }

        void MakeBoard();
        void CleanupWidgets();
        void ShowLoadingScreen();
        void ShowHelpScreen();
        void StartAdventureGame(int theStage);
        void StartPracticeGame(const std::string &theLevelName, int theStartLevel, bool endless);

        bool CheckSaveGame(bool showConfirm);
        void StartSavedGame(bool showConfirm);

        void SaveProfile();
        void LoadingThreadProc();
        void LoadingThreadCompleted();

        void SwitchSong(int id);
        void PlaySong(int id, bool fade, double fadeSpeed);

        void DoCheckForUpdatesDialog();
        void DoUserDialog();
        void DoConfirmQuitDialog();
        void DoCreateUserDialog();
        void DoRenameUserDialog(const std::string &theName);
        void DoConfirmDeleteUserDialog(const std::string &theName);
        void DoRegisterDialog();
        void DoNextTempleDialog();
        void DoOptionsDialog();
        void DoConfirmContinueDialog(const std::string &theVerboseLevelString, const std::string &theDisplayName, int theScore);
        void DoGetReadyDialog();
        void DoConfirmMainMenuDialog();
        void DoStatsDialog(bool slide, bool doCounter);

        void FinishUpdateDialogs(int theDialogId, bool confirm);
        void FinishUserDialog(bool confirm);
        void FinishConfirmQuitDialog(bool confirm);
        void FinishCreateUserDialog(bool confirm);
        void FinishRenameUserDialog(bool confirm);
        void FinishConfirmDeleteUserDialog(bool confirm);
        void FinishNeedRegisterDialog(bool confirm);
        void FinishRegisterDialog(bool confirm);
        void FinishNextTempleDialog(bool save);
        void FinishConfirmContinueDialog(bool startGame);
        void FinishGetReadyDialog();
        void FinishOptionsDialog(bool confirm);
        void FinishConfirmMainMenuDialog(bool mainMenu);
        void FinishStatsDialog(bool);

        bool CheckYesNoButton(int theButton);

        void ShowMainMenu();
        void ShowAdventureScreen(bool fromMenu, bool revealTemple);
        void ShowPracticeScreen(bool fromMenu);
        void ShowCreditsScreen(bool happyEnd);
        void ShowMoreGamesScreen();

        void EndHelpScreen();
        void ReturnToMainMenu();
    };

};

static inline Sexy::CircleShootApp *GetCircleShootApp() { return (Sexy::CircleShootApp *)Sexy::gSexyAppBase; }

#endif
