#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <raylib.h>
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
    
    public:
        R3D_Light light;
        void Load();
        static void Loop(void *self);
        void Render() const;
        void Run();
        void Update();
        void Unload();
};
