#include "ball.hpp"

void Ball::Load(){
    std::string pathResource = DIR_RESOURCE;
    const char* pathSoundSplat = pathResource.append("/").append(URI_SOUND_SPLAT).c_str();

    sound = LoadSound(pathSoundSplat);

    // Create scene objects
    mesh = R3D_GenMeshSphere(0.5f, 64, 64, true);
    material = R3D_GetDefaultMaterial();
}

void Ball::Render() const {
    // DrawCircleV(position, 40, color);
    R3D_DrawMesh(&mesh, &material, MatrixIdentity());
}

const int Ball::Update(){
    
    if(IsKeyPressed(KEY_SPACE)){
        count++;
        PlaySound(sound);
    }

    // position = GetMousePosition();

    return count;
}

void Ball::Unload(){
    R3D_UnloadMesh(&mesh);
    UnloadSound(sound);
}