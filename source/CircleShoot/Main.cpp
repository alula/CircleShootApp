#include "Zuma_Prefix.pch"

#include "CircleShootApp.h"

#ifdef _WIN32
//#if 0
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#else
int main() {
#endif
    Sexy::CircleShootApp app;

    app.Init();
    app.Start();
    app.Shutdown();
    
    return 0;
}