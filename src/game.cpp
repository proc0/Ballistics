#include "game.hpp"

void Game::Load() {
    
    // Setup lighting
    R3D_Light light = R3D_CreateLight(R3D_LIGHT_DIR);
    {
        R3D_SetLightDirection(light, (Vector3){ -1, -1, -1 });
        R3D_EnableShadow(light, 4096);
        R3D_SetLightActive(light, true);
    }

    physics.Load();

    plane = R3D_GenMeshPlane(300, 300, 1, 1, true);
    material = R3D_GetDefaultMaterial();
    material.albedo.color = (Color) { 31, 31, 31, 255 };
    material.orm.roughness = 0.0f;
    material.orm.metalness = 0.5f;

    ball.Load();
    btRigidBody* sphereCollision = physics.Init();
    ball.Init(sphereCollision);
    // Camera setup
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

    R3D_Begin(camera);
    R3D_DrawMesh(&plane, &material, MatrixIdentity());
    ball.Render();
    R3D_End();

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
    const Vector3 ballPosition = ball.Update();
    camera.position.x = ballPosition.x;
    camera.position.z = ballPosition.z + 30.0f;
    camera.target = ballPosition;
}
