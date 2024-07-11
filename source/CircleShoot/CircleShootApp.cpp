#include "Zuma_Prefix.pch"

#include <SexyAppFramework/MTRand.h>
#include <SexyAppFramework/MusicInterface.h>
#include <SexyAppFramework/ResourceManager.h>
#include <SexyAppFramework/MemoryImage.h>
#include <SexyAppFramework/Dialog.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/ImageFont.h>
#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/Checkbox.h>
#include <SexyAppFramework/DialogButton.h>

#include "Board.h"
#include "DataSync.h"
#include "CircleCommon.h"
#include "CircleShootApp.h"
#include "LoadingScreen.h"
#include "MainMenu.h"
#include "CreateUserDialog.h"
#include "OptionsDialog.h"
#include "StatsDialog.h"
#include "UserDialog.h"
#include "AdventureScreen.h"
#include "PracticeScreen.h"
#include "MoreGamesScreen.h"
#include "CreditsScreen.h"
#include "HelpScreen.h"
#include "LevelParser.h"
#include "ProfileMgr.h"
#include "HighScoreMgr.h"
#include "WidgetMover.h"
#include "WorkerThread.h"
#include "Res.h"

using namespace Sexy;

CircleShootApp::CircleShootApp()
{
    mTitle = "Zuma Deluxe " + mProductVersion;
    mRegKey = "PopCap\\Zuma";

    mAutoEnable3D = true;

    mLevelParser = new LevelParser();
    mProfileMgr = new ProfileMgr();
    mHighScoreMgr = new HighScoreMgr();
    mWidgetMover = new WidgetMover();
    mWorkerThread = new WorkerThread();

    mWidth = CIRCLE_WINDOW_WIDTH;
    mHeight = CIRCLE_WINDOW_HEIGHT;

    mAdventureScreen = NULL;
    mBoard = NULL;
    mMainMenu = NULL;
    mPracticeScreen = NULL;
    mProfile = NULL;
    mHelpScreen = NULL;
    mMoreGamesScreen = NULL;
    mLoadingScreen = NULL;
    mCreditsScreen = NULL;

    mSongId = 0;
    mLastSong = -1;
    mUnk28 = 0;
    mMusicVolume = 0.6;
    mSfxVolume = 0.6;
    mDoPlayCount = false;
    mPlayCount = 0;
    mUnk29 = 0;
    mMaxExecutions = 0;
    mMaxPlays = 0;
    mMaxTime = 0;
}

CircleShootApp::~CircleShootApp()
{
    // Sexy::SpriteMgrDeleteGlobals()
    // Sexy::BallDeleteGlobals()
    // Sexy::GunDeleteGlobals()

    delete mLevelParser;
    delete mProfileMgr;
    delete mHighScoreMgr;
    delete mWidgetMover;
    delete mWorkerThread;

    // mResourceManager->DeleteResources("");
}

void CircleShootApp::Init()
{
    SexyAppBase::Init();

    mMaxExecutions = GetInteger("MaxExecutions", 0);
    mMaxPlays = GetInteger("MaxPlays", 0);
    mMaxTime = GetInteger("MaxTime", 60);

#ifdef _WIN32
    mLastSongSwitchTime = GetTickCount() - 100000;
    gMainThreadId = (int)GetCurrentThreadId();
#else
#ifdef _POSIX
    // todo
    gMainThreadId = (int)pthread_self();
#endif
#error Unimplemented on this platform.
#endif

    gThreadRand.SRand(Sexy::Rand());
    gAppRand.SRand(Sexy::Rand());

    this->mProfileMgr->Load();
    this->mHighScoreMgr->Load();

    std::string profile;

    if (this->RegistryReadString("CurUser", &profile))
    {
        mProfile = mProfileMgr->GetProfile(profile);
    }
    else
    {
        mProfile = mProfileMgr->GetAnyProfile();
    }

    if (!mLevelParser->ParseLevelFile("levels\\levels.xml"))
    {
        Popup(mLevelParser->GetErrorText());
        exit(0);
    }

    if (mLevelParser->mLevels.size() == 0)
    {
        Popup("No levels defined in levels.xml");
        exit(0);
    }

    // mResourceManager->SetAllowMissingProgramImages(true); // hack?

    LoadResourceManifest();
    if (!mResourceManager->LoadResources("Init"))
    {
        ShowResourceError(true);
    }

    ImageFont *fontMain10 = reinterpret_cast<ImageFont *>(mResourceManager->GetFont("FONT_MAIN10"));
    if (fontMain10)
    {
        ImageFont *fontDialog = reinterpret_cast<ImageFont *>(fontMain10->Duplicate());
        fontDialog->AddTag("Shadow1");
        mResourceManager->ReplaceFont("FONT_DIALOG", fontDialog);

        ImageFont *fontMain10Outline = reinterpret_cast<ImageFont *>(fontMain10->Duplicate());
        fontMain10Outline->RemoveTag("Outline");
        mResourceManager->ReplaceFont("FONT_MAIN10OUTLINE", fontDialog);

        ImageFont *fontMain10Outline2 = reinterpret_cast<ImageFont *>(fontMain10->Duplicate());
        fontMain10Outline2->RemoveTag("Outline2");
        mResourceManager->ReplaceFont("FONT_MAIN10OUTLINE2", fontDialog);

        ImageFont *fontMain10Outline3 = reinterpret_cast<ImageFont *>(fontMain10->Duplicate());
        fontMain10Outline3->RemoveTag("Outline3");
        mResourceManager->ReplaceFont("FONT_MAIN10OUTLINE3", fontDialog);
    }

    if (!ExtractInitResources(mResourceManager))
    {
        ShowResourceError(true);
    }

    SetCursorImage(0, Sexy::IMAGE_CURSOR_POINTER);
    SetCursorImage(1, Sexy::IMAGE_CURSOR_HAND);
    SetCursorImage(2, Sexy::IMAGE_CURSOR_DRAGGING);
    SetCursorImage(3, Sexy::IMAGE_CURSOR_TEXT);

    mMusicInterface->LoadMusic(0, "music\\zuma.mo3");
    mMusicInterface->LoadMusic(1, "music\\zuma.mo3");
    PlaySong(24, false, 0.01);
    ShowLoadingScreen();
}

void CircleShootApp::Shutdown()
{
    if (!this->mShutdown)
    {
        CleanupWidgets();

        if (this->mMaxTime <= 0)
        {
            if (this->mMaxPlays > 0)
            {
                // todo
            }
        }
        else
        {
            // todo
        }

        SexyAppBase::Shutdown();
    }
}

void CircleShootApp::UpdateFrames()
{
    SexyAppBase::UpdateFrames();
    mWidgetMover->Update();
}

void CircleShootApp::ButtonDepress(int theId)
{
    CheckYesNoButton(theId);
}

Dialog *CircleShootApp::NewDialog(int theDialogId,
                                  bool isModal,
                                  const SexyString &theDialogHeader,
                                  const SexyString &theDialogLines,
                                  const SexyString &theDialogFooter,
                                  int theButtonMode)
{
    CircleDialog *aDialog = new CircleDialog(Sexy::IMAGE_DIALOG_BACK,
                                             Sexy::IMAGE_DIALOG_BUTTON,
                                             theDialogId,
                                             isModal,
                                             theDialogHeader,
                                             theDialogLines,
                                             theDialogFooter,
                                             theButtonMode,
                                             false);

    Sexy::SetupDialog(aDialog, 348);
    return aDialog;
}

bool CircleShootApp::KillDialog(int theDialogId)
{
    Widget *aDialog = GetDialog(theDialogId);
    if (aDialog)
    {
        mWidgetMover->RemoveWidget(aDialog);
    }

    if (!SexyAppBase::KillDialog(theDialogId))
    {
        return false;
    }

    if (mDialogMap.empty())
    {
        mWidgetManager->SetFocus(mBoard);
    }

    return true;
}

void CircleShootApp::GotFocus()
{
    if (mBoard)
    {
        mBoard->Pause(false);
    }
}

void CircleShootApp::LostFocus()
{
    if (mBoard)
    {
        mBoard->Pause(true);
    }
}

void CircleShootApp::MakeBoard()
{
    mDidNextTempleDialog = false;
    CleanupWidgets();

    mBoard = new Board(this);

    mBoard->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mBoard);
    mWidgetManager->SetFocus(mBoard);
    mDoPlayCount = false;
}

void CircleShootApp::CleanupWidgets()
{
    if (mAdventureScreen)
    {
        mWidgetMover->SafeDeleteWidget(mAdventureScreen);
        mAdventureScreen = NULL;
    }

    if (mBoard)
    {
        if (mDoPlayCount)
        {
            mPlayCount++;
            mDoPlayCount = false;
        }

        mBoard->WaitForLoadingThread();
        if (mBoard->NeedSaveGame())
        {
            mBoard->SaveGame(GetSaveGameName(mBoard->IsPracticeMode(), mProfile->mId));
        }

        mWidgetMover->SafeDeleteWidget(mBoard);
        mBoard = NULL;
    }

    if (mLoadingScreen)
    {
        mWidgetMover->SafeDeleteWidget(mLoadingScreen);
        mLoadingScreen = NULL;
    }

    if (mMainMenu)
    {
        mWidgetMover->SafeDeleteWidget(mMainMenu);
        mMainMenu = NULL;
    }

    if (mPracticeScreen)
    {
        mWidgetMover->SafeDeleteWidget(mPracticeScreen);
        mPracticeScreen = NULL;
    }

    if (mMoreGamesScreen)
    {
        mWidgetMover->SafeDeleteWidget(mMoreGamesScreen);
        mMoreGamesScreen = NULL;
    }

    if (mHelpScreen)
    {
        mWidgetMover->SafeDeleteWidget(mHelpScreen);
        mHelpScreen = NULL;
    }

    if (mCreditsScreen)
    {
        mWidgetMover->SafeDeleteWidget(mCreditsScreen);
        mCreditsScreen = NULL;
    }
}

void CircleShootApp::ShowLoadingScreen()
{
    CleanupWidgets();

    mLoadingScreen = new LoadingScreen();

    mLoadingScreen->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mLoadingScreen);
    mWidgetManager->SetFocus(mLoadingScreen);
}

void CircleShootApp::ShowHelpScreen()
{
    mHelpScreen = new HelpScreen();
    mHelpScreen->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mHelpScreen);
    mWidgetManager->SetFocus(mHelpScreen);
    if (mBoard)
    {
        mBoard->Pause(true, true);
    }
    ClearUpdateBacklog();
}

void CircleShootApp::StartAdventureGame(int theStage)
{
    if (mDidNextTempleDialog && theStage == mProfile->mMaxStage && CheckSaveGame(false))
    {
        StartSavedGame(true);
    }
    else
    {
        MakeBoard();
        mBoard->SetStartLevel(mLevelParser->GetLevelNumByStage(theStage));
        mBoard->Reset(true);
        if (mProfile->mShowHelpScreen)
        {
            ShowHelpScreen();
        }

        PlaySong(0, 1, 0.01);
    }
}

void CircleShootApp::StartPracticeGame(const std::string &theLevelName, int theStartLevel, bool endless)
{
    MakeBoard();
    mBoard->SetPracticeBoard(theLevelName);
    mBoard->SetStartLevel(theStartLevel);
    mBoard->SetIsEndless(endless);
    mBoard->Reset(true);
    if (mProfile->mShowHelpScreen)
    {
        ShowHelpScreen();
    }

    PlaySong(0, true, 0.01);
}

bool CircleShootApp::CheckSaveGame(bool showConfirm)
{
    std::string aSaveGameName = GetSaveGameName(mIsPractice, mProfile->mId);
    if (!ReadBufferFromFile(aSaveGameName, &mSaveGameBuffer))
        return false;

    DataReader aReader;
    aReader.OpenMemory(mSaveGameBuffer.GetDataPtr(), mSaveGameBuffer.GetDataLen(), false);
    if (aReader.ReadLong() != gSaveGameVersion)
    {
        EraseFile(aSaveGameName);
        mSaveGameBuffer.Clear();
        return false;
    }

    std::string aVerboseLevelString;
    std::string aLevelDisplayName;
    bool aIsPractice = aReader.ReadBool();
    aReader.ReadString(aVerboseLevelString);
    aReader.ReadString(aLevelDisplayName);
    int aScore = aReader.ReadLong();
    if (showConfirm)
    {
        if (aScore > 0)
        {
            DoConfirmContinueDialog(aVerboseLevelString, aLevelDisplayName, aScore);
        }
        else
        {
            EraseFile(aSaveGameName);
            mSaveGameBuffer.Clear();
        }
    }
    else
    {
        EraseFile(aSaveGameName);
    }

    return true;
}

void CircleShootApp::StartSavedGame(bool showConfirm)
{
    MakeBoard();
    mBoard->SetStartLevel(0);
    mBoard->LoadGame(mSaveGameBuffer);
    mSaveGameBuffer.Clear();
    if (!showConfirm)
    {
        if (mProfile->mShowHelpScreen)
        {
            ShowHelpScreen();
        }
        else
        {
            DoGetReadyDialog();
        }
    }
    PlaySong(0, true, 0.01);
}

void CircleShootApp::SaveProfile()
{
    if (mProfile)
    {
        mProfile->SaveDetails();
    }
}

void CircleShootApp::LoadingThreadProc()
{
    const char *resourceGroups[5];
    resourceGroups[0] = "Register";
    resourceGroups[1] = "LoadingThread";
    resourceGroups[2] = "MainMenu";
    resourceGroups[3] = "AdventureScreen";
    resourceGroups[4] = "GauntletScreen";

    mUnk28 = 0;
    mNumLoadingThreadTasks = 0;
    int i;

    for (i = 0; i < 5; i++)
    {
        mNumLoadingThreadTasks +=
            mResourceManager->GetNumResources(resourceGroups[i]);
    }
    ++mNumLoadingThreadTasks;

    i = 0;
    for (;;)
    {
        mResourceManager->StartLoadResources(resourceGroups[i]);

        while (mResourceManager->LoadNextResource())
            ++mCompletedLoadingThreadTasks;

        if (mResourceManager->HadError() ||
            i == 0 && !Sexy::ExtractResourcesByName(mResourceManager,
                                                    resourceGroups[0]))
        {
            break;
        }

        ++mUnk28;
        ++i;

        if (i == 5)
        {
            Sexy::SharedImageRef checkBoxLine = mResourceManager->GetImage("IMAGE_DIALOG_CHECKBOXLINE");
            if (checkBoxLine.mSharedImage != NULL)
            {
                Sexy::MemoryImage *image = new MemoryImage(this);
                image->Create(20, checkBoxLine->mHeight);

                Sexy::Graphics g(image);

                for (int j = 0; j != 20; ++j)
                    g.DrawImage(checkBoxLine, j, 0);

                mResourceManager->ReplaceImage("IMAGE_DIALOG_CHECKBOXLINE", image);
            }

            Sexy::SharedImageRef ballShadow = mResourceManager->GetImage("IMAGE_BALL_SHADOW");
            if (ballShadow.mSharedImage != NULL)
            {
                Sexy::MemoryImage *image = new MemoryImage(this);
                image->Create(ballShadow->mWidth, ballShadow->mHeight);

                Sexy::Graphics g(image);
                g.SetColorizeImages(true);
                g.SetColor(Sexy::Color(0, 0, 0, 96));
                g.DrawImage(ballShadow, 0, 0);

                mResourceManager->ReplaceImage("IMAGE_BALL_SHADOW", image);
            }

            Sexy::Font *main8Font = mResourceManager->GetFont("FONT_MAIN8");
            if (main8Font != NULL)
            {
                Sexy::ImageFont *imFont = reinterpret_cast<Sexy::ImageFont *>(main8Font);
                imFont->AddTag("Outline");

                mResourceManager->ReplaceFont("FONT_MAIN8OUTLINE", imFont);
            }

            int v10 = 1;
            while (Sexy::ExtractResourcesByName(mResourceManager, resourceGroups[v10]))
            {
                if (++v10 >= 5)
                {
                    ++mCompletedLoadingThreadTasks;
                    return;
                }
            }

            break;
        }
    }

    ShowResourceError();
    mLoadingFailed = true;
}

void CircleShootApp::LoadingThreadCompleted()
{
    if (ShouldCheckForUpdate())
    {
        DoConfirmCheckForUpdatesDialog();
    }
}

void CircleShootApp::FinishStatsDialog(bool confirm)
{
    KillDialog(DialogType_Stats);
    if (mBoard == NULL)
        return;

    if (!mBoard->IsGameOver())
    {
        mBoard->Pause(false, true);
    }
    else if (mBoard->IsPracticeMode())
    {
        ShowPracticeScreen(false);
    }
    else if (!mBoard->IsWinning())
    {
        ShowAdventureScreen(false, false);
    }
    else if (mBoard->GetCurrentStage() < 12)
    {
        ShowAdventureScreen(false, true);
    }
    else
    {
        ShowCreditsScreen(true);
    }
}

void CircleShootApp::FinishConfirmQuitDialog(bool confirm)
{
    KillDialog(DialogType_ConfirmQuit);

    if (confirm)
    {
        Shutdown();
    }
}

void CircleShootApp::DoStatsDialog(bool slide, bool doCounter)
{
    if (mBoard == NULL)
        return;

    mBoard->Pause(true, true);

    StatsDialog *aDialog = new StatsDialog(mBoard, doCounter);
    SetupDialog(aDialog, 460);
    AddDialog(DialogType_Stats, aDialog);

    if (slide)
    {
        mWidgetMover->MoveWidget(
            aDialog,
            aDialog->mX,
            aDialog->mHeight + 480,
            aDialog->mX,
            aDialog->mY,
            false);
    }
}

void CircleShootApp::DoNextTempleDialog()
{
    mDidNextTempleDialog = true;
    DoDialog(DialogType_NextTemple, true, "Enter Next Temple", "You are now going to enter the next temple.\nGet Ready!", "", Dialog::BUTTONS_OK_CANCEL);
}

void CircleShootApp::FinishNextTempleDialog(bool save)
{
    KillDialog(DialogType_NextTemple);

    if (save && CheckSaveGame(false))
    {
        StartSavedGame(true);
    }
    else if (mAdventureScreen)
    {
        mWidgetManager->SetFocus(mAdventureScreen);
    }
}

void CircleShootApp::DoUserDialog()
{
    KillDialog(DialogType_User);
    UserDialog *aDialog = new UserDialog();
    SetupDialog(aDialog, 400);
    AddDialog(DialogType_User, aDialog);
}

void CircleShootApp::DoCreateUserDialog()
{
    KillDialog(DialogType_CreateUser);
    CreateUserDialog *aDialog = new CreateUserDialog(false);
    SetupDialog(aDialog, 400);
    AddDialog(DialogType_CreateUser, aDialog);
}

void CircleShootApp::DoRenameUserDialog(const std::string &theName)
{
    KillDialog(DialogType_RenameUser);
    CreateUserDialog *aDialog = new CreateUserDialog(true);
    aDialog->SetName(theName);
    SetupDialog(aDialog, 400);
    AddDialog(DialogType_RenameUser, aDialog);
}

void CircleShootApp::DoConfirmDeleteUserDialog(const std::string &theName)
{
    KillDialog(DialogType_ConfirmDeleteUser);

    std::string aText = Sexy::StrFormat("This will permanently remove '%s' from the player roster!", theName.c_str());
    DoDialog(DialogType_ConfirmDeleteUser, true, "Are You Sure?", aText, "", Dialog::BUTTONS_YES_NO);
}

void CircleShootApp::FinishUserDialog(bool confirm)
{
    UserDialog *aDialog = (UserDialog *)GetDialog(DialogType_User);
    if (!aDialog)
        return;

    if (confirm)
    {
        std::string aName = aDialog->GetSelName();
        UserProfile *aProfile = mProfileMgr->GetProfile(aName);

        if (aProfile)
        {
            mProfile = aProfile;
            mWidgetManager->MarkAllDirty();
            if (mMainMenu)
            {
                mMainMenu->SyncProfile();
            }
        }
    }

    KillDialog(DialogType_User);
}

void CircleShootApp::FinishCreateUserDialog(bool confirm)
{
    CreateUserDialog *aDialog = (CreateUserDialog *)GetDialog(DialogType_CreateUser);
    if (!aDialog)
        return;

    std::string aName = aDialog->GetName();
    if (confirm && aName.empty())
    {
        DoDialog(DialogType_NameEntry, true, "Enter Your Name",
                 "Enter your name to create a new user profile for storing high score data and games in progress.",
                 "OK", Dialog::BUTTONS_FOOTER);
        return;
    }

    if (mProfile)
    {
        if (!confirm)
        {
            KillDialog(DialogType_CreateUser);
            return;
        }
    }
    else if (!confirm || aName.empty())
    {
        DoDialog(DialogType_NameEntry, true, "Enter Your Name",
                 "Enter your name to create a new user profile for storing high score data and games in progress.",
                 "OK", Dialog::BUTTONS_FOOTER);
        return;
    }

    UserProfile *aProfile = mProfileMgr->AddProfile(aName);
    if (aProfile)
    {
        mProfileMgr->Save();
        mProfile = aProfile;
        KillDialog(DialogType_User);
        KillDialog(DialogType_CreateUser);
        mWidgetManager->MarkAllDirty();
        if (mMainMenu)
        {
            mMainMenu->SyncProfile();
        }
    }
    else
    {
        // yup, it's 10 here
        DoDialog(DialogType_NameEntry, true, "Name Conflict",
                 "The name you entered is already being used.  Please enter a unique player name.",
                 "OK", Dialog::BUTTONS_FOOTER);
    }
}

void CircleShootApp::FinishRenameUserDialog(bool confirm)
{
    if (!confirm)
    {
        KillDialog(DialogType_RenameUser);
        return;
    }

    UserDialog *aUserDialog = (UserDialog *)GetDialog(DialogType_User);
    CreateUserDialog *aCreateDialog = (CreateUserDialog *)GetDialog(DialogType_RenameUser);
    if (!aUserDialog || !aCreateDialog)
        return;

    std::string aSelName = aUserDialog->GetSelName();
    std::string aNewName = aCreateDialog->GetName();

    if (!aNewName.empty())
    {
        int cmp = stricmp(aSelName.c_str(), aNewName.c_str());
        if (mProfileMgr->RenameProfile(aSelName, aNewName))
        {
            mProfileMgr->Save();

            if (!cmp)
            {
                mProfile = mProfileMgr->GetProfile(aNewName);
            }

            aUserDialog->FinishRenameUser(aNewName);
            mWidgetManager->MarkAllDirty();
            KillDialog(DialogType_RenameUser);
        }
        else
        {
            DoDialog(DialogType_NameConflict, true, "Name Conflict",
                     "The name you entered is already being used.  Please enter a unique player name.",
                     "OK", Dialog::BUTTONS_FOOTER);
        }
    }
}

void CircleShootApp::FinishConfirmDeleteUserDialog(bool confirm)
{
    KillDialog(DialogType_ConfirmDeleteUser);
    if (!confirm)
        return;

    UserDialog *aDialog = (UserDialog *)GetDialog(DialogType_User);
    if (!aDialog)
        return;

    std::string aCurProfileName = mProfile ? mProfile->mName : "";
    std::string aSelName = aDialog->GetSelName();

    if (aSelName == aCurProfileName)
    {
        mProfile = NULL;
    }

    mProfileMgr->DeleteProfile(aSelName);
    aDialog->FinishDeleteUser();

    if (!mProfile)
    {
        std::string aNewName = aDialog->GetSelName();
        mProfile = mProfileMgr->GetProfile(aNewName);
        if (!mProfile)
        {
            mProfile = mProfileMgr->GetAnyProfile();
        }
    }

    mProfileMgr->Save();

    if (!mProfile)
    {
        DoCreateUserDialog();
    }

    mWidgetManager->MarkAllDirty();

    if (mMainMenu)
    {
        mMainMenu->SyncProfile();
    }
}

void CircleShootApp::DoRegisterDialog()
{
}

void CircleShootApp::DoCheckForUpdatesDialog()
{
}

void CircleShootApp::DoConfirmMainMenuDialog()
{
    if (mBoard && mBoard->NeedSaveGame())
    {
        Dialog *aDialog = DoDialog(DialogType_ConfirmMainMenu, true, "Leave Game?", "Your game session will be saved upon leaving. Do you want to continue?", "", Dialog::BUTTONS_OK_CANCEL);
        aDialog->mYesButton->mLabel = "Leave";

        OptionsDialog *aDialogOptions = (OptionsDialog *)GetDialog(DialogType_Options);
        if (aDialogOptions)
        {
            aDialog->Resize(aDialog->mX, aDialogOptions->mY + aDialogOptions->mHeight - aDialog->mHeight, aDialog->mWidth, aDialog->mHeight);
        }
    }
    else
    {
        ShowMainMenu();
    }
}

void CircleShootApp::FinishUpdateDialogs(int theDialogId, bool confirm)
{
}

void CircleShootApp::DoConfirmQuitDialog()
{
    DoDialog(DialogType_ConfirmQuit, true, "Quit Zuma?", "Are you sure you want to\nquit the game?", "", Dialog::BUTTONS_YES_NO);
}

void CircleShootApp::SwitchSong(int id)
{
    if (this->mLastSong == id ||
        (GetTickCount() - this->mLastSongSwitchTime) >= 5000)
    {
        PlaySong(id, true, 0.01);
    }
}

void CircleShootApp::DoOptionsDialog()
{
    if (mBoard != NULL)
    {
        mBoard->Pause(true, true);
    }

    Dialog *dialog = new OptionsDialog(mBoard != NULL);
    SetupDialog(dialog, 400);
    AddDialog(DialogType_Options, dialog);
}

void CircleShootApp::DoConfirmContinueDialog(const std::string &theVerboseLevelString, const std::string &theDisplayName, int theScore)
{
    std::string aScore = Sexy::StrFormat("%s (%s)\r\nScore: %d\r\n", theVerboseLevelString.c_str(), theDisplayName.c_str(), theScore);
    std::string aText = "Your game was saved when you quit.  If you do not continue now, the game will be lost.\r\n"
                        "\r\n"
                        "Do you want to continue your last game?"
                        "\r\n"
                        "\r\n" +
                        aScore;

    Dialog *aDialog = NewDialog(DialogType_ConfirmContinue, true, "CONTINUE?", aText, "", Dialog::BUTTONS_OK_CANCEL);

    aDialog->mYesButton->mLabel = "Continue";
    aDialog->mNoButton->mLabel = "New Game";

    AddDialog(DialogType_ConfirmContinue, aDialog);
}

void CircleShootApp::DoGetReadyDialog()
{
    if (mDialogMap.empty())
    {
        DoDialog(DialogType_GetReady, true, "GO", "", "Get Ready!", Dialog::BUTTONS_FOOTER);
        if (mBoard)
        {
            mBoard->Pause(true, true);
        }

        mBoard->SetShowBallsDuringPause(true);
        mBoard->SetFullPauseFade();
    }
}

void CircleShootApp::PlaySong(int id, bool fade, double fadeSpeed)
{
    if (this->mLastSong != id)
    {
        int prevSong = mSongId;

        if (fade)
        {
            mSongId = (mSongId + 1) % 2;
            mMusicInterface->FadeOut(prevSong, true, fadeSpeed);
            mMusicInterface->FadeIn(mSongId, id, fadeSpeed * 0.5);
        }
        else
        {
            mMusicInterface->PlayMusic(mSongId, id);
        }

        mLastSong = id;
        mLastSongSwitchTime = GetTickCount();
    }
}

void CircleShootApp::FinishGetReadyDialog()
{
    KillDialog(DialogType_GetReady);
    if (mBoard)
    {
        mBoard->Pause(false, true);
    }
}

void CircleShootApp::FinishConfirmContinueDialog(bool startGame)
{
    KillDialog(DialogType_ConfirmContinue);

    std::string aSaveGameName = GetSaveGameName(mIsPractice, mProfile->mId);
    EraseFile(aSaveGameName);

    if (startGame)
    {
        StartSavedGame(true);
    }
    else if (mIsPractice)
    {
        ShowPracticeScreen(false);
    }
    else
    {
        ShowAdventureScreen(false, false);
    }
}

void CircleShootApp::FinishOptionsDialog(bool saveSettings)
{
    Sexy::OptionsDialog *dialog = (OptionsDialog *)GetDialog(DialogType_Options);
    if (dialog != NULL)
    {
        if (saveSettings)
        {
            bool fullscreen = dialog->mFullScreenCheckbox->IsChecked();
            bool acceleration = dialog->m3DAccelCheckbox->IsChecked();
            SwitchScreenMode(fullscreen != true, acceleration);
            bool cursorsEnabled = dialog->mCustomCursorsCheckbox->IsChecked();
            EnableCustomCursors(cursorsEnabled);
            ClearUpdateBacklog();
        }

        KillDialog(0);
        if (mBoard)
        {
            mBoard->Pause(false, true);
        }
    }
}

void CircleShootApp::FinishConfirmMainMenuDialog(bool mainMenu)
{
    KillDialog(DialogType_ConfirmMainMenu);
    if (mainMenu)
    {
        ShowMainMenu();
    }
}

bool CircleShootApp::CheckYesNoButton(int theButton)
{
    if ((theButton % 10000 - 2000) < 1000)
    {
        switch (theButton)
        {
        case 2000:
            FinishOptionsDialog(true);
            return true;
        case 2001:
            // DoCheckForUpdatesDialog();
            return true;
        case 2002:
        case 12002:
        case 22002:
            // FinishUpdateDialogs(theButton - 2000, true);
            return true;
        case 2007:
            FinishUserDialog(true);
            return true;
        case 2008:
            FinishCreateUserDialog(true);
            return true;
        case 2009:
            FinishRenameUserDialog(true);
            return true;
        case 2012:
            FinishConfirmDeleteUserDialog(true);
            return true;
        case 2013:
            FinishConfirmContinueDialog(true);
            return true;
        case 2014:
            FinishStatsDialog(true);
            return true;
        case 2015:
            FinishGetReadyDialog();
            return true;
        case 2016:
            FinishNextTempleDialog(true);
            return true;
        case 2017:
            // FinishRegisterDialog(true);
            return true;
        case 2020:
            FinishConfirmMainMenuDialog(true);
            return true;
        case 2021:
            FinishConfirmQuitDialog(true);
            return true;
        case 2022:
            // FinishNeedRegisterDialog(true);
            return true;
        default:
            KillDialog(theButton - 2000);
            return true;
        }
    }
    else if ((theButton % 10000 - 3000) < 1000)
    {
        switch (theButton)
        {
        case 3002:
        case 13002:
        case 23002:
            // FinishUpdateDialogs(theButton - 3000, true);
            return true;
        case 3007:
            FinishUserDialog(false);
            return true;
        case 3008:
            FinishCreateUserDialog(false);
            return true;
        case 3009:
            FinishRenameUserDialog(false);
            return true;
        case 3012:
            FinishConfirmDeleteUserDialog(false);
            return true;
        case 3013:
            FinishConfirmContinueDialog(false);
            return true;
        case 3014:
            FinishStatsDialog(false);
            return true;
        case 3016:
            FinishNextTempleDialog(false);
            return true;
        case 3017:
            // FinishRegisterDialog(false);
            return true;
        case 3020:
            FinishConfirmMainMenuDialog(false);
            return true;
        case 3021:
            FinishConfirmQuitDialog(false);
            return true;
        case 3022:
            // FinishNeedRegisterDialog(false);
            return true;
        default:
            KillDialog(theButton - 3000);
            return true;
        }
    }

    return false;
}

void CircleShootApp::ShowMainMenu()
{
    if (mPracticeScreen != NULL)
    {
        mWidgetMover->MoveWidget(mPracticeScreen, mPracticeScreen->mX, mPracticeScreen->mY, -mPracticeScreen->mWidth, mPracticeScreen->mY, true);
        mPracticeScreen = NULL;
    }

    if (mAdventureScreen != NULL)
    {
        mWidgetMover->MoveWidget(mAdventureScreen, mAdventureScreen->mX, mAdventureScreen->mY, -mAdventureScreen->mWidth, mAdventureScreen->mY, true);
        mAdventureScreen = NULL;
    }

    if (mMoreGamesScreen != NULL)
    {
        mWidgetMover->MoveWidget(mMoreGamesScreen, mMoreGamesScreen->mX, mMoreGamesScreen->mY, -mMoreGamesScreen->mWidth, mMoreGamesScreen->mY, true);
        mMoreGamesScreen = NULL;
    }

    FinishOptionsDialog(true);
    CleanupWidgets();

    mMainMenu = new MainMenu();
    mMainMenu->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mMainMenu);
    mWidgetManager->BringToBack(mMainMenu);
    mWidgetManager->SetFocus(mMainMenu);

    if (!mProfile)
    {
        DoCreateUserDialog();
    }

    PlaySong(28, true, 0.01);
    ClearUpdateBacklog();
}

void CircleShootApp::ShowAdventureScreen(bool fromMenu, bool revealTemple)
{
    mIsPractice = false;
    if (fromMenu && CheckSaveGame(true))
        return;

    MainMenu *aMainMenu = mMainMenu;
    mWidgetMover->DelayDeleteWidget(mMainMenu);
    mMainMenu = NULL;

    CleanupWidgets();

    mAdventureScreen = new AdventureScreen();
    mAdventureScreen->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mAdventureScreen);
    mWidgetManager->SetFocus(mAdventureScreen);

    if (revealTemple)
    {
        mAdventureScreen->RevealTemple(75, mProfile->mMaxStage / 3 + 1);
        mAdventureScreen->SetStartNextTempleOnRevel(true);
    }

    if (aMainMenu)
    {
        mWidgetMover->MoveWidget(mAdventureScreen, -mAdventureScreen->mWidth, 0, 0, 0, false);
    }

    PlaySong(32, true, 0.01);
    ClearUpdateBacklog();
}

void CircleShootApp::ShowPracticeScreen(bool fromMenu)
{
    mIsPractice = true;
    if (fromMenu && CheckSaveGame(true))
        return;

    MainMenu *aMainMenu = mMainMenu;
    mWidgetMover->DelayDeleteWidget(mMainMenu);
    mMainMenu = NULL;

    CleanupWidgets();

    mPracticeScreen = new PracticeScreen();
    mPracticeScreen->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mPracticeScreen);
    mWidgetManager->SetFocus(mPracticeScreen);

    if (aMainMenu)
    {
        mWidgetMover->MoveWidget(mPracticeScreen, -mPracticeScreen->mWidth, 0, 0, 0, false);
    }

    PlaySong(34, true, 0.01);
    ClearUpdateBacklog();
}

void CircleShootApp::ShowCreditsScreen(bool happyEnd)
{
    CleanupWidgets();
    mCreditsScreen = new CreditsScreen(happyEnd);
    mCreditsScreen->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mCreditsScreen);
    mWidgetManager->SetFocus(mCreditsScreen);
    PlaySong(0, true, 0.01);
}

void CircleShootApp::ShowMoreGamesScreen()
{
}

void CircleShootApp::EndHelpScreen()
{
    if (mHelpScreen)
    {
        mWidgetMover->SafeDeleteWidget(mHelpScreen);
        mHelpScreen = NULL;
    }

    if (mBoard)
    {
        mWidgetManager->SetFocus(mBoard);

        if (mBoard)
        {
            mBoard->Pause(false, true);
        }

        if (mBoard->IsSavedGame())
        {
            DoGetReadyDialog();
        }
    }
}

void CircleShootApp::ReturnToMainMenu()
{
    DoConfirmMainMenuDialog();
}