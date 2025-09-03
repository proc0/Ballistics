#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "game.hpp"

#if __EMSCRIPTEN__
EM_JS(int, getBrowserWidth, (), {
    return window.innerWidth;
});

EM_JS(int, getBrowserHeight, (), {
    return window.innerHeight;
});
#endif

int main(void){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Example");
    InitAudioDevice();

    Game NewGame;
    
    NewGame.Load();
    NewGame.Run();
    NewGame.Unload();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}