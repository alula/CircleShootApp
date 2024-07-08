# Zuma Deluxe Decompilation

Based on reverse engineering of Mac and Windows executables distributed on PopCap.com.

Status: Fully playable, but potential bugs and inaccuracies are expected.

This is a public preview version, most development is done in a private repo with tons of mess, so any changes will be just merged as huge commits until it's finished and then development switches there.

Dependencies:
- It's pretty much self contained, it compiles and works fine with MinGW and MSVC toolchains.
- CMake 3.18+: https://cmake.org/
- BASS 1.8+ DLL (BASS 2.x will NOT work without changes to the framework code), shipped with original game.

Differencies / accuracy notes:
- The most noticeable difference is that this project is based on open-source release of SexyAppFramework (1.3), with patches to bring back certain old behavior. Later we may reverse engineer framework code and bring it closer to older/internal versions of SexyApp.
- To reduce legal risk, we do not aim to reverse engineer DRM stuff, the resulting executable is DRM-free.
- Game logic code is where we put most care and aim for asm accuracy where possible.
- Certain variable/parameter names and missing methods have been reconstructed by reverse engineering Flash/C# ports of Zuma's Revenge.

Legal notes:
- The source code of game itself (everything in `source/CircleShootApp/`) is provided as-is.
- Any modifications are licensed under MIT.
- The main focus of the project is bug-fixing, modding and porting and is done in a good faith - we don't condone piracy, we just essentially want to give the game a "second life".
- The decompilation project does not include any assets - you need to get them from your own copy of the game - if you don't own it, Steam version is easily available and will do just fine: https://store.steampowered.com/app/3330/Zuma_Deluxe/
