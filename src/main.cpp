#include "game.hpp"

int main(void){

    Game client;

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