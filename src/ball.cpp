#include "ball.hpp"

#define BALL_MAX_SPEED 50.0f
#define BALL_ACCELERATION 8.0f
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
    sphere = R3D_LoadModel("assets/soccerball.glb");
    texture = LoadTexture("assets/soccertext.jpg");
    sphere.materials[0].albedo.texture = texture; 
}

void Ball::Render() const {
    R3D_DrawModelPro(&sphere, transform);
}

const std::pair<Vector3, Vector3> Ball::Update(Physics& bullet, Vector3 cameraPos) {
    
    Vector3 forwardZ = Vector3Subtract(position, cameraPos);
    forwardZ.y = position.y;
    Vector3 forwardX = Vector3RotateByAxisAngle(forwardZ, {0, 1, 0}, -90.0f);
    Vector3 fwdZ = Vector3Normalize(forwardZ);
    Vector3 fwdX = Vector3Normalize(forwardX);

    if(IsKeyPressed(KEY_SPACE)){
        if(bullet.IsGrounded()){
            PlaySound(sound);
            collision->applyForce(btVector3(0, 300.0f, 0), btVector3(0, 0, 0));
            bullet.SetGrounded(false);
        }
    }

    if (IsKeyDown(KEY_W)) {
        // if(fabsf(collision->getLinearVelocity().getZ()) < BALL_MAX_SPEED){
            collision->applyForce(btVector3(fwdZ.x*BALL_ACCELERATION, 0, fwdZ.z*BALL_ACCELERATION), btVector3(0, 0, 0));
        // }
    }

    if (IsKeyDown(KEY_S)) {
        // if(collision->getLinearVelocity().getZ() < BALL_MAX_SPEED){
            collision->applyForce(btVector3(fwdZ.x*-BALL_ACCELERATION, 0, fwdZ.z*-BALL_ACCELERATION), btVector3(0, 0, 0));
        // }
    }

    if (IsKeyDown(KEY_A)) {
            collision->applyForce(btVector3(fwdX.x*-BALL_ACCELERATION, 0, fwdX.z*-BALL_ACCELERATION), btVector3(0, 0, 0));
        // if(fabsf(collision->getLinearVelocity().getX()) < BALL_MAX_SPEED){
        //     collision->applyForce(btVector3(-BALL_ACCELERATION, 0.0f, 0), btVector3(fwdX.x, 0.0f, fwdZ.z));
        // }
    }

    if (IsKeyDown(KEY_D)) {
            collision->applyForce(btVector3(fwdX.x*BALL_ACCELERATION, 0, fwdX.z*BALL_ACCELERATION), btVector3(0, 0, 0));
        // if(collision->getLinearVelocity().getX() < BALL_MAX_SPEED){
        //     collision->applyForce(btVector3(BALL_ACCELERATION, 0, 0), btVector3(fwdX.x, 0.0f, fwdZ.z));
        // }
    }

    if (collision->getMotionState()) {
        btTransform trans;
        collision->getMotionState()->getWorldTransform(trans);
        float x = float(trans.getOrigin().getX());
        float y = float(trans.getOrigin().getY());
        float z = float(trans.getOrigin().getZ());

        btQuaternion quatRot = trans.getRotation();
        Quaternion quatRot2 = (Quaternion){
            x: quatRot.getX(),
            y: quatRot.getY(),
            z: quatRot.getZ(),
            w: quatRot.getW(),
        };

        Vector3 ballDelta = (Vector3){ x: x - transform.m12, y: y - transform.m13, z: z - transform.m14 };
        transform = MatrixMultiply(QuaternionToMatrix(quatRot2), MatrixTranslate(x, y, z));
        Vector3 ballPos = (Vector3){ x: transform.m12, y: transform.m13, z: transform.m14 };

        position = ballPos;
        return std::make_pair(ballPos, ballDelta);
    }
}

void Ball::Unload(){
    R3D_UnloadModel(&sphere, true);
    UnloadTexture(texture);
    UnloadSound(sound);
}