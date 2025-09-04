#include "ball.hpp"

void Ball::Init(btRigidBody* _collision){
    collision = _collision;

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
    std::string pathResource = DIR_RESOURCE;
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

const Vector3 Ball::Update(){
    
    if(IsKeyPressed(KEY_SPACE)){
        count++;
        PlaySound(sound);
    }

    if (IsKeyDown(KEY_W)) {
        if(collision->getLinearVelocity().getZ() < 100.0f){
            collision->applyForce(btVector3(0, 0, -10.0f), btVector3(0, 1.0f, 0));
        }
    }

    if (IsKeyDown(KEY_S)) {
        if(collision->getLinearVelocity() > btVector3(0.0f, 0.0f, 0.0f)){
            collision->applyForce(btVector3(0.0f, 0.0f, 10.0f), btVector3(0.0f, 0.0f, 0.0f));
        }
    }

    if (IsKeyDown(KEY_A)) {
        collision->applyForce(btVector3(-10.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
    }

    if (IsKeyDown(KEY_D)) {
        collision->applyForce(btVector3(10.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
    }

    if (collision->getMotionState()) {
        btTransform trans;
        collision->getMotionState()->getWorldTransform(trans);
        float x = float(trans.getOrigin().getX());
        float y = float(trans.getOrigin().getY());
        float z = float(trans.getOrigin().getZ());

        transform = MatrixTranslate(x, y, z);
        return (Vector3){ x, y, z };
    }
}

void Ball::Unload(){
    R3D_UnloadModel(&sphere, true);
    UnloadTexture(texture);
    UnloadSound(sound);
}