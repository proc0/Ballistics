#include "game.hpp"

void Game::Load() {

    skybox = R3D_LoadSkybox("assets/skybox3.png", CUBEMAP_LAYOUT_AUTO_DETECT);

    R3D_SetSceneBounds((BoundingBox) { { -80, -80, -80 }, { 80, 80, 80 } });
    // Setup lighting
    R3D_Light light = R3D_CreateLight(R3D_LIGHT_DIR);
    {
        R3D_SetLightDirection(light, (Vector3){ -1, -1, -1 });
        R3D_EnableShadow(light, 4096);
        R3D_SetLightActive(light, true);
    }

    R3D_EnableSkybox(skybox);
    
    plane = R3D_GenMeshPlane(300, 300, 1, 1, true);
    material = R3D_GetDefaultMaterial();
    material.albedo.color = (Color) { 31, 31, 31, 255 };
    material.orm.roughness = 0.5f;
    material.orm.metalness = 0.0f;
    
    physics.Load();
    ball.Load();
    block.Load();

    physics.Init();
    ball.Init(physics);
    block.Init(physics);
    // Camera setup
    camera = (Camera3D) {
        .position = (Vector3) { 0, 12, 24 },
        .target = (Vector3) { 0, 0, 0 },
        .up = (Vector3) { 0, 1, 0 },
        .fovy = 45,
        .projection = CAMERA_PERSPECTIVE,
    };

    UpdateCamera(&camera, CAMERA_THIRD_PERSON);
    // camera.position = (Vector3){ 0.0f, 2.0f, -100.0f };
    // camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    // camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };

}

void Game::Loop(void *self) {
    Game *client = static_cast<Game *>(self);

    // const int result = client->Update();
    // client->Render(result);
    client->Update();
    client->Render(client->lookatpos);
}

#if __EMSCRIPTEN__
EM_JS(int, getBrowserWidth, (), {
    return window.innerWidth;
});

EM_JS(int, getBrowserHeight, (), {
    return window.innerHeight;
});
#endif

void Game::Render(const Vector3 pos) const {

#if __EMSCRIPTEN__
    static int PADDING = 30; // set padding to avoid scrollbar and browser edge overlap
    SetWindowSize(getBrowserWidth() - PADDING, getBrowserHeight() - PADDING);
#endif

    BeginDrawing();

    R3D_Begin(camera);
    R3D_DrawMesh(&plane, &material, MatrixIdentity());
    ball.Render(pos);
    block.Render();
    R3D_End();

    BeginMode3D(camera);
    DrawLine3D(pos, { pos.x + 5, pos.y, pos.z }, RED);
    DrawLine3D(pos, { pos.x, pos.y + 5, pos.z }, GREEN);
    DrawLine3D(pos, { pos.x, pos.y, pos.z + 5 }, BLUE);
    EndMode3D();
    DrawFPS(10, 10);
    EndDrawing();
}

void Game::Run() {
    // no target FPS for web (performance)
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(Loop, this, 0, 1);
    #else
        SetTargetFPS(60);
        while (!WindowShouldClose()) {
            Loop(this);
        }
    #endif
}

void Game::Unload(){
    block.Unload();
    ball.Unload();
    physics.Unload();
    R3D_UnloadMesh(&plane);
    R3D_UnloadSkybox(skybox);
}

void Game::Update(){

    physics.Update();
    block.Update();
    const std::pair<Vector3, Vector3> ballPosition = ball.Update(physics);

    Matrix lookAt = MatrixLookAt(camera.position, ballPosition.first, camera.up);
    lookatpos = Vector3Transform(Vector3Subtract(camera.position, ballPosition.first), lookAt);
    
    camera.target = ballPosition.first;
    camera.position += ballPosition.second;
    camera.position.y = 20.0f;
    // CameraYaw(&camera, -135*DEG2RAD, true);
    // CameraPitch(&camera, -45*DEG2RAD, true, true, false);
    // UpdateCamera(&camera, CAMERA_THIRD_PERSON); 

}
