#ifndef __RES_H__
#define __RES_H__

namespace Sexy
{
    class ResourceManager;
	class Image;
	class Font;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

	enum ResourceId
	{
        IMAGE_LOADING_SCREEN_ID,
        IMAGE_LOADERBAR_ID,
        IMAGE_CURSOR_POINTER_ID,
        IMAGE_CURSOR_HAND_ID,
        IMAGE_CURSOR_DRAGGING_ID,
        IMAGE_CURSOR_TEXT_ID,
        FONT_MAIN10_ID,
        FONT_DIALOG_ID,
        FONT_MAIN10OUTLINE_ID,
        FONT_MAIN10OUTLINE2_ID,
        FONT_MAIN10OUTLINE3_ID,
        FONT_FLOAT_ID,
        IMAGE_DIALOG_BUTTON_ID,
        IMAGE_DIALOG_BACK_ID,
        FONT_TITLE_ID,
        FONT_PLAIN_ID,
        SOUND_BUTTON1_ID,
        IMAGE_MOREGAMES_SCREEN_ID,
        IMAGE_MOREGAMES_BUTTON_ID,
        IMAGE_HELP_FRONT_ID,
        IMAGE_HELP_BACK_ID,
        IMAGE_MM_BACK_ID,
        IMAGE_MM_GAUNTLET_ID,
        IMAGE_MM_ARCADE_ID,
        IMAGE_MM_MOREGAMES_ID,
        IMAGE_MM_OPTIONS_ID,
        IMAGE_MM_QUIT_ID,
        IMAGE_MM_EYELEFT_ID,
        IMAGE_MM_EYERIGHT_ID,
        IMAGE_MM_EYEMASK_ID,
        IMAGE_MM_SUN_ID,
        IMAGE_MM_SKY_ID,
        IMAGE_MM_HAT_ID,
        IMAGE_ADVSKY_ID,
        IMAGE_ADVBACK_ID,
        IMAGE_ADVMIDDLE_ID,
        IMAGE_ADVTEMPLE1_ID,
        IMAGE_ADVTEMPLE2_ID,
        IMAGE_ADVTEMPLE3_ID,
        IMAGE_ADVTEMPLE4_ID,
        IMAGE_ADVTEMPLE2V_ID,
        IMAGE_ADVTEMPLE3V_ID,
        IMAGE_ADVDOOR1A_ID,
        IMAGE_ADVDOOR1B_ID,
        IMAGE_ADVDOOR1C_ID,
        IMAGE_ADVDOOR2A_ID,
        IMAGE_ADVDOOR2B_ID,
        IMAGE_ADVDOOR2C_ID,
        IMAGE_ADVDOOR3A_ID,
        IMAGE_ADVDOOR3B_ID,
        IMAGE_ADVDOOR3C_ID,
        IMAGE_ADVDOOR4A_ID,
        IMAGE_ADVDOOR4B_ID,
        IMAGE_ADVDOOR4C_ID,
        IMAGE_ADVMAINMENUBUTTON_ID,
        IMAGE_ADVPLAYBUTTON_ID,
        IMAGE_ADVSTAGE_ID,
        IMAGE_ADVTITLE_ID,
        IMAGE_ADVHIGHSCORE_ID,
        IMAGE_GAUNTSCREEN_ID,
        IMAGE_GAUNTSKY_ID,
        IMAGE_GAUNTDOOR1_ID,
        IMAGE_GAUNTDOOR2_ID,
        IMAGE_GAUNTDOOR3_ID,
        IMAGE_GAUNTDOOR4_ID,
        IMAGE_GAUNTBACKBUTTON_ID,
        IMAGE_GAUNTMAINMENUBUTTON_ID,
        IMAGE_GAUNTNEXTBUTTON_ID,
        IMAGE_GAUNTPLAYBUTTON_ID,
        IMAGE_GAUNTEAGLEGEM_ID,
        IMAGE_GAUNTJAGUARGEM_ID,
        IMAGE_GAUNTSUNGEM_ID,
        IMAGE_GAUNTTITLE_ID,
        IMAGE_GAUNTHIGHSCORE_ID,
        IMAGE_GAUNTPRACTICEBUTTON_ID,
        IMAGE_GAUNTSURVIVALBUTTON_ID,
        IMAGE_SUNGLOW_ID,
        IMAGE_BLUE_BALL_ID,
        IMAGE_YELLOW_BALL_ID,
        IMAGE_RED_BALL_ID,
        IMAGE_GREEN_BALL_ID,
        IMAGE_PURPLE_BALL_ID,
        IMAGE_WHITE_BALL_ID,
        IMAGE_NEXT_BALL_ID,
        IMAGE_BALL_ALPHA_ID,
        IMAGE_BALL_SHADOW_ID,
        IMAGE_BLUE_ACCURACY_ID,
        IMAGE_YELLOW_ACCURACY_ID,
        IMAGE_RED_ACCURACY_ID,
        IMAGE_GREEN_ACCURACY_ID,
        IMAGE_PURPLE_ACCURACY_ID,
        IMAGE_WHITE_ACCURACY_ID,
        IMAGE_BLUE_BACKWARDS_ID,
        IMAGE_YELLOW_BACKWARDS_ID,
        IMAGE_RED_BACKWARDS_ID,
        IMAGE_GREEN_BACKWARDS_ID,
        IMAGE_PURPLE_BACKWARDS_ID,
        IMAGE_WHITE_BACKWARDS_ID,
        IMAGE_BLUE_SLOW_ID,
        IMAGE_YELLOW_SLOW_ID,
        IMAGE_RED_SLOW_ID,
        IMAGE_GREEN_SLOW_ID,
        IMAGE_PURPLE_SLOW_ID,
        IMAGE_WHITE_SLOW_ID,
        IMAGE_GRAY_EXPLOSION_ID,
        IMAGE_EXPLOSION_ID,
        IMAGE_BLUE_BOMB_ID,
        IMAGE_YELLOW_BOMB_ID,
        IMAGE_RED_BOMB_ID,
        IMAGE_GREEN_BOMB_ID,
        IMAGE_PURPLE_BOMB_ID,
        IMAGE_WHITE_BOMB_ID,
        IMAGE_BLUE_LIGHT_ID,
        IMAGE_YELLOW_LIGHT_ID,
        IMAGE_RED_LIGHT_ID,
        IMAGE_GREEN_LIGHT_ID,
        IMAGE_PURPLE_LIGHT_ID,
        IMAGE_WHITE_LIGHT_ID,
        IMAGE_BACKWARDS_LIGHT_ID,
        IMAGE_ACCURACY_LIGHT_ID,
        IMAGE_SLOW_LIGHT_ID,
        IMAGE_SPARKLE_ID,
        IMAGE_SHOOTER_BOTTOM_ID,
        IMAGE_SHOOTER_TOP_ID,
        IMAGE_SHOOTER_TONGUE_ID,
        IMAGE_EYE_BLINK_ID,
        IMAGE_DIALOG_CHECKBOX_ID,
        IMAGE_DIALOG_CHECKBOXCAP_ID,
        IMAGE_DIALOG_CHECKBOXLINE_ID,
        IMAGE_SLIDER_TRACK_ID,
        IMAGE_SLIDER_THUMB_ID,
        IMAGE_UPDATE_BAR_ID,
        IMAGE_GOD_HEAD_ID,
        IMAGE_HOLE_ID,
        IMAGE_HOLE_COVER_ID,
        IMAGE_COIN_ID,
        IMAGE_RIGHT_MOUSE_ID,
        IMAGE_MENU_BAR_ID,
        IMAGE_MENU_BUTTON_ID,
        IMAGE_ZUMA_BAR_ID,
        IMAGE_ZUMA_BAR_DONE_ID,
        IMAGE_FROG_LIVES_ID,
        SOUND_BALLCLICK1_ID,
        SOUND_BALLCLICK2_ID,
        SOUND_BALLFIRE_ID,
        SOUND_WARNING_ID,
        SOUND_EXPLODE_ID,
        SOUND_EARTHQUAKE_ID,
        SOUND_FROGLAND_ID,
        SOUND_JEWEL_APPEAR_ID,
        SOUND_JEWEL_DISAPPEAR_ID,
        SOUND_JEWEL_HIT_ID,
        SOUND_CHAIN_BONUS_ID,
        SOUND_GAP_BONUS_ID,
        SOUND_ROLLING_ID,
        SOUND_EXTRA_LIFE_ID,
        SOUND_SLOWDOWN_BALL_ID,
        SOUND_BACKWARDS_BALL_ID,
        SOUND_ACCURACY_BALL_ID,
        SOUND_TRAIL_LIGHT_ID,
        SOUND_TRAIL_LIGHT_END_ID,
        SOUND_GAME_START_ID,
        SOUND_LEVEL_UP_ID,
        SOUND_BAR_FULL_ID,
        SOUND_LOST_LIFE_ID,
        SOUND_GAME_OVER_ID,
        SOUND_STAGE_COMPLETE_ID,
        SOUND_TEMPLE_COMPLETE_ID,
        SOUND_BONUS_EXPLOSION_ID,
        SOUND_BALLDESTROYED1_ID,
        SOUND_BALLDESTROYED2_ID,
        SOUND_BALLDESTROYED3_ID,
        SOUND_BALLDESTROYED4_ID,
        SOUND_BALLDESTROYED5_ID,
        SOUND_COMBO_ID,
        SOUND_BUTTON2_ID,
        SOUND_CHORAL1_ID,
        SOUND_POP_ID,
        SOUND_UFO_ID,
        FONT_HUGE_ID,
        FONT_MONODIGIT_ID,
        FONT_BROWNTITLE_ID,
        FONT_MAIN12_ID,
        FONT_MAIN8_ID,
        FONT_MAIN8OUTLINE_ID,
        RESOURCE_ID_MAX
    };

	Image* GetImageById(ResourceId theId);
    Font* GetFontById(ResourceId theId);
	int GetSoundById(ResourceId theId);

    bool ExtractResourcesByName(ResourceManager*, const char* name);

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    // Init Resources
	bool ExtractInitResources(ResourceManager *theMgr);

    extern Image* IMAGE_LOADING_SCREEN;
    extern Image* IMAGE_LOADERBAR;
    extern Image* IMAGE_CURSOR_POINTER;
    extern Image* IMAGE_CURSOR_HAND;
    extern Image* IMAGE_CURSOR_DRAGGING;
    extern Image* IMAGE_CURSOR_TEXT;
    extern Font* FONT_MAIN10;
    extern Font* FONT_DIALOG;
    extern Font* FONT_MAIN10OUTLINE;
    extern Font* FONT_MAIN10OUTLINE2;
    extern Font* FONT_MAIN10OUTLINE3;
    extern Font* FONT_FLOAT;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    // Adventure Screen Resources
    bool ExtractAdventureScreenResources(ResourceManager* theMgr);

    extern Image* IMAGE_ADVBACK;
    extern Image* IMAGE_ADVDOOR1A;
    extern Image* IMAGE_ADVDOOR1B;
    extern Image* IMAGE_ADVDOOR1C;
    extern Image* IMAGE_ADVDOOR2A;
    extern Image* IMAGE_ADVDOOR2B;
    extern Image* IMAGE_ADVDOOR2C;
    extern Image* IMAGE_ADVDOOR3A;
    extern Image* IMAGE_ADVDOOR3B;
    extern Image* IMAGE_ADVDOOR3C;
    extern Image* IMAGE_ADVDOOR4A;
    extern Image* IMAGE_ADVDOOR4B;
    extern Image* IMAGE_ADVDOOR4C;
    extern Image* IMAGE_ADVHIGHSCORE;
    extern Image* IMAGE_ADVMAINMENUBUTTON;
    extern Image* IMAGE_ADVMIDDLE;
    extern Image* IMAGE_ADVPLAYBUTTON;
    extern Image* IMAGE_ADVSKY;
    extern Image* IMAGE_ADVSTAGE;
    extern Image* IMAGE_ADVTEMPLE1;
    extern Image* IMAGE_ADVTEMPLE2;
    extern Image* IMAGE_ADVTEMPLE2V;
    extern Image* IMAGE_ADVTEMPLE3;
    extern Image* IMAGE_ADVTEMPLE3V;
    extern Image* IMAGE_ADVTEMPLE4;
    extern Image* IMAGE_ADVTITLE;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    // Gauntlet Screen Resources
    bool ExtractGauntletScreenResources(ResourceManager* theMgr);

    extern Image* IMAGE_GAUNTBACKBUTTON;
    extern Image* IMAGE_GAUNTDOOR1;
    extern Image* IMAGE_GAUNTDOOR2;
    extern Image* IMAGE_GAUNTDOOR3;
    extern Image* IMAGE_GAUNTDOOR4;
    extern Image* IMAGE_GAUNTEAGLEGEM;
    extern Image* IMAGE_GAUNTHIGHSCORE;
    extern Image* IMAGE_GAUNTJAGUARGEM;
    extern Image* IMAGE_GAUNTMAINMENUBUTTON;
    extern Image* IMAGE_GAUNTNEXTBUTTON;
    extern Image* IMAGE_GAUNTPLAYBUTTON;
    extern Image* IMAGE_GAUNTPRACTICEBUTTON;
    extern Image* IMAGE_GAUNTSCREEN;
    extern Image* IMAGE_GAUNTSKY;
    extern Image* IMAGE_GAUNTSUNGEM;
    extern Image* IMAGE_GAUNTSURVIVALBUTTON;
    extern Image* IMAGE_GAUNTTITLE;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    // Help Screen Resources
    bool ExtractHelpScreenResources(ResourceManager* theMgr);

    extern Image* IMAGE_HELP_BACK;
    extern Image* IMAGE_HELP_FRONT;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    // Help Screen Resources
    bool ExtractMainMenuResources(ResourceManager* theMgr);

    extern Image* IMAGE_MM_ARCADE;
    extern Image* IMAGE_MM_BACK;
    extern Image* IMAGE_MM_EYELEFT;
    extern Image* IMAGE_MM_EYEMASK;
    extern Image* IMAGE_MM_EYERIGHT;
    extern Image* IMAGE_MM_GAUNTLET;
    extern Image* IMAGE_MM_HAT;
    extern Image* IMAGE_MM_MOREGAMES;
    extern Image* IMAGE_MM_OPTIONS;
    extern Image* IMAGE_MM_QUIT;
    extern Image* IMAGE_MM_SKY;
    extern Image* IMAGE_MM_SUN;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    // More Games Resources
    bool ExtractMoreGamesResources(ResourceManager* theMgr);

    extern Image* IMAGE_MOREGAMES_BUTTON;
    extern Image* IMAGE_MOREGAMES_SCREEN;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    
    // Register Resources
    bool ExtractRegisterResources(ResourceManager* theMgr);

    extern Font* FONT_PLAIN;
    extern Font* FONT_TITLE;
    extern Image* IMAGE_DIALOG_BACK;
    extern Image* IMAGE_DIALOG_BUTTON;
    extern int SOUND_BUTTON1;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    
    // Loading Thread Resources 
    bool ExtractLoadingThreadResources(ResourceManager* theMgr);

    extern Font* FONT_BROWNTITLE;
    extern Font* FONT_HUGE;
    extern Font* FONT_MAIN12;
    extern Font* FONT_MAIN8;
    extern Font* FONT_MAIN8OUTLINE;
    extern Font* FONT_MONODIGIT;
    extern Image* IMAGE_SUNGLOW;
    extern Image* IMAGE_BLUE_BALL;
    extern Image* IMAGE_YELLOW_BALL;
    extern Image* IMAGE_RED_BALL;
    extern Image* IMAGE_GREEN_BALL;
    extern Image* IMAGE_PURPLE_BALL;
    extern Image* IMAGE_WHITE_BALL;
    extern Image* IMAGE_NEXT_BALL;
    extern Image* IMAGE_BALL_ALPHA;
    extern Image* IMAGE_BALL_SHADOW;
    extern Image* IMAGE_BLUE_ACCURACY;
    extern Image* IMAGE_YELLOW_ACCURACY;
    extern Image* IMAGE_RED_ACCURACY;
    extern Image* IMAGE_GREEN_ACCURACY;
    extern Image* IMAGE_PURPLE_ACCURACY;
    extern Image* IMAGE_WHITE_ACCURACY;
    extern Image* IMAGE_BLUE_BACKWARDS;
    extern Image* IMAGE_YELLOW_BACKWARDS;
    extern Image* IMAGE_RED_BACKWARDS;
    extern Image* IMAGE_GREEN_BACKWARDS;
    extern Image* IMAGE_PURPLE_BACKWARDS;
    extern Image* IMAGE_WHITE_BACKWARDS;
    extern Image* IMAGE_BLUE_SLOW;
    extern Image* IMAGE_YELLOW_SLOW;
    extern Image* IMAGE_RED_SLOW;
    extern Image* IMAGE_GREEN_SLOW;
    extern Image* IMAGE_PURPLE_SLOW;
    extern Image* IMAGE_WHITE_SLOW;
    extern Image* IMAGE_GRAY_EXPLOSION;
    extern Image* IMAGE_EXPLOSION;
    extern Image* IMAGE_BLUE_BOMB;
    extern Image* IMAGE_YELLOW_BOMB;
    extern Image* IMAGE_RED_BOMB;
    extern Image* IMAGE_GREEN_BOMB;
    extern Image* IMAGE_PURPLE_BOMB;
    extern Image* IMAGE_WHITE_BOMB;
    extern Image* IMAGE_BLUE_LIGHT;
    extern Image* IMAGE_YELLOW_LIGHT;
    extern Image* IMAGE_RED_LIGHT;
    extern Image* IMAGE_GREEN_LIGHT;
    extern Image* IMAGE_PURPLE_LIGHT;
    extern Image* IMAGE_WHITE_LIGHT;
    extern Image* IMAGE_BACKWARDS_LIGHT;
    extern Image* IMAGE_ACCURACY_LIGHT;
    extern Image* IMAGE_SLOW_LIGHT;
    extern Image* IMAGE_SPARKLE;
    extern Image* IMAGE_SHOOTER_BOTTOM;
    extern Image* IMAGE_SHOOTER_TOP;
    extern Image* IMAGE_SHOOTER_TONGUE;
    extern Image* IMAGE_EYE_BLINK;
    extern Image* IMAGE_DIALOG_CHECKBOX;
    extern Image* IMAGE_DIALOG_CHECKBOXCAP;
    extern Image* IMAGE_DIALOG_CHECKBOXLINE;
    extern Image* IMAGE_SLIDER_TRACK;
    extern Image* IMAGE_SLIDER_THUMB;
    extern Image* IMAGE_UPDATE_BAR;
    extern Image* IMAGE_GOD_HEAD;
    extern Image* IMAGE_HOLE;
    extern Image* IMAGE_HOLE_COVER;
    extern Image* IMAGE_COIN;
    extern Image* IMAGE_RIGHT_MOUSE;
    extern Image* IMAGE_MENU_BAR;
    extern Image* IMAGE_MENU_BUTTON;
    extern Image* IMAGE_ZUMA_BAR;
    extern Image* IMAGE_ZUMA_BAR_DONE;
    extern Image* IMAGE_FROG_LIVES;
    extern int SOUND_BALLCLICK1;
    extern int SOUND_BALLCLICK2;
    extern int SOUND_BALLFIRE;
    extern int SOUND_WARNING;
    extern int SOUND_EXPLODE;
    extern int SOUND_EARTHQUAKE;
    extern int SOUND_FROGLAND;
    extern int SOUND_JEWEL_APPEAR;
    extern int SOUND_JEWEL_DISAPPEAR;
    extern int SOUND_JEWEL_HIT;
    extern int SOUND_CHAIN_BONUS;
    extern int SOUND_GAP_BONUS;
    extern int SOUND_ROLLING;
    extern int SOUND_EXTRA_LIFE;
    extern int SOUND_SLOWDOWN_BALL;
    extern int SOUND_BACKWARDS_BALL;
    extern int SOUND_ACCURACY_BALL;
    extern int SOUND_TRAIL_LIGHT;
    extern int SOUND_TRAIL_LIGHT_END;
    extern int SOUND_GAME_START;
    extern int SOUND_LEVEL_UP;
    extern int SOUND_BAR_FULL;
    extern int SOUND_LOST_LIFE;
    extern int SOUND_GAME_OVER;
    extern int SOUND_STAGE_COMPLETE;
    extern int SOUND_TEMPLE_COMPLETE;
    extern int SOUND_BONUS_EXPLOSION;
    extern int SOUND_BALLDESTROYED1;
    extern int SOUND_BALLDESTROYED2;
    extern int SOUND_BALLDESTROYED3;
    extern int SOUND_BALLDESTROYED4;
    extern int SOUND_BALLDESTROYED5;
    extern int SOUND_COMBO;
    extern int SOUND_BUTTON2;
    extern int SOUND_CHORAL1;
    extern int SOUND_POP;
    extern int SOUND_UFO;
};

#endif