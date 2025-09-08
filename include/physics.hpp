#pragma once

#include <btBulletDynamicsCommon.h>

class Physics {
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    public:
        void Load();
        btRigidBody* Init();
        void Update();
        void Unload();
        bool IsGrounded();
};

