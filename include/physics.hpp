#pragma once

#include <btBulletDynamicsCommon.h>

class Physics {
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
    bool isGrounded = false;

    public:
        void Load();
        btRigidBody* CreateBlock(int x, int y, int z);
        btRigidBody* CreateSphere();
        static void onTickGroundSphere(btDynamicsWorld *world, btScalar timeStep);
        void Init();
        void Update();
        void Unload();
        bool IsGrounded();
};

