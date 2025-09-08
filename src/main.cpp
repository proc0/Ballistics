#include "game.hpp"

int main(void){

    Game client;

#ifdef __EMSCRIPTEN__
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
#else
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
#endif

    R3D_Flags flags = R3D_FLAG_TRANSPARENT_SORTING | R3D_FLAG_FXAA;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROJECT_NAME);
    InitAudioDevice();
    R3D_Init(SCREEN_WIDTH, SCREEN_HEIGHT, flags);
    /* --- Setup background color and ambient light --- */

    R3D_SetBackgroundColor(BLACK);
    R3D_SetAmbientColor(DARKGRAY);

    /* --- Setup post processing parameters --- */

    R3D_SetSSR(true);

    R3D_SetSSAO(true);
    R3D_SetSSAORadius(2.0f);

    R3D_SetBloomIntensity(0.1f);
    R3D_SetBloomMode(R3D_BLOOM_MIX);
    R3D_SetTonemapMode(R3D_TONEMAP_ACES);
    
    client.Load();
    client.Run();
    client.Unload();

    R3D_Close();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}