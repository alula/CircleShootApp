#include "Zuma_Prefix.pch"

#include "CircleShootApp.h"

#ifdef _WIN32
//int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
int main() {
#else
int main() {
#endif
    Sexy::CircleShootApp app;

    app.Init();
    app.Set3DAcclerated(false);
    app.Start();
    app.Shutdown();
    
    return 0;
}