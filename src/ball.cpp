#include "ball.hpp"

void Ball::Init(btRigidBody* _collision){
    collision = _collision;
}

void Ball::Load(){
    std::string pathResource = DIR_RESOURCE;
    const char* pathSoundSplat = pathResource.append("/").append(URI_SOUND_SPLAT).c_str();

    sound = LoadSound(pathSoundSplat);

    texture = LoadTexture("assets/uvgrid_1024.png");
    // Create scene objects
    // mesh = R3D_GenMeshSphere(0.5f, 64, 64, true);
    sphere = R3D_LoadModel("assets/ball.obj");
    // material = R3D_GetDefaultMaterial();

    sphere.materials[0].albedo.texture = texture; 

    transform = MatrixTranslate(2., 10., 0.);
}

void Ball::Render() const {
    // DrawCircleV(position, 40, color);
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

    if (collision->getMotionState())
    {
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