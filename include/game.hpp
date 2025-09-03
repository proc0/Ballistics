#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <raylib.h>

#include "config.h"
#include "physics.hpp"
#include "ball.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Game {
    Physics physics;
    Ball ball;
    bool isCursorHidden = false;

    public:
        void Load();
        static void Loop(void *self);
        void Render(const int result) const;
        void Run();
        const int Update();
        void Unload();
};
