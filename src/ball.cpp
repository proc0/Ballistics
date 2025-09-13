#include "ball.hpp"

#define BALL_MAX_SPEED 30.0f
#define BALL_ACCELERATION 2.0f
#define BALL_BREAK_FORCE 20.0f

void Ball::Init(Physics& bullet){
    collision = bullet.CreateSphere();

    if (collision->getMotionState()) {
        btTransform trans;
        collision->getMotionState()->getWorldTransform(trans);
        float x = float(trans.getOrigin().getX());
        float y = float(trans.getOrigin().getY());
        float z = float(trans.getOrigin().getZ());

        transform = MatrixTranslate(x, y, z);
    }
}

void Ball::Load(){
    std::string pathResource = DIR_ASSETS;
    const char* pathSoundSplat = pathResource.append("/").append(URI_SOUND_SPLAT).c_str();

    sound = LoadSound(pathSoundSplat);

    // Create scene objects
    sphere = R3D_LoadModel("assets/ball.obj");
    texture = LoadTexture("assets/uvgrid_1024.png");
    sphere.materials[0].albedo.texture = texture; 
}

void Ball::Render() const {
    R3D_DrawModelPro(&sphere, transform);
}

const std::pair<Vector3, Vector3> Ball::Update(Physics& bullet, Matrix camMatrix){
    Vector3 ballMovement = { 0, 0, 0 };

    btVector3 ballLinearV = collision->getLinearVelocity();
    Vector3 ballLinearVelocity = { ballLinearV.getX(), ballLinearV.getY(), ballLinearV.getZ() };
    Vector3 ballNormalVelocity = Vector3Transform(ballLinearVelocity, camMatrix);

    if(IsKeyPressed(KEY_SPACE)){
        if(bullet.IsGrounded()){
            PlaySound(sound);
            // collision->applyForce(btVector3(0, 150, 0), btVector3(0, 0, 0));
            ballMovement = { 0, 150, 0 };
            Vector3 vPrime1 = Vector3Transform(ballMovement, camMatrix);
            collision->applyForce({vPrime1.x, vPrime1.y, vPrime1.z}, btVector3(0.0f, 0.0f, 0.0f));
            bullet.SetGrounded(false);
        }
    }

    if (IsKeyDown(KEY_W)) {
        if(ballNormalVelocity.z < BALL_MAX_SPEED){
            // collision->applyForce(btVector3(0, 0, -BALL_ACCELERATION), btVector3(0.0f, 0.0f, 0.0f));
            collision->setLinearVelocity({ ballLinearV.getX(), ballLinearV.getY(), 0 });
            ballMovement = { 0, 0, -BALL_ACCELERATION };
            Vector3 vPrime1 = Vector3Transform(ballMovement, camMatrix);
            collision->applyForce({vPrime1.x, vPrime1.y, vPrime1.z}, btVector3(0.0f, 0.0f, 0.0f));
        }
    }

    if (IsKeyDown(KEY_S)) {
        if(ballNormalVelocity.z < BALL_MAX_SPEED){
            // collision->applyForce(btVector3(0.0f, 0.0f, BALL_ACCELERATION), btVector3(0.0f, 0.0f, 0.0f));
            collision->setLinearVelocity({ ballLinearV.getX(), ballLinearV.getY(), 0 });
            ballMovement = { 0, 0, BALL_ACCELERATION };
            Vector3 vPrime1 = Vector3Transform(ballMovement, camMatrix);
            collision->applyForce({vPrime1.x, vPrime1.y, vPrime1.z}, btVector3(0.0f, 0.0f, 0.0f));
        }
    }

    if (IsKeyDown(KEY_A)) {
        if(ballNormalVelocity.x < BALL_MAX_SPEED){
            // collision->applyForce(btVector3(-BALL_ACCELERATION, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
            collision->setLinearVelocity({ 0, ballLinearV.getY(), ballLinearV.getZ() });
            ballMovement = { -BALL_ACCELERATION, 0, 0 };
            Vector3 vPrime1 = Vector3Transform(ballMovement, camMatrix);
            collision->applyForce({vPrime1.x, vPrime1.y, vPrime1.z}, btVector3(0.0f, 0.0f, 0.0f));
        }
    }

    if (IsKeyDown(KEY_D)) {
        if(ballNormalVelocity.x < BALL_MAX_SPEED){
            // collision->applyForce(btVector3(BALL_ACCELERATION, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
            collision->setLinearVelocity({ 0, ballLinearV.getY(), ballLinearV.getZ() });
            ballMovement = { BALL_ACCELERATION, 0, 0 };
            Vector3 vPrime1 = Vector3Transform(ballMovement, camMatrix);
            collision->applyForce({vPrime1.x, vPrime1.y, vPrime1.z}, btVector3(0.0f, 0.0f, 0.0f));
        }
    }

    // btQuaternion ballOrientation = collision->getOrientation();
    // btVector3 ballVectorOrientation = (btVector3){ ballOrientation.getX(), ballOrientation.getY(), ballOrientation.getZ() };
    // btScalar ballVectorW = ballOrientation.getW();
    // btVector3 vPrime = 2.0f * btDot(ballVectorOrientation, ballMovement) * ballVectorOrientation + (ballVectorW*ballVectorW - btDot(ballVectorOrientation, ballVectorOrientation)) * ballMovement + 2.0f * ballVectorW * btCross(ballVectorOrientation, ballMovement);
    
    // Vector3 vPrime1 = Vector3Transform(ballMovement, camMatrix);
    // collision->applyForce({vPrime1.x, vPrime1.y, vPrime1.z}, btVector3(0.0f, 0.0f, 0.0f));

    if (collision->getMotionState()) {
        btTransform trans;
        collision->getMotionState()->getWorldTransform(trans);
        float x = float(trans.getOrigin().getX());
        float y = float(trans.getOrigin().getY());
        float z = float(trans.getOrigin().getZ());

        btQuaternion quatRot = trans.getRotation();
        // transform = MatrixTranslate(x, y, z);
        Quaternion quatRot2 = (Quaternion){
            x: quatRot.getX(),
            y: quatRot.getY(),
            z: quatRot.getZ(),
            w: quatRot.getW(),
        };

        Vector3 ballDelta = (Vector3){ x: x - transform.m12, y: y - transform.m13, z: z - transform.m14 };
        transform = MatrixMultiply(QuaternionToMatrix(quatRot2), MatrixTranslate(x, y, z));
        Vector3 ballPos = (Vector3){ x: transform.m12, y: transform.m13, z: transform.m14 };

        return std::make_pair(ballPos, ballDelta);
    }
}

void Ball::Unload(){
    R3D_UnloadModel(&sphere, true);
    UnloadTexture(texture);
    UnloadSound(sound);
}