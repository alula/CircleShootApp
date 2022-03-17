#include "Zuma_Prefix.pch"

#include <SexyAppFramework/ResourceManager.h>
#include "CircleCommon.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static void *gResources[] =
    {
        &IMAGE_LOADING_SCREEN,
        &IMAGE_LOADERBAR,
        &IMAGE_CURSOR_POINTER,
        &IMAGE_CURSOR_HAND,
        &IMAGE_CURSOR_DRAGGING,
        &IMAGE_CURSOR_TEXT,
        &FONT_MAIN10,
        &FONT_DIALOG,
        &FONT_MAIN10OUTLINE,
        &FONT_MAIN10OUTLINE2,
        &FONT_MAIN10OUTLINE3,
        &FONT_FLOAT,
        &IMAGE_DIALOG_BUTTON,
        &IMAGE_DIALOG_BACK,
        &FONT_TITLE,
        &FONT_PLAIN,
        &SOUND_BUTTON1,
        &IMAGE_MOREGAMES_SCREEN,
        &IMAGE_MOREGAMES_BUTTON,
        &IMAGE_HELP_FRONT,
        &IMAGE_HELP_BACK,
        &IMAGE_MM_BACK,
        &IMAGE_MM_GAUNTLET,
        &IMAGE_MM_ARCADE,
        &IMAGE_MM_MOREGAMES,
        &IMAGE_MM_OPTIONS,
        &IMAGE_MM_QUIT,
        &IMAGE_MM_EYELEFT,
        &IMAGE_MM_EYERIGHT,
        &IMAGE_MM_EYEMASK,
        &IMAGE_MM_SUN,
        &IMAGE_MM_SKY,
        &IMAGE_MM_HAT,
        &IMAGE_ADVSKY,
        &IMAGE_ADVBACK,
        &IMAGE_ADVMIDDLE,
        &IMAGE_ADVTEMPLE1,
        &IMAGE_ADVTEMPLE2,
        &IMAGE_ADVTEMPLE3,
        &IMAGE_ADVTEMPLE4,
        &IMAGE_ADVTEMPLE2V,
        &IMAGE_ADVTEMPLE3V,
        &IMAGE_ADVDOOR1A,
        &IMAGE_ADVDOOR1B,
        &IMAGE_ADVDOOR1C,
        &IMAGE_ADVDOOR2A,
        &IMAGE_ADVDOOR2B,
        &IMAGE_ADVDOOR2C,
        &IMAGE_ADVDOOR3A,
        &IMAGE_ADVDOOR3B,
        &IMAGE_ADVDOOR3C,
        &IMAGE_ADVDOOR4A,
        &IMAGE_ADVDOOR4B,
        &IMAGE_ADVDOOR4C,
        &IMAGE_ADVMAINMENUBUTTON,
        &IMAGE_ADVPLAYBUTTON,
        &IMAGE_ADVSTAGE,
        &IMAGE_ADVTITLE,
        &IMAGE_ADVHIGHSCORE,
        &IMAGE_GAUNTSCREEN,
        &IMAGE_GAUNTSKY,
        &IMAGE_GAUNTDOOR1,
        &IMAGE_GAUNTDOOR2,
        &IMAGE_GAUNTDOOR3,
        &IMAGE_GAUNTDOOR4,
        &IMAGE_GAUNTBACKBUTTON,
        &IMAGE_GAUNTMAINMENUBUTTON,
        &IMAGE_GAUNTNEXTBUTTON,
        &IMAGE_GAUNTPLAYBUTTON,
        &IMAGE_GAUNTEAGLEGEM,
        &IMAGE_GAUNTJAGUARGEM,
        &IMAGE_GAUNTSUNGEM,
        &IMAGE_GAUNTTITLE,
        &IMAGE_GAUNTHIGHSCORE,
        &IMAGE_GAUNTPRACTICEBUTTON,
        &IMAGE_GAUNTSURVIVALBUTTON,
        &IMAGE_SUNGLOW,
        &IMAGE_BLUE_BALL,
        &IMAGE_YELLOW_BALL,
        &IMAGE_RED_BALL,
        &IMAGE_GREEN_BALL,
        &IMAGE_PURPLE_BALL,
        &IMAGE_WHITE_BALL,
        &IMAGE_NEXT_BALL,
        &IMAGE_BALL_ALPHA,
        &IMAGE_BALL_SHADOW,
        &IMAGE_BLUE_ACCURACY,
        &IMAGE_YELLOW_ACCURACY,
        &IMAGE_RED_ACCURACY,
        &IMAGE_GREEN_ACCURACY,
        &IMAGE_PURPLE_ACCURACY,
        &IMAGE_WHITE_ACCURACY,
        &IMAGE_BLUE_BACKWARDS,
        &IMAGE_YELLOW_BACKWARDS,
        &IMAGE_RED_BACKWARDS,
        &IMAGE_GREEN_BACKWARDS,
        &IMAGE_PURPLE_BACKWARDS,
        &IMAGE_WHITE_BACKWARDS,
        &IMAGE_BLUE_SLOW,
        &IMAGE_YELLOW_SLOW,
        &IMAGE_RED_SLOW,
        &IMAGE_GREEN_SLOW,
        &IMAGE_PURPLE_SLOW,
        &IMAGE_WHITE_SLOW,
        &IMAGE_GRAY_EXPLOSION,
        &IMAGE_EXPLOSION,
        &IMAGE_BLUE_BOMB,
        &IMAGE_YELLOW_BOMB,
        &IMAGE_RED_BOMB,
        &IMAGE_GREEN_BOMB,
        &IMAGE_PURPLE_BOMB,
        &IMAGE_WHITE_BOMB,
        &IMAGE_BLUE_LIGHT,
        &IMAGE_YELLOW_LIGHT,
        &IMAGE_RED_LIGHT,
        &IMAGE_GREEN_LIGHT,
        &IMAGE_PURPLE_LIGHT,
        &IMAGE_WHITE_LIGHT,
        &IMAGE_BACKWARDS_LIGHT,
        &IMAGE_ACCURACY_LIGHT,
        &IMAGE_SLOW_LIGHT,
        &IMAGE_SPARKLE,
        &IMAGE_SHOOTER_BOTTOM,
        &IMAGE_SHOOTER_TOP,
        &IMAGE_SHOOTER_TONGUE,
        &IMAGE_EYE_BLINK,
        &IMAGE_DIALOG_CHECKBOX,
        &IMAGE_DIALOG_CHECKBOXCAP,
        &IMAGE_DIALOG_CHECKBOXLINE,
        &IMAGE_SLIDER_TRACK,
        &IMAGE_SLIDER_THUMB,
        &IMAGE_UPDATE_BAR,
        &IMAGE_GOD_HEAD,
        &IMAGE_HOLE,
        &IMAGE_HOLE_COVER,
        &IMAGE_COIN,
        &IMAGE_RIGHT_MOUSE,
        &IMAGE_MENU_BAR,
        &IMAGE_MENU_BUTTON,
        &IMAGE_ZUMA_BAR,
        &IMAGE_ZUMA_BAR_DONE,
        &IMAGE_FROG_LIVES,
        &SOUND_BALLCLICK1,
        &SOUND_BALLCLICK2,
        &SOUND_BALLFIRE,
        &SOUND_WARNING,
        &SOUND_EXPLODE,
        &SOUND_EARTHQUAKE,
        &SOUND_FROGLAND,
        &SOUND_JEWEL_APPEAR,
        &SOUND_JEWEL_DISAPPEAR,
        &SOUND_JEWEL_HIT,
        &SOUND_CHAIN_BONUS,
        &SOUND_GAP_BONUS,
        &SOUND_ROLLING,
        &SOUND_EXTRA_LIFE,
        &SOUND_SLOWDOWN_BALL,
        &SOUND_BACKWARDS_BALL,
        &SOUND_ACCURACY_BALL,
        &SOUND_TRAIL_LIGHT,
        &SOUND_TRAIL_LIGHT_END,
        &SOUND_GAME_START,
        &SOUND_LEVEL_UP,
        &SOUND_BAR_FULL,
        &SOUND_LOST_LIFE,
        &SOUND_GAME_OVER,
        &SOUND_STAGE_COMPLETE,
        &SOUND_TEMPLE_COMPLETE,
        &SOUND_BONUS_EXPLOSION,
        &SOUND_BALLDESTROYED1,
        &SOUND_BALLDESTROYED2,
        &SOUND_BALLDESTROYED3,
        &SOUND_BALLDESTROYED4,
        &SOUND_BALLDESTROYED5,
        &SOUND_COMBO,
        &SOUND_BUTTON2,
        &SOUND_CHORAL1,
        &SOUND_POP,
        &SOUND_UFO,
        &FONT_HUGE,
        &FONT_MONODIGIT,
        &FONT_BROWNTITLE,
        &FONT_MAIN12,
        &FONT_MAIN8,
        &FONT_MAIN8OUTLINE,
};

Image *Sexy::GetImageById(ResourceId theId)
{
    return *(Image **)gResources[(int)theId];
}

Font *Sexy::GetFontById(ResourceId theId)
{
    return *(Font **)gResources[(int)theId];
}

int Sexy::GetSoundById(ResourceId theId)
{
    return *(int *)gResources[(int)theId];
}

bool Sexy::ExtractResourcesByName(ResourceManager *theMgr, const char *name)
{
    if (!strcmp(name, "AdventureScreen"))
        return Sexy::ExtractAdventureScreenResources(theMgr);
    if (!strcmp(name, "GauntletScreen"))
        return Sexy::ExtractGauntletScreenResources(theMgr);
    if (!strcmp(name, "HelpScreen"))
        return Sexy::ExtractHelpScreenResources(theMgr);
    if (!strcmp(name, "Init"))
        return Sexy::ExtractInitResources(theMgr);
    if (!strcmp(name, "LoadingThread"))
        return Sexy::ExtractLoadingThreadResources(theMgr);
    if (!strcmp(name, "MainMenu"))
        return Sexy::ExtractMainMenuResources(theMgr);
    if (!strcmp(name, "MoreGames"))
        return Sexy::ExtractMoreGamesResources(theMgr);
    if (!strcmp(name, "Register"))
        return Sexy::ExtractRegisterResources(theMgr);

    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Init Resources

Font *Sexy::FONT_DIALOG;
Font *Sexy::FONT_FLOAT;
Font *Sexy::FONT_MAIN10;
Font *Sexy::FONT_MAIN10OUTLINE;
Font *Sexy::FONT_MAIN10OUTLINE2;
Font *Sexy::FONT_MAIN10OUTLINE3;

Image *Sexy::IMAGE_CURSOR_DRAGGING;
Image *Sexy::IMAGE_CURSOR_HAND;
Image *Sexy::IMAGE_CURSOR_POINTER;
Image *Sexy::IMAGE_CURSOR_TEXT;

Image *Sexy::IMAGE_LOADERBAR;
Image *Sexy::IMAGE_LOADING_SCREEN;

bool Sexy::ExtractInitResources(ResourceManager *theMgr)
{
    ResourceManager &aMgr = *theMgr;
    try
    {
        FONT_DIALOG = aMgr.GetFontThrow("FONT_DIALOG");
        FONT_FLOAT = aMgr.GetFontThrow("FONT_FLOAT");
        FONT_MAIN10 = aMgr.GetFontThrow("FONT_MAIN10");
        FONT_MAIN10OUTLINE = aMgr.GetFontThrow("FONT_MAIN10OUTLINE");
        FONT_MAIN10OUTLINE2 = aMgr.GetFontThrow("FONT_MAIN10OUTLINE2");
        FONT_MAIN10OUTLINE3 = aMgr.GetFontThrow("FONT_MAIN10OUTLINE3");

        IMAGE_CURSOR_DRAGGING = aMgr.GetImageThrow("IMAGE_CURSOR_DRAGGING");
        IMAGE_CURSOR_HAND = aMgr.GetImageThrow("IMAGE_CURSOR_HAND");
        IMAGE_CURSOR_POINTER = aMgr.GetImageThrow("IMAGE_CURSOR_POINTER");
        IMAGE_CURSOR_TEXT = aMgr.GetImageThrow("IMAGE_CURSOR_TEXT");

        IMAGE_LOADERBAR = aMgr.GetImageThrow("IMAGE_LOADERBAR");
        IMAGE_LOADING_SCREEN = aMgr.GetImageThrow("IMAGE_LOADING_SCREEN");
    }
    catch (ResourceManagerException &)
    {
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Adventure Screen Resources

bool Sexy::ExtractAdventureScreenResources(ResourceManager *theMgr)
{
    ResourceManager &aMgr = *theMgr;
    try
    {
        IMAGE_ADVBACK = aMgr.GetImageThrow("IMAGE_ADVBACK");
        IMAGE_ADVDOOR1A = aMgr.GetImageThrow("IMAGE_ADVDOOR1A");
        IMAGE_ADVDOOR1B = aMgr.GetImageThrow("IMAGE_ADVDOOR1B");
        IMAGE_ADVDOOR1C = aMgr.GetImageThrow("IMAGE_ADVDOOR1C");
        IMAGE_ADVDOOR2A = aMgr.GetImageThrow("IMAGE_ADVDOOR2A");
        IMAGE_ADVDOOR2B = aMgr.GetImageThrow("IMAGE_ADVDOOR2B");
        IMAGE_ADVDOOR2C = aMgr.GetImageThrow("IMAGE_ADVDOOR2C");
        IMAGE_ADVDOOR3A = aMgr.GetImageThrow("IMAGE_ADVDOOR3A");
        IMAGE_ADVDOOR3B = aMgr.GetImageThrow("IMAGE_ADVDOOR3B");
        IMAGE_ADVDOOR3C = aMgr.GetImageThrow("IMAGE_ADVDOOR3C");
        IMAGE_ADVDOOR4A = aMgr.GetImageThrow("IMAGE_ADVDOOR4A");
        IMAGE_ADVDOOR4B = aMgr.GetImageThrow("IMAGE_ADVDOOR4B");
        IMAGE_ADVDOOR4C = aMgr.GetImageThrow("IMAGE_ADVDOOR4C");
        IMAGE_ADVHIGHSCORE = aMgr.GetImageThrow("IMAGE_ADVHIGHSCORE");
        IMAGE_ADVMAINMENUBUTTON = aMgr.GetImageThrow("IMAGE_ADVMAINMENUBUTTON");
        IMAGE_ADVMIDDLE = aMgr.GetImageThrow("IMAGE_ADVMIDDLE");
        IMAGE_ADVPLAYBUTTON = aMgr.GetImageThrow("IMAGE_ADVPLAYBUTTON");
        IMAGE_ADVSKY = aMgr.GetImageThrow("IMAGE_ADVSKY");
        IMAGE_ADVSTAGE = aMgr.GetImageThrow("IMAGE_ADVSTAGE");
        IMAGE_ADVTEMPLE1 = aMgr.GetImageThrow("IMAGE_ADVTEMPLE1");
        IMAGE_ADVTEMPLE2 = aMgr.GetImageThrow("IMAGE_ADVTEMPLE2");
        IMAGE_ADVTEMPLE2V = aMgr.GetImageThrow("IMAGE_ADVTEMPLE2V");
        IMAGE_ADVTEMPLE3 = aMgr.GetImageThrow("IMAGE_ADVTEMPLE3");
        IMAGE_ADVTEMPLE3V = aMgr.GetImageThrow("IMAGE_ADVTEMPLE3V");
        IMAGE_ADVTEMPLE4 = aMgr.GetImageThrow("IMAGE_ADVTEMPLE4");
        IMAGE_ADVTITLE = aMgr.GetImageThrow("IMAGE_ADVTITLE");
    }
    catch (ResourceManagerException &)
    {
        return false;
    }
    return true;
}

Image *Sexy::IMAGE_ADVBACK;
Image *Sexy::IMAGE_ADVDOOR1A;
Image *Sexy::IMAGE_ADVDOOR1B;
Image *Sexy::IMAGE_ADVDOOR1C;
Image *Sexy::IMAGE_ADVDOOR2A;
Image *Sexy::IMAGE_ADVDOOR2B;
Image *Sexy::IMAGE_ADVDOOR2C;
Image *Sexy::IMAGE_ADVDOOR3A;
Image *Sexy::IMAGE_ADVDOOR3B;
Image *Sexy::IMAGE_ADVDOOR3C;
Image *Sexy::IMAGE_ADVDOOR4A;
Image *Sexy::IMAGE_ADVDOOR4B;
Image *Sexy::IMAGE_ADVDOOR4C;
Image *Sexy::IMAGE_ADVHIGHSCORE;
Image *Sexy::IMAGE_ADVMAINMENUBUTTON;
Image *Sexy::IMAGE_ADVMIDDLE;
Image *Sexy::IMAGE_ADVPLAYBUTTON;
Image *Sexy::IMAGE_ADVSKY;
Image *Sexy::IMAGE_ADVSTAGE;
Image *Sexy::IMAGE_ADVTEMPLE1;
Image *Sexy::IMAGE_ADVTEMPLE2;
Image *Sexy::IMAGE_ADVTEMPLE2V;
Image *Sexy::IMAGE_ADVTEMPLE3;
Image *Sexy::IMAGE_ADVTEMPLE3V;
Image *Sexy::IMAGE_ADVTEMPLE4;
Image *Sexy::IMAGE_ADVTITLE;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Gauntlet Screen Resources

bool Sexy::ExtractGauntletScreenResources(ResourceManager *theMgr)
{
    ResourceManager &aMgr = *theMgr;
    try
    {
        IMAGE_GAUNTBACKBUTTON = aMgr.GetImageThrow("IMAGE_GAUNTBACKBUTTON");
        IMAGE_GAUNTDOOR1 = aMgr.GetImageThrow("IMAGE_GAUNTDOOR1");
        IMAGE_GAUNTDOOR2 = aMgr.GetImageThrow("IMAGE_GAUNTDOOR2");
        IMAGE_GAUNTDOOR3 = aMgr.GetImageThrow("IMAGE_GAUNTDOOR3");
        IMAGE_GAUNTDOOR4 = aMgr.GetImageThrow("IMAGE_GAUNTDOOR4");
        IMAGE_GAUNTEAGLEGEM = aMgr.GetImageThrow("IMAGE_GAUNTEAGLEGEM");
        IMAGE_GAUNTHIGHSCORE = aMgr.GetImageThrow("IMAGE_GAUNTHIGHSCORE");
        IMAGE_GAUNTJAGUARGEM = aMgr.GetImageThrow("IMAGE_GAUNTJAGUARGEM");
        IMAGE_GAUNTMAINMENUBUTTON = aMgr.GetImageThrow("IMAGE_GAUNTMAINMENUBUTTON");
        IMAGE_GAUNTNEXTBUTTON = aMgr.GetImageThrow("IMAGE_GAUNTNEXTBUTTON");
        IMAGE_GAUNTPLAYBUTTON = aMgr.GetImageThrow("IMAGE_GAUNTPLAYBUTTON");
        IMAGE_GAUNTPRACTICEBUTTON = aMgr.GetImageThrow("IMAGE_GAUNTPRACTICEBUTTON");
        IMAGE_GAUNTSCREEN = aMgr.GetImageThrow("IMAGE_GAUNTSCREEN");
        IMAGE_GAUNTSKY = aMgr.GetImageThrow("IMAGE_GAUNTSKY");
        IMAGE_GAUNTSUNGEM = aMgr.GetImageThrow("IMAGE_GAUNTSUNGEM");
        IMAGE_GAUNTSURVIVALBUTTON = aMgr.GetImageThrow("IMAGE_GAUNTSURVIVALBUTTON");
        IMAGE_GAUNTTITLE = aMgr.GetImageThrow("IMAGE_GAUNTTITLE");
    }
    catch (ResourceManagerException &)
    {
        return false;
    }
    return true;
}

Image *Sexy::IMAGE_GAUNTBACKBUTTON;
Image *Sexy::IMAGE_GAUNTDOOR1;
Image *Sexy::IMAGE_GAUNTDOOR2;
Image *Sexy::IMAGE_GAUNTDOOR3;
Image *Sexy::IMAGE_GAUNTDOOR4;
Image *Sexy::IMAGE_GAUNTEAGLEGEM;
Image *Sexy::IMAGE_GAUNTHIGHSCORE;
Image *Sexy::IMAGE_GAUNTJAGUARGEM;
Image *Sexy::IMAGE_GAUNTMAINMENUBUTTON;
Image *Sexy::IMAGE_GAUNTNEXTBUTTON;
Image *Sexy::IMAGE_GAUNTPLAYBUTTON;
Image *Sexy::IMAGE_GAUNTPRACTICEBUTTON;
Image *Sexy::IMAGE_GAUNTSCREEN;
Image *Sexy::IMAGE_GAUNTSKY;
Image *Sexy::IMAGE_GAUNTSUNGEM;
Image *Sexy::IMAGE_GAUNTSURVIVALBUTTON;
Image *Sexy::IMAGE_GAUNTTITLE;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Help Screen Resources

bool Sexy::ExtractHelpScreenResources(ResourceManager *theMgr)
{
    ResourceManager &aMgr = *theMgr;
    try
    {
        IMAGE_HELP_BACK = aMgr.GetImageThrow("IMAGE_HELP_BACK");
        IMAGE_HELP_FRONT = aMgr.GetImageThrow("IMAGE_HELP_FRONT");
    }
    catch (ResourceManagerException &)
    {
        return false;
    }
    return true;
}

Image *Sexy::IMAGE_HELP_BACK;
Image *Sexy::IMAGE_HELP_FRONT;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Main Menu Resources

bool Sexy::ExtractMainMenuResources(ResourceManager *theMgr)
{
    ResourceManager &aMgr = *theMgr;
    try
    {
        IMAGE_MM_ARCADE = aMgr.GetImageThrow("IMAGE_MM_ARCADE");
        IMAGE_MM_BACK = aMgr.GetImageThrow("IMAGE_MM_BACK");
        IMAGE_MM_EYELEFT = aMgr.GetImageThrow("IMAGE_MM_EYELEFT");
        IMAGE_MM_EYEMASK = aMgr.GetImageThrow("IMAGE_MM_EYEMASK");
        IMAGE_MM_EYERIGHT = aMgr.GetImageThrow("IMAGE_MM_EYERIGHT");
        IMAGE_MM_GAUNTLET = aMgr.GetImageThrow("IMAGE_MM_GAUNTLET");
        IMAGE_MM_HAT = aMgr.GetImageThrow("IMAGE_MM_HAT");
        IMAGE_MM_MOREGAMES = aMgr.GetImageThrow("IMAGE_MM_MOREGAMES");
        IMAGE_MM_OPTIONS = aMgr.GetImageThrow("IMAGE_MM_OPTIONS");
        IMAGE_MM_QUIT = aMgr.GetImageThrow("IMAGE_MM_QUIT");
        IMAGE_MM_SKY = aMgr.GetImageThrow("IMAGE_MM_SKY");
        IMAGE_MM_SUN = aMgr.GetImageThrow("IMAGE_MM_SUN");
    }
    catch (ResourceManagerException &)
    {
        return false;
    }
    return true;
}

Image *Sexy::IMAGE_MM_ARCADE;
Image *Sexy::IMAGE_MM_BACK;
Image *Sexy::IMAGE_MM_EYELEFT;
Image *Sexy::IMAGE_MM_EYEMASK;
Image *Sexy::IMAGE_MM_EYERIGHT;
Image *Sexy::IMAGE_MM_GAUNTLET;
Image *Sexy::IMAGE_MM_HAT;
Image *Sexy::IMAGE_MM_MOREGAMES;
Image *Sexy::IMAGE_MM_OPTIONS;
Image *Sexy::IMAGE_MM_QUIT;
Image *Sexy::IMAGE_MM_SKY;
Image *Sexy::IMAGE_MM_SUN;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// More Games Resources

bool Sexy::ExtractMoreGamesResources(ResourceManager *theMgr)
{
    ResourceManager &aMgr = *theMgr;
    try
    {
        IMAGE_MOREGAMES_SCREEN = aMgr.GetImageThrow("IMAGE_MOREGAMES_SCREEN");
        IMAGE_MOREGAMES_BUTTON = aMgr.GetImageThrow("IMAGE_MOREGAMES_BUTTON");
    }
    catch (ResourceManagerException &)
    {
        return false;
    }
    return true;
}

Image *Sexy::IMAGE_MOREGAMES_SCREEN;
Image *Sexy::IMAGE_MOREGAMES_BUTTON;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Register Resources

bool Sexy::ExtractRegisterResources(ResourceManager *theMgr)
{
    ResourceManager &aMgr = *theMgr;
    try
    {
        FONT_PLAIN = aMgr.GetFontThrow("FONT_PLAIN");
        FONT_TITLE = aMgr.GetFontThrow("FONT_TITLE");
        IMAGE_DIALOG_BACK = aMgr.GetImageThrow("IMAGE_DIALOG_BACK");
        IMAGE_DIALOG_BUTTON = aMgr.GetImageThrow("IMAGE_DIALOG_BUTTON");
        SOUND_BUTTON1 = aMgr.GetSoundThrow("SOUND_BUTTON1");
    }
    catch (ResourceManagerException &)
    {
        return false;
    }
    return true;
}

Font *Sexy::FONT_PLAIN;
Font *Sexy::FONT_TITLE;
Image *Sexy::IMAGE_DIALOG_BACK;
Image *Sexy::IMAGE_DIALOG_BUTTON;
int Sexy::SOUND_BUTTON1;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Loading Thread Resources
bool Sexy::ExtractLoadingThreadResources(ResourceManager *theMgr)
{
    ResourceManager &aMgr = *theMgr;
    try
    {
        FONT_HUGE = aMgr.GetFontThrow("FONT_HUGE");
        FONT_MONODIGIT = aMgr.GetFontThrow("FONT_MONODIGIT");
        FONT_BROWNTITLE = aMgr.GetFontThrow("FONT_BROWNTITLE");
        FONT_MAIN12 = aMgr.GetFontThrow("FONT_MAIN12");
        FONT_MAIN8 = aMgr.GetFontThrow("FONT_MAIN8");
        FONT_MAIN8OUTLINE = aMgr.GetFontThrow("FONT_MAIN8OUTLINE");
        IMAGE_SUNGLOW = aMgr.GetImageThrow("IMAGE_SUNGLOW");
        IMAGE_BLUE_BALL = aMgr.GetImageThrow("IMAGE_BLUE_BALL");
        IMAGE_YELLOW_BALL = aMgr.GetImageThrow("IMAGE_YELLOW_BALL");
        IMAGE_RED_BALL = aMgr.GetImageThrow("IMAGE_RED_BALL");
        IMAGE_GREEN_BALL = aMgr.GetImageThrow("IMAGE_GREEN_BALL");
        IMAGE_PURPLE_BALL = aMgr.GetImageThrow("IMAGE_PURPLE_BALL");
        IMAGE_WHITE_BALL = aMgr.GetImageThrow("IMAGE_WHITE_BALL");
        IMAGE_NEXT_BALL = aMgr.GetImageThrow("IMAGE_NEXT_BALL");
        IMAGE_BALL_ALPHA = aMgr.GetImageThrow("IMAGE_BALL_ALPHA");
        IMAGE_BALL_SHADOW = aMgr.GetImageThrow("IMAGE_BALL_SHADOW");
        IMAGE_BLUE_ACCURACY = aMgr.GetImageThrow("IMAGE_BLUE_ACCURACY");
        IMAGE_YELLOW_ACCURACY = aMgr.GetImageThrow("IMAGE_YELLOW_ACCURACY");
        IMAGE_RED_ACCURACY = aMgr.GetImageThrow("IMAGE_RED_ACCURACY");
        IMAGE_GREEN_ACCURACY = aMgr.GetImageThrow("IMAGE_GREEN_ACCURACY");
        IMAGE_PURPLE_ACCURACY = aMgr.GetImageThrow("IMAGE_PURPLE_ACCURACY");
        IMAGE_WHITE_ACCURACY = aMgr.GetImageThrow("IMAGE_WHITE_ACCURACY");
        IMAGE_BLUE_BACKWARDS = aMgr.GetImageThrow("IMAGE_BLUE_BACKWARDS");
        IMAGE_YELLOW_BACKWARDS = aMgr.GetImageThrow("IMAGE_YELLOW_BACKWARDS");
        IMAGE_RED_BACKWARDS = aMgr.GetImageThrow("IMAGE_RED_BACKWARDS");
        IMAGE_GREEN_BACKWARDS = aMgr.GetImageThrow("IMAGE_GREEN_BACKWARDS");
        IMAGE_PURPLE_BACKWARDS = aMgr.GetImageThrow("IMAGE_PURPLE_BACKWARDS");
        IMAGE_WHITE_BACKWARDS = aMgr.GetImageThrow("IMAGE_WHITE_BACKWARDS");
        IMAGE_BLUE_SLOW = aMgr.GetImageThrow("IMAGE_BLUE_SLOW");
        IMAGE_YELLOW_SLOW = aMgr.GetImageThrow("IMAGE_YELLOW_SLOW");
        IMAGE_RED_SLOW = aMgr.GetImageThrow("IMAGE_RED_SLOW");
        IMAGE_GREEN_SLOW = aMgr.GetImageThrow("IMAGE_GREEN_SLOW");
        IMAGE_PURPLE_SLOW = aMgr.GetImageThrow("IMAGE_PURPLE_SLOW");
        IMAGE_WHITE_SLOW = aMgr.GetImageThrow("IMAGE_WHITE_SLOW");
        IMAGE_GRAY_EXPLOSION = aMgr.GetImageThrow("IMAGE_GRAY_EXPLOSION");
        IMAGE_EXPLOSION = aMgr.GetImageThrow("IMAGE_EXPLOSION");
        IMAGE_BLUE_BOMB = aMgr.GetImageThrow("IMAGE_BLUE_BOMB");
        IMAGE_YELLOW_BOMB = aMgr.GetImageThrow("IMAGE_YELLOW_BOMB");
        IMAGE_RED_BOMB = aMgr.GetImageThrow("IMAGE_RED_BOMB");
        IMAGE_GREEN_BOMB = aMgr.GetImageThrow("IMAGE_GREEN_BOMB");
        IMAGE_PURPLE_BOMB = aMgr.GetImageThrow("IMAGE_PURPLE_BOMB");
        IMAGE_WHITE_BOMB = aMgr.GetImageThrow("IMAGE_WHITE_BOMB");
        IMAGE_BLUE_LIGHT = aMgr.GetImageThrow("IMAGE_BLUE_LIGHT");
        IMAGE_YELLOW_LIGHT = aMgr.GetImageThrow("IMAGE_YELLOW_LIGHT");
        IMAGE_RED_LIGHT = aMgr.GetImageThrow("IMAGE_RED_LIGHT");
        IMAGE_GREEN_LIGHT = aMgr.GetImageThrow("IMAGE_GREEN_LIGHT");
        IMAGE_PURPLE_LIGHT = aMgr.GetImageThrow("IMAGE_PURPLE_LIGHT");
        IMAGE_WHITE_LIGHT = aMgr.GetImageThrow("IMAGE_WHITE_LIGHT");
        IMAGE_BACKWARDS_LIGHT = aMgr.GetImageThrow("IMAGE_BACKWARDS_LIGHT");
        IMAGE_ACCURACY_LIGHT = aMgr.GetImageThrow("IMAGE_ACCURACY_LIGHT");
        IMAGE_SLOW_LIGHT = aMgr.GetImageThrow("IMAGE_SLOW_LIGHT");
        IMAGE_SPARKLE = aMgr.GetImageThrow("IMAGE_SPARKLE");
        IMAGE_SHOOTER_BOTTOM = aMgr.GetImageThrow("IMAGE_SHOOTER_BOTTOM");
        IMAGE_SHOOTER_TOP = aMgr.GetImageThrow("IMAGE_SHOOTER_TOP");
        IMAGE_SHOOTER_TONGUE = aMgr.GetImageThrow("IMAGE_SHOOTER_TONGUE");
        IMAGE_EYE_BLINK = aMgr.GetImageThrow("IMAGE_EYE_BLINK");
        IMAGE_DIALOG_CHECKBOX = aMgr.GetImageThrow("IMAGE_DIALOG_CHECKBOX");
        IMAGE_DIALOG_CHECKBOXCAP = aMgr.GetImageThrow("IMAGE_DIALOG_CHECKBOXCAP");
        IMAGE_DIALOG_CHECKBOXLINE = aMgr.GetImageThrow("IMAGE_DIALOG_CHECKBOXLINE");
        IMAGE_SLIDER_TRACK = aMgr.GetImageThrow("IMAGE_SLIDER_TRACK");
        IMAGE_SLIDER_THUMB = aMgr.GetImageThrow("IMAGE_SLIDER_THUMB");
        IMAGE_UPDATE_BAR = aMgr.GetImageThrow("IMAGE_UPDATE_BAR");
        IMAGE_GOD_HEAD = aMgr.GetImageThrow("IMAGE_GOD_HEAD");
        IMAGE_HOLE = aMgr.GetImageThrow("IMAGE_HOLE");
        IMAGE_HOLE_COVER = aMgr.GetImageThrow("IMAGE_HOLE_COVER");
        IMAGE_COIN = aMgr.GetImageThrow("IMAGE_COIN");
        IMAGE_RIGHT_MOUSE = aMgr.GetImageThrow("IMAGE_RIGHT_MOUSE");
        IMAGE_MENU_BAR = aMgr.GetImageThrow("IMAGE_MENU_BAR");
        IMAGE_MENU_BUTTON = aMgr.GetImageThrow("IMAGE_MENU_BUTTON");
        IMAGE_ZUMA_BAR = aMgr.GetImageThrow("IMAGE_ZUMA_BAR");
        IMAGE_ZUMA_BAR_DONE = aMgr.GetImageThrow("IMAGE_ZUMA_BAR_DONE");
        IMAGE_FROG_LIVES = aMgr.GetImageThrow("IMAGE_FROG_LIVES");
        SOUND_BALLCLICK1 = aMgr.GetSoundThrow("SOUND_BALLCLICK1");
        SOUND_BALLCLICK2 = aMgr.GetSoundThrow("SOUND_BALLCLICK2");
        SOUND_BALLFIRE = aMgr.GetSoundThrow("SOUND_BALLFIRE");
        SOUND_WARNING = aMgr.GetSoundThrow("SOUND_WARNING");
        SOUND_EXPLODE = aMgr.GetSoundThrow("SOUND_EXPLODE");
        SOUND_EARTHQUAKE = aMgr.GetSoundThrow("SOUND_EARTHQUAKE");
        SOUND_FROGLAND = aMgr.GetSoundThrow("SOUND_FROGLAND");
        SOUND_JEWEL_APPEAR = aMgr.GetSoundThrow("SOUND_JEWEL_APPEAR");
        SOUND_JEWEL_DISAPPEAR = aMgr.GetSoundThrow("SOUND_JEWEL_DISAPPEAR");
        SOUND_JEWEL_HIT = aMgr.GetSoundThrow("SOUND_JEWEL_HIT");
        SOUND_CHAIN_BONUS = aMgr.GetSoundThrow("SOUND_CHAIN_BONUS");
        SOUND_GAP_BONUS = aMgr.GetSoundThrow("SOUND_GAP_BONUS");
        SOUND_ROLLING = aMgr.GetSoundThrow("SOUND_ROLLING");
        SOUND_EXTRA_LIFE = aMgr.GetSoundThrow("SOUND_EXTRA_LIFE");
        SOUND_SLOWDOWN_BALL = aMgr.GetSoundThrow("SOUND_SLOWDOWN_BALL");
        SOUND_BACKWARDS_BALL = aMgr.GetSoundThrow("SOUND_BACKWARDS_BALL");
        SOUND_ACCURACY_BALL = aMgr.GetSoundThrow("SOUND_ACCURACY_BALL");
        SOUND_TRAIL_LIGHT = aMgr.GetSoundThrow("SOUND_TRAIL_LIGHT");
        SOUND_TRAIL_LIGHT_END = aMgr.GetSoundThrow("SOUND_TRAIL_LIGHT_END");
        SOUND_GAME_START = aMgr.GetSoundThrow("SOUND_GAME_START");
        SOUND_LEVEL_UP = aMgr.GetSoundThrow("SOUND_LEVEL_UP");
        SOUND_BAR_FULL = aMgr.GetSoundThrow("SOUND_BAR_FULL");
        SOUND_LOST_LIFE = aMgr.GetSoundThrow("SOUND_LOST_LIFE");
        SOUND_GAME_OVER = aMgr.GetSoundThrow("SOUND_GAME_OVER");
        SOUND_STAGE_COMPLETE = aMgr.GetSoundThrow("SOUND_STAGE_COMPLETE");
        SOUND_TEMPLE_COMPLETE = aMgr.GetSoundThrow("SOUND_TEMPLE_COMPLETE");
        SOUND_BONUS_EXPLOSION = aMgr.GetSoundThrow("SOUND_BONUS_EXPLOSION");
        SOUND_BALLDESTROYED1 = aMgr.GetSoundThrow("SOUND_BALLDESTROYED1");
        SOUND_BALLDESTROYED2 = aMgr.GetSoundThrow("SOUND_BALLDESTROYED2");
        SOUND_BALLDESTROYED3 = aMgr.GetSoundThrow("SOUND_BALLDESTROYED3");
        SOUND_BALLDESTROYED4 = aMgr.GetSoundThrow("SOUND_BALLDESTROYED4");
        SOUND_BALLDESTROYED5 = aMgr.GetSoundThrow("SOUND_BALLDESTROYED5");
        SOUND_COMBO = aMgr.GetSoundThrow("SOUND_COMBO");
        SOUND_BUTTON2 = aMgr.GetSoundThrow("SOUND_BUTTON2");
        SOUND_CHORAL1 = aMgr.GetSoundThrow("SOUND_CHORAL1");
        SOUND_POP = aMgr.GetSoundThrow("SOUND_POP");
        SOUND_UFO = aMgr.GetSoundThrow("SOUND_UFO");
    }
    catch (ResourceManagerException &)
    {
        return false;
    }
    return true;
}

Font *Sexy::FONT_HUGE;
Font *Sexy::FONT_MONODIGIT;
Font *Sexy::FONT_BROWNTITLE;
Font *Sexy::FONT_MAIN12;
Font *Sexy::FONT_MAIN8;
Font *Sexy::FONT_MAIN8OUTLINE;
Image *Sexy::IMAGE_SUNGLOW;
Image *Sexy::IMAGE_BLUE_BALL;
Image *Sexy::IMAGE_YELLOW_BALL;
Image *Sexy::IMAGE_RED_BALL;
Image *Sexy::IMAGE_GREEN_BALL;
Image *Sexy::IMAGE_PURPLE_BALL;
Image *Sexy::IMAGE_WHITE_BALL;
Image *Sexy::IMAGE_NEXT_BALL;
Image *Sexy::IMAGE_BALL_ALPHA;
Image *Sexy::IMAGE_BALL_SHADOW;
Image *Sexy::IMAGE_BLUE_ACCURACY;
Image *Sexy::IMAGE_YELLOW_ACCURACY;
Image *Sexy::IMAGE_RED_ACCURACY;
Image *Sexy::IMAGE_GREEN_ACCURACY;
Image *Sexy::IMAGE_PURPLE_ACCURACY;
Image *Sexy::IMAGE_WHITE_ACCURACY;
Image *Sexy::IMAGE_BLUE_BACKWARDS;
Image *Sexy::IMAGE_YELLOW_BACKWARDS;
Image *Sexy::IMAGE_RED_BACKWARDS;
Image *Sexy::IMAGE_GREEN_BACKWARDS;
Image *Sexy::IMAGE_PURPLE_BACKWARDS;
Image *Sexy::IMAGE_WHITE_BACKWARDS;
Image *Sexy::IMAGE_BLUE_SLOW;
Image *Sexy::IMAGE_YELLOW_SLOW;
Image *Sexy::IMAGE_RED_SLOW;
Image *Sexy::IMAGE_GREEN_SLOW;
Image *Sexy::IMAGE_PURPLE_SLOW;
Image *Sexy::IMAGE_WHITE_SLOW;
Image *Sexy::IMAGE_GRAY_EXPLOSION;
Image *Sexy::IMAGE_EXPLOSION;
Image *Sexy::IMAGE_BLUE_BOMB;
Image *Sexy::IMAGE_YELLOW_BOMB;
Image *Sexy::IMAGE_RED_BOMB;
Image *Sexy::IMAGE_GREEN_BOMB;
Image *Sexy::IMAGE_PURPLE_BOMB;
Image *Sexy::IMAGE_WHITE_BOMB;
Image *Sexy::IMAGE_BLUE_LIGHT;
Image *Sexy::IMAGE_YELLOW_LIGHT;
Image *Sexy::IMAGE_RED_LIGHT;
Image *Sexy::IMAGE_GREEN_LIGHT;
Image *Sexy::IMAGE_PURPLE_LIGHT;
Image *Sexy::IMAGE_WHITE_LIGHT;
Image *Sexy::IMAGE_BACKWARDS_LIGHT;
Image *Sexy::IMAGE_ACCURACY_LIGHT;
Image *Sexy::IMAGE_SLOW_LIGHT;
Image *Sexy::IMAGE_SPARKLE;
Image *Sexy::IMAGE_SHOOTER_BOTTOM;
Image *Sexy::IMAGE_SHOOTER_TOP;
Image *Sexy::IMAGE_SHOOTER_TONGUE;
Image *Sexy::IMAGE_EYE_BLINK;
Image *Sexy::IMAGE_DIALOG_CHECKBOX;
Image *Sexy::IMAGE_DIALOG_CHECKBOXCAP;
Image *Sexy::IMAGE_DIALOG_CHECKBOXLINE;
Image *Sexy::IMAGE_SLIDER_TRACK;
Image *Sexy::IMAGE_SLIDER_THUMB;
Image *Sexy::IMAGE_UPDATE_BAR;
Image *Sexy::IMAGE_GOD_HEAD;
Image *Sexy::IMAGE_HOLE;
Image *Sexy::IMAGE_HOLE_COVER;
Image *Sexy::IMAGE_COIN;
Image *Sexy::IMAGE_RIGHT_MOUSE;
Image *Sexy::IMAGE_MENU_BAR;
Image *Sexy::IMAGE_MENU_BUTTON;
Image *Sexy::IMAGE_ZUMA_BAR;
Image *Sexy::IMAGE_ZUMA_BAR_DONE;
Image *Sexy::IMAGE_FROG_LIVES;
int Sexy::SOUND_BALLCLICK1;
int Sexy::SOUND_BALLCLICK2;
int Sexy::SOUND_BALLFIRE;
int Sexy::SOUND_WARNING;
int Sexy::SOUND_EXPLODE;
int Sexy::SOUND_EARTHQUAKE;
int Sexy::SOUND_FROGLAND;
int Sexy::SOUND_JEWEL_APPEAR;
int Sexy::SOUND_JEWEL_DISAPPEAR;
int Sexy::SOUND_JEWEL_HIT;
int Sexy::SOUND_CHAIN_BONUS;
int Sexy::SOUND_GAP_BONUS;
int Sexy::SOUND_ROLLING;
int Sexy::SOUND_EXTRA_LIFE;
int Sexy::SOUND_SLOWDOWN_BALL;
int Sexy::SOUND_BACKWARDS_BALL;
int Sexy::SOUND_ACCURACY_BALL;
int Sexy::SOUND_TRAIL_LIGHT;
int Sexy::SOUND_TRAIL_LIGHT_END;
int Sexy::SOUND_GAME_START;
int Sexy::SOUND_LEVEL_UP;
int Sexy::SOUND_BAR_FULL;
int Sexy::SOUND_LOST_LIFE;
int Sexy::SOUND_GAME_OVER;
int Sexy::SOUND_STAGE_COMPLETE;
int Sexy::SOUND_TEMPLE_COMPLETE;
int Sexy::SOUND_BONUS_EXPLOSION;
int Sexy::SOUND_BALLDESTROYED1;
int Sexy::SOUND_BALLDESTROYED2;
int Sexy::SOUND_BALLDESTROYED3;
int Sexy::SOUND_BALLDESTROYED4;
int Sexy::SOUND_BALLDESTROYED5;
int Sexy::SOUND_COMBO;
int Sexy::SOUND_BUTTON2;
int Sexy::SOUND_CHORAL1;
int Sexy::SOUND_POP;
int Sexy::SOUND_UFO;