#include "game.hpp"

void Game::Load() {
    ball.Load();
    physics.Load();
}

void Game::Loop(void *self) {
    Game *client = static_cast<Game *>(self);

    const int result = client->Update();
    client->Render(result);
}

#if __EMSCRIPTEN__
EM_JS(int, getBrowserWidth, (), {
    return window.innerWidth;
});

EM_JS(int, getBrowserHeight, (), {
    return window.innerHeight;
});
#endif

void Game::Render(const int result) const {

#if __EMSCRIPTEN__
    static int PADDING = 30; // set padding to avoid scrollbar and browser edge overlap
    SetWindowSize(getBrowserWidth() - PADDING, getBrowserHeight() - PADDING);
#endif

    BeginDrawing();

    ClearBackground(BLACK);
    DrawRectangleGradientH(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE, GREEN);

    ball.Render();

    const char* countText = TextFormat("Count: %i", result);
    DrawText(countText, 50, 50, 20, WHITE);
    if (isCursorHidden == 1) DrawText("CURSOR HIDDEN", SCREEN_WIDTH/2-100, 60, 20, BLACK);
    else DrawText("CURSOR VISIBLE", SCREEN_WIDTH/2-100, 60, 20, WHITE);

    EndDrawing();
}

void Game::Run() {
    // no target FPS for web (performance)
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(Loop, this, 0, 1);
        SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    #else
        SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE); 
        SetTargetFPS(120);
        while (!WindowShouldClose()) {
            Loop(this);
        }
    #endif
}

void Game::Unload(){
    physics.Unload();
    ball.Unload();
}

const int Game::Update(){
    physics.Update();
    const int result = ball.Update();
    
    if (IsKeyPressed(KEY_H)){
        if (isCursorHidden == 0)
        {
            HideCursor();
            isCursorHidden = 1;
        }
        else
        {
            ShowCursor();
            isCursorHidden = 0;
        }
    }

    return result;
}
