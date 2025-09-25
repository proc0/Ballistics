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
        .fovy = 60,
        .projection = CAMERA_PERSPECTIVE,
    };

    DisableCursor();
    UpdateCamera(&camera, CAMERA_THIRD_PERSON);
    ballPos = (Vector3) { 0, 0, 0 };

}

void Game::Loop(void *self) {
    Game *client = static_cast<Game *>(self);

    // const int result = client->Update();
    // client->Render(result);
    client->Update();
    client->Render(client->ballPos, client->ballDir);
}

#if __EMSCRIPTEN__
EM_JS(int, getBrowserWidth, (), {
    return window.innerWidth;
});

EM_JS(int, getBrowserHeight, (), {
    return window.innerHeight;
});
#endif

void Game::Render(const Vector3 ballPos, const Vector3 ballDir) const {

#if __EMSCRIPTEN__
    static int PADDING = 30; // set padding to avoid scrollbar and browser edge overlap
    SetWindowSize(getBrowserWidth() - PADDING, getBrowserHeight() - PADDING);
#endif

    BeginDrawing();

    R3D_Begin(camera);
    R3D_DrawMesh(&plane, &material, MatrixIdentity());
    ball.Render();
    block.Render();
    R3D_End();

    BeginMode3D(camera);
    DrawLine3D(ballDir, ballPos, BLUE);
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

    float rotDeg = -135*DEG2RAD*GetMouseDelta().x*0.003f;
    CameraYaw(&camera, rotDeg, true);

    const std::pair<Vector3, Vector3> ballResult = ball.Update(physics, camera.position);

    Vector3 forwardZ = Vector3Subtract(ballResult.first, camera.position);
    Matrix ballTrans = MatrixTranslate(ballResult.first.x, ballResult.first.y, ballResult.first.z);
    forwardZ.y = ballResult.first.y;
    ballDir = Vector3Transform(forwardZ, ballTrans);

    camera.target = ballResult.first;
    camera.position += ballResult.second;
    camera.position.y = 10.0f;
    ballPos = ballResult.first;
}
