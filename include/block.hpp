#pragma once

#include <vector>
#include <raylib.h>
#include <raymath.h>
#include <btBulletDynamicsCommon.h>
#include <r3d.h>

#include "physics.hpp"
#include "config.h"

class Block {
    Texture2D texture;
    btAlignedObjectArray<btRigidBody*> collisions;
    btAlignedObjectArray<Matrix> transforms;
    btAlignedObjectArray<R3D_Model> cubes;
    
    public:
        void Load();
        void Init(Physics& bullet);
        void Render() const;
        void Pyramid(int blockWidth, int pyramidHeight, int xOffset, int yOffset, int zOffset, Physics& bullet);
        void Update();
        void Unload();
};
