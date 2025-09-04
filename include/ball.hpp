#pragma once

#include <string>
#include <raylib.h>
#include <raymath.h>
#include <r3d.h>

#include "config.h"

#define URI_SOUND_SPLAT "splat1.wav"

class Ball {
    R3D_Mesh mesh = { 0 };
    R3D_Material material = { 0 };
    Sound sound;
    int count = 0;
    
    public:
        void Load();
        void Render() const;
        const int Update();
        void Unload();
};
