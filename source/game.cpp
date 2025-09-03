#include "game.hpp"

void Game::Load(){
    std::string pathResource = DIR_RESOURCE;
    const char* pathSoundSplat = pathResource.append("/").append(URI_SOUND_SPLAT).c_str();

    splat = LoadSound(pathSoundSplat);
    count = 0;

    physics.Load();
}

void Game::Unload(){
    physics.Unload();

    UnloadSound(splat);
}

void Game::Update(){
    physics.Update();
    
    if(IsKeyPressed(KEY_SPACE)){
        count++;
        PlaySound(splat);
    }
    
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
    
    ballPosition = GetMousePosition();
}

void Game::Render() const {

    // set proper width and height in browser window
#if __EMSCRIPTEN__
    int padding = 30; // set padding to avoid scrollbar and browser edge overlap
    SetWindowSize(getBrowserWidth() - padding, getBrowserHeight() - padding);
#endif

    BeginDrawing();

    ClearBackground(BLACK);
    DrawRectangleGradientH(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE, GREEN);
    DrawCircleV(ballPosition, 40, ballColor);
    const char* countText = TextFormat("Count: %i", count);
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

void Game::Loop(void *self)
{
    Game *Client = static_cast<Game *>(self);

    Client->Update();
    Client->Render();
}