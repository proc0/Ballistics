#pragma once

#include <raylib.h>
#include <string>

#include "config.h"

#define URI_SOUND_SPLAT "splat1.wav"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Game {
    int count;
    Sound splat;
    Color ballColor = RED;
    Vector2 ballPosition = { -100.0f, -100.0f };
    bool isCursorHidden = false;

    public:
        void Load();
        static void Loop(void *self); // Emscripten
        void Render() const;
        void Run();
        void Unload();
        void Update();
};
