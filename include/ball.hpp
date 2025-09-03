#pragma once

#include <raylib.h>
#include <string>

#include "config.h"

#define URI_SOUND_SPLAT "splat1.wav"

class Ball {
    Sound sound;
    Color color = RED;
    Vector2 position = { -100.0f, -100.0f };
    int count = 0;

    public:
        void Load();
        void Render() const;
        const int Update();
        void Unload();
};
