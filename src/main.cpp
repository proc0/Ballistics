#include "game.hpp"

int main(void){

    Game client;

#ifdef __EMSCRIPTEN__
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
#else
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
#endif

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROJECT_NAME);
    R3D_Init(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    InitAudioDevice();
    
    client.Load();
    client.Run();
    client.Unload();

    CloseAudioDevice();
    R3D_Close();
    CloseWindow();

    return 0;
}