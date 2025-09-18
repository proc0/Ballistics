#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <raylib.h>
#include <rcamera.h>
#include <r3d.h>

#include "config.h"
#include "physics.hpp"
#include "ball.hpp"
#include "block.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Game {
    Physics physics;
    Ball ball;
    Block block;
    Camera camera;
    R3D_Mesh plane = { 0 };
    R3D_Material material = { 0 };
    R3D_Skybox skybox;
    Vector3 mousePosition;
    Vector3 lookatpos;
    Vector3 lookatpos2;
    Matrix lookAt;
    Quaternion ballOrientation;
    Vector3 forwardZ;
    float angle;
    
    public:
        R3D_Light light;
        void Load();
        static void Loop(void *self);
        void Render(const Vector3 pos, const Vector3 pos2) const;
        void Run();
        void Update();
        void Unload();
};
