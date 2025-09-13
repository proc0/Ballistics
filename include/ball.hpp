#pragma once

#include <string>
#include <raylib.h>
#include <raymath.h>
#include <btBulletDynamicsCommon.h>
#include <r3d.h>

#include "physics.hpp"
#include "config.h"

#define URI_SOUND_SPLAT "splat1.wav"

class Ball {
    // R3D_Mesh mesh = { 0 };
    R3D_Model sphere = { 0 };
    // R3D_Material material = { 0 };
    Texture2D texture;
    Sound sound;
    btRigidBody* collision;
    Matrix rotation = { 0 };
    Matrix transform = { 0 };

    public:
        void Load();
        void Init(Physics& bullet);
        void Render() const;
        const std::pair<Vector3, Vector3> Update(Physics& bullet);
        void Unload();
};
