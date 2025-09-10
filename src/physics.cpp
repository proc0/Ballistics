#include "physics.hpp"

void Physics::Load() {
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void Physics::Init() {
	
	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(150.), btScalar(10.), btScalar(150.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -10, 0));

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		btScalar mass(0.);
		btVector3 localInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body, 1, 1);
	}
}

btRigidBody* Physics::CreateBlock(int x, int y, int z) {

	//create a dynamic rigidbody

	//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	btCollisionShape* colShape = new btBoxShape(btVector3(btScalar(1.), btScalar(1.), btScalar(1.)));
	collisionShapes.push_back(colShape);
	
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(0.3f);

	btVector3 localInertia(0, 0, 0);
	colShape->calculateLocalInertia(mass, localInertia);
	startTransform.setOrigin(btVector3(x, y, z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	
	body->setFriction(1000);
	dynamicsWorld->addRigidBody(body);

	return body;
}

btRigidBody* Physics::CreateSphere() {

	//create a dynamic rigidbody

	//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	btCollisionShape* colShape = new btSphereShape(btScalar(1.));
	collisionShapes.push_back(colShape);
	
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(0.8f);

	btVector3 localInertia(0, 0, 0);
	colShape->calculateLocalInertia(mass, localInertia);
	startTransform.setOrigin(btVector3(2, 10, 0));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	
	body->setFriction(1000);
	dynamicsWorld->addRigidBody(body, 1, 1);
	// void* callback = [this](btDynamicsWorld *world, btScalar timeStep){ this->onTickGroundSphere(world, timeStep); };
	// dynamicsWorld->setInternalTickCallback([this](btDynamicsWorld *world, btScalar timeStep){ this->onTickGroundSphere(world, timeStep); });
	dynamicsWorld->setInternalTickCallback(onTickGroundSphere, this);

	return body;
}

void Physics::onTickGroundSphere(btDynamicsWorld *world, btScalar timeStep) {
    int numManifolds = world->getDispatcher()->getNumManifolds();
	Physics* info = static_cast<Physics*>(world->getWorldUserInfo());
    // printf("numManifolds = %d\n",numManifolds);
	if(numManifolds == 1){
		printf("Grounded");
		info->isGrounded = true;
	}
}

bool Physics::IsGrounded(){
	return isGrounded;
}

// bool Physics::IsGrounded(){
// 	bool result = false;
//     int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
//     for (int i=0;i<numManifolds;i++)
//     {
//         btPersistentManifold* contactManifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
//         // btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
//         // btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

//         int numContacts = contactManifold->getNumContacts();
//         for (int j=0;j<numContacts;j++)
//         {
//             btManifoldPoint& pt = contactManifold->getContactPoint(j);
//             if (pt.getDistance()<0.f)
//             {
//                 // const btVector3& ptA = pt.getPositionWorldOnA();
//                 // const btVector3& ptB = pt.getPositionWorldOnB();
//                 // const btVector3& normalOnB = pt.m_normalWorldOnB;
// 				result = true;
// 				break;
//             }
//         }

// 		if(result) break;
//     }
// 	return result;
// }

void Physics::Update(){
	dynamicsWorld->stepSimulation(1.f / 60.f, 10);
	// btRigidBody* ball;
	// for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	// {
	// 	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
	// 	btRigidBody* body = btRigidBody::upcast(obj);
	// 	btTransform trans;
	// 	if (body && body->getMotionState())
	// 	{
	// 		body->getMotionState()->getWorldTransform(trans);
	// 	}
	// 	else
	// 	{
	// 		trans = obj->getWorldTransform();
	// 	}
}

void Physics::Unload() {

	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}
