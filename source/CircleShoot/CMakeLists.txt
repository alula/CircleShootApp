project(CircleShoot LANGUAGES CXX)

add_executable(Zuma WIN32
    Ball.cpp
    BlendedImage.cpp
    Board.cpp
    Bullet.cpp
    CircleButton.cpp
    CircleCommon.cpp
    CircleDialog.cpp
    CircleShootApp.cpp
    CurveData.cpp
    CurveMgr.cpp
    DataSync.cpp
    Gun.cpp
    HighScoreMgr.cpp
    LevelParser.cpp
    LoadingScreen.cpp
    Main.cpp
    MainMenu.cpp
    OptionsDialog.cpp
    ParticleMgr.cpp
    PracticeScreen.cpp
    ProfileMgr.cpp
    Res.cpp
    SoundMgr.cpp
    SpriteMgr.cpp
    StatsDialog.cpp
    TransitionMgr.cpp
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

target_link_libraries(Zuma SexyAppFramework)