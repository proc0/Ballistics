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
        block.Render();
        R3D_End();
        DrawFPS(10, 10);
    R3D_End();
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

    Vector3 forwardVector = { 0, 0, 1};
    Vector3 rightVector = Vector3Normalize(Vector3CrossProduct({ 0, 1, 0 }, forwardVector));
    Vector3 upVector = Vector3Normalize(Vector3CrossProduct(forwardVector, rightVector));
    float transZ = Vector3DotProduct(camera.position, forwardVector);
    float transX = Vector3DotProduct(camera.position, rightVector);
    float transY = Vector3DotProduct(camera.position, upVector);
    camMatrix = {
        m0: rightVector.x,
        m4: rightVector.y,
        m8: rightVector.z,
        m12: -transX,
        m1: upVector.x,
        m5: upVector.y,
        m9: upVector.z,
        m13: -transY,
        m2: forwardVector.x,
        m6: forwardVector.y,
        m10: forwardVector.z,
        m14: -transZ,
        m3: 0,
        m7: 0,
        m11: 0,
        m15: 1
    };
    camTrans = { (Vector3){ -transX, -transY, -transZ }, QuaternionFromMatrix(camMatrix), (Vector3){ 1, 1, 1 }};
    const std::pair<Vector3, Vector3> ballPosition = ball.Update(physics, camMatrix);
    physics.Update();
    block.Update();


    // CameraMoveRight(&camera, ballPosition.second.x, false);
    // CameraMoveForward(&camera, ballPosition.second.z, false);
    // Matrix rotation = MatrixRotate(GetCameraUp(&camera), -GetMouseDelta().x*0.003f);
    // Vector3 view = Vector3Subtract(camera.position, camera.target);
    // view = Vector3Transform(view, rotation);
    // camera.position = Vector3Add(camera.target, view);


    // Vector3 forwardVector = Vector3Normalize(Vector3Subtract(camera.position, ballPosition.second));
    // Vector3 rightVector = Vector3Normalize(Vector3CrossProduct(camera.up, forwardVector));
    // Vector3 upVector = Vector3Normalize(Vector3CrossProduct(forwardVector, rightVector));
    // float transZ = Vector3DotProduct(camera.position, forwardVector);
    // float transX = Vector3DotProduct(camera.position, rightVector);
    // float transY = Vector3DotProduct(camera.position, upVector);
    // Matrix lookAt = {
    //     m0: rightVector.x,
    //     m4: rightVector.y,
    //     m8: rightVector.z,
    //     m12: -transX,
    //     m1: upVector.x,
    //     m5: upVector.y,
    //     m9: upVector.z,
    //     m13: -transY,
    //     m2: forwardVector.x,
    //     m6: forwardVector.y,
    //     m10: forwardVector.z,
    //     m14: -transZ,
    //     m3: 0,
    //     m7: 0,
    //     m11: 0,
    //     m15: 1
    // };
    // camera.position = ballPosition + (Vector3){30.0f, 0.0f, 30.0f};
    camera.position.z = ballPosition.first.z + 30.0f;
    camera.target = ballPosition.first;
    // CameraYaw(&camera, -GetMouseDelta().x*0.003f, true);
    // camera.position = Vector3Transform(camera.position, lookAt);
    // camera.position += ballPosition.second;
    // camera.position.z += 30.0f;
    // camera.position += ballPosition.second;
    // camera.position.y = 20.0f;

    // CameraYaw(&camera, -135*DEG2RAD, true);
    // CameraPitch(&camera, -45*DEG2RAD, true, true, false);
    // UpdateCamera(&camera, CAMERA_THIRD_PERSON); 
    R3D_Begin(camera);
    BeginDrawing();
    DrawGizmo3D(GIZMO_LOCAL, &camTrans);
    EndDrawing();
    R3D_End();
}
