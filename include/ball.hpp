#pragma once

#include <string>
#include <raylib.h>
#include <raymath.h>
#include <btBulletDynamicsCommon.h>
#include <r3d.h>

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
        bool isJumping = false;
        void Load();
        void Init(btRigidBody* collision);
        void Render() const;
        const Vector3 Update(bool isGrounded);
        void Unload();
};
