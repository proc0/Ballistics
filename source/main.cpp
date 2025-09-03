#include "game.hpp"

int main(void){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROJECT_NAME);
    InitAudioDevice();

    Game client;
    
    client.Load();
    client.Run();
    client.Unload();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}