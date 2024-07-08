project(CircleShoot LANGUAGES CXX)

add_executable(Zuma WIN32
    AdventureScreen.cpp
    Ball.cpp
    BlendedImage.cpp
    Board.cpp
    Bullet.cpp
    CircleButton.cpp
    CircleCheckbox.cpp
    CircleCommon.cpp
    CircleDialog.cpp
    CircleShootApp.cpp
    CreateUserDialog.cpp
    CreditsScreen.cpp
    CurveData.cpp
    CurveMgr.cpp
    DataSync.cpp
    Gun.cpp
    HelpScreen.cpp
    HighScoreMgr.cpp
    LevelParser.cpp
    LoadingScreen.cpp
    Main.cpp
    MainMenu.cpp
    MoreGamesScreen.cpp
    OptionsDialog.cpp
    ParticleMgr.cpp
    PracticeScreen.cpp
    ProfileMgr.cpp
    Res.cpp
    SoundMgr.cpp
    SpriteMgr.cpp
    StatsDialog.cpp
    TransitionMgr.cpp
    UserDialog.cpp
    WayPoint.cpp
    WidgetMover.cpp
    WorkerThread.cpp
    Zuma.rc
)

target_include_directories(Zuma PRIVATE
    ${SEXYAPP_INCLUDE_DIRS}
)

if(MSYS OR MINGW)
    target_link_libraries(Zuma -static-libgcc -static-libstdc++)
endif()

if(MSVC AND MSVC_VERSION EQUAL 1300)
    target_link_options(Zuma PRIVATE /INCREMENTAL:NO /DEBUG /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /MACHINE:IX86 /pdb:${CMAKE_CURRENT_BINARY_DIR}/Zuma.pdb)
endif()

target_link_libraries(Zuma SexyAppFramework)