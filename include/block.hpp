#pragma once

#include <vector>
#include <raylib.h>
#include <raymath.h>
#include <btBulletDynamicsCommon.h>
#include <r3d.h>

#include "physics.hpp"
#include "config.h"

class Block {
    // R3D_Mesh mesh = { 0 };
    R3D_Model cube = { 0 };
    // R3D_Material material = { 0 };
    Texture2D texture;
    // std::vector<btRigidBody*> collisions;
    // std::vector<Matrix> transforms;
    btRigidBody* collision;
    // Matrix rotation = { 0 };
    Matrix transform = { 0 };
    
    public:
        void Load();
        void Init(Physics& bullet);
        void Render() const;
        void Update();
        void Unload();
};
