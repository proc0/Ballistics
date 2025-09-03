#include "ball.hpp"

void Ball::Load(){
    std::string pathResource = DIR_RESOURCE;
    const char* pathSoundSplat = pathResource.append("/").append(URI_SOUND_SPLAT).c_str();

    sound = LoadSound(pathSoundSplat);
}

void Ball::Render() const {
    DrawCircleV(position, 40, color);
}

const int Ball::Update(){
    
    if(IsKeyPressed(KEY_SPACE)){
        count++;
        PlaySound(sound);
    }

    position = GetMousePosition();

    return count;
}

void Ball::Unload(){
    UnloadSound(sound);
}