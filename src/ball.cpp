#include "ball.hpp"

#define BALL_MAX_SPEED 50.0f
#define BALL_ACCELERATION 10.0f
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

const Vector3 Ball::Update(bool isGrounded){
    
    if(IsKeyPressed(KEY_SPACE)){
        if(isGrounded){
            PlaySound(sound);
            collision->applyForce(btVector3(0, 150, 0), btVector3(0, 0, 0));
        }
    }

    if (IsKeyDown(KEY_W)) {
        if(fabsf(collision->getLinearVelocity().getZ()) < BALL_MAX_SPEED){
            collision->applyForce(btVector3(0, 0, -BALL_ACCELERATION), btVector3(0.0f, 0.0f, 0.0f));
        }
    }

    if (IsKeyDown(KEY_S)) {
        if(collision->getLinearVelocity().getZ() < BALL_MAX_SPEED){
            collision->applyForce(btVector3(0.0f, 0.0f, BALL_ACCELERATION), btVector3(0.0f, 0.0f, 0.0f));
        }
    }

    if (IsKeyDown(KEY_A)) {
        if(fabsf(collision->getLinearVelocity().getX()) < BALL_MAX_SPEED){
            collision->applyForce(btVector3(-BALL_ACCELERATION, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
        }
    }

    if (IsKeyDown(KEY_D)) {
        if(collision->getLinearVelocity().getX() < BALL_MAX_SPEED){
            collision->applyForce(btVector3(BALL_ACCELERATION, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
        }
    }

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
        // auto quatTrans = QuaternionFromMatrix(MatrixTranslate(x, y, z));
        // transform = QuaternionToMatrix(QuaternionMultiply(quatTrans, quatRot2));
        transform = MatrixMultiply(QuaternionToMatrix(quatRot2), MatrixTranslate(x, y, z));
        return (Vector3){ x: transform.m12, y: transform.m13, z: transform.m14 };
    }
}

void Ball::Unload(){
    R3D_UnloadModel(&sphere, true);
    UnloadTexture(texture);
    UnloadSound(sound);
}