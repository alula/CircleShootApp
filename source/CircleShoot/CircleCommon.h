#ifndef __SEXY_CIRCLECOMMON_H__
#define __SEXY_CIRCLECOMMON_H__

#include <SexyAppFramework/Point.h>

// For convenience of making mods that want a different window size.
#define CIRCLE_WINDOW_WIDTH 640
#define CIRCLE_WINDOW_HEIGHT 480

#define MAX_PRIORITY 5
#define MAX_BALL_COLORS 6

// From looking at SexyAppFramework code it seems like this specific PI constant was just copy-pasted everywhere.
// We need this specific one instead of SEXY_PI for accurate codegen.
#define SEXY_PI 3.1415901f

#ifdef CIRCLE_ENDIAN_SWAP_ENABLED
unsigned int ByteSwap(unsigned int theValue);
#endif

namespace Sexy
{
    class ButtonListener;
    class CircleButton;
    class DialogButton;
    class Dialog;
    class EditWidget;
    class Graphics;
    class Image;
    class MemoryImage;
    class MTRand;
    class Widget;

    enum MirrorType
    {
        MirrorType_None,
        MirrorType_X,
        MirrorType_Y,
        MirrorType_XY,
    };

    enum PowerType
    {
        PowerType_Bomb,
        PowerType_SlowDown,
        PowerType_Accuracy,
        PowerType_MoveBackwards,
        PowerType_Max,
        PowerType_None = 4,
    };

    enum DialogType
    {
        DialogType_Options = 0,
        DialogType_ConfirmCheckForUpdates = 1,
        DialogType_Update = 2,
        DialogType_User = 7,
        DialogType_CreateUser = 8,
        DialogType_RenameUser = 9,
        DialogType_NameEntry = 10,
        DialogType_NameConflict = 11,
        DialogType_ConfirmDeleteUser = 12,
        DialogType_ConfirmContinue = 13,
        DialogType_Stats = 14,
        DialogType_GetReady = 15,
        DialogType_NextTemple = 16,
        DialogType_Register = 17,
        DialogType_RegisterError = 18,
        DialogType_AlreadyRegistered = 19,
        DialogType_ConfirmMainMenu = 20,
        DialogType_ConfirmQuit = 21,
        DialogType_NeedRegister = 22,
        DialogType_Max,
    };

    typedef TPoint<int> IntPoint;

    extern MTRand gThreadRand;
    extern MTRand gAppRand;

    extern int gMainThreadId;

    extern const char *gSmallGauntletStages[];
    extern const char *gGauntletStages[];
    extern uint gTextBallColors[MAX_BALL_COLORS];
    extern uint gDarkBallColors[MAX_BALL_COLORS];
    extern uint gBrightBallColors[MAX_BALL_COLORS];
    extern uint gBallColors[MAX_BALL_COLORS];

    extern int gButtonSequenceCount;
    extern int gLastButtonOver;

    extern int gSaveGameVersion;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    std::string GetTimeString(int aTime);
    void SetupEditWidget(EditWidget *theWidget);
    int AppGetTickCount();
    int GetBoardStateCount();
    int AppRand();
    int ThreadRand();
    std::string GetSaveGameName(bool, int);

    void MirrorPoint(float &x, float &y, MirrorType theMirror);
    void MirrorPoint(int &x, int &y, MirrorType theMirror);
    void MirrorRect(int &x, int &y, int w, int h, MirrorType theMirror);
    void ImageMirror(MemoryImage *theImage, MirrorType theMirror);

    inline int GetDefaultBallRadius() { return 16; }

    void DrawCheckboxText(Graphics *g, std::string const &theText, Widget *theWidget);
    void SetupButton(DialogButton *theButton, int numCols);
    CircleButton *MakeButton(int id, ButtonListener *theListener, std::string const &label, int flags, Image *image, int a6);
    void SetupDialog(Dialog *theDialog, int theMinWidth);
    MemoryImage *CutoutImageFromAlpha(MemoryImage *theBackgroundImage, MemoryImage *theAlpha, int x, int y);
    void FillCircle(Graphics *g, int x, int y, int theRadius);
    int BoardGetTickCount();
};

#endif
