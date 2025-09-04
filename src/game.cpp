#include "game.hpp"

void Game::Load() {
    
    // Setup lighting
    light = R3D_CreateLight(R3D_LIGHT_DIR);
    R3D_SetLightDirection(light, (Vector3){ -1, -1, -1 });
    R3D_SetLightActive(light, true);
    // light = R3D_CreateLight(R3D_LIGHT_SPOT);
    // {
    //     R3D_LightLookAt(light, (Vector3) { 0, 10, 5 }, (Vector3) { 0 });
    //     R3D_EnableShadow(light, 4096);
    //     R3D_SetLightActive(light, true);
    // }

    physics.Load();

    plane = R3D_GenMeshPlane(300, 300, 1, 1, true);
    material = R3D_GetDefaultMaterial();

    ball.Load();
    btRigidBody* sphereCollision = physics.Init();
    ballCollision = sphereCollision;
    ball.Init(sphereCollision);
    // Camera setup
    // camera = {
    //     .position = { -3, 3, 3 },
    //     .target = { 0, 0, 0 },
    //     .up = { 0, 1, 0 },
    //     .fovy = 60.0f,
    //     .projection = CAMERA_PERSPECTIVE
    // };
    camera = (Camera3D) {
        .position = (Vector3) { 0, 12, 24 },
        .target = (Vector3) { 0, 0, 0 },
        .up = (Vector3) { 0, 1, 0 },
        .fovy = 45,
        .projection = CAMERA_PERSPECTIVE,
    };

}

void Game::Loop(void *self) {
    Game *client = static_cast<Game *>(self);

    // const int result = client->Update();
    // client->Render(result);
    client->Update();
    client->Render();
}

#if __EMSCRIPTEN__
EM_JS(int, getBrowserWidth, (), {
    return window.innerWidth;
});

EM_JS(int, getBrowserHeight, (), {
    return window.innerHeight;
});
#endif

void Game::Render() const {

#if __EMSCRIPTEN__
    static int PADDING = 30; // set padding to avoid scrollbar and browser edge overlap
    SetWindowSize(getBrowserWidth() - PADDING, getBrowserHeight() - PADDING);
#endif

    BeginDrawing();

    // ClearBackground(BLACK);
    // DrawRectangleGradientH(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE, GREEN);
    R3D_Begin(camera);
    R3D_DrawMesh(&plane, &material, MatrixIdentity());
    ball.Render();
    R3D_End();

    // const char* countText = TextFormat("Count: %i", result);
    // DrawText(countText, 50, 50, 20, WHITE);
    // if (isCursorHidden == 1) DrawText("CURSOR HIDDEN", SCREEN_WIDTH/2-100, 60, 20, BLACK);
    // else DrawText("CURSOR VISIBLE", SCREEN_WIDTH/2-100, 60, 20, WHITE);
    DrawFPS(10, 10);
    EndDrawing();
}

void Game::Run() {
    // no target FPS for web (performance)
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(Loop, this, 0, 1);
        SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    #else
        SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
        SetTargetFPS(60);
        while (!WindowShouldClose()) {
            Loop(this);
        }
    #endif
}

void Game::Unload(){
    R3D_UnloadMesh(&plane);
    ball.Unload();
    physics.Unload();
}

void Game::Update(){
    physics.Update();
    const Vector3 result = ball.Update();
    // UpdateCamera(&camera, CAMERA_THIRD_PERSON);
    // if (IsKeyPressed(KEY_H)){
    //     if (isCursorHidden == 0)
    //     {
    //         HideCursor();
    //         isCursorHidden = 1;
    //     }
    //     else
    //     {
    //         ShowCursor();
    //         isCursorHidden = 0;
    //     }
    // }

    camera.position.x = result.x;
    camera.position.z = result.z + 30.0f;
    camera.target = result;
}
