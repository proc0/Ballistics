#include "block.hpp"

void Block::Init(Physics& bullet){

    Pyramid(2, 4, 10, 10, -10, bullet);

    texture = LoadTexture("assets/uvgrid_1024.png");
    
    for (int i = 0; i < collisions.size(); i++) {
        if (collisions[i]->getMotionState()) {
            btTransform transform;

            collisions[i]->getMotionState()->getWorldTransform(transform);
            float x = float(transform.getOrigin().getX());
            float y = float(transform.getOrigin().getY());
            float z = float(transform.getOrigin().getZ());
    
            transforms.push_back(MatrixTranslate(x, y, z));
        }
        R3D_Model cube = R3D_LoadModel("assets/cube.obj");
        cube.materials[0].albedo.texture = texture;
        cube.materials[0].orm.roughness = 0.0f;
        cube.materials[0].orm.metalness = 0.8f;
        cubes.push_back(cube);
    }
}

void Block::Load(){
    // Create scene objects
    // cube = R3D_LoadModel("assets/cube.obj");
    // texture = LoadTexture("assets/uvgrid_1024.png");
    // cube.materials[0].albedo.texture = texture; 
}

void Block::Render() const {
    for (int i = 0; i < transforms.size(); i++) {
        R3D_DrawModelPro(&cubes[i], transforms[i]);
    }
}

void Block::Update(){

    for (int i = 0; i < collisions.size(); i++) {
        if (collisions[i]->getMotionState()) {
            btTransform transform;

            collisions[i]->getMotionState()->getWorldTransform(transform);
            float x = float(transform.getOrigin().getX());
            float y = float(transform.getOrigin().getY());
            float z = float(transform.getOrigin().getZ());

            btQuaternion btRotation = transform.getRotation();
            Quaternion rotation = (Quaternion){
                x: btRotation.getX(),
                y: btRotation.getY(),
                z: btRotation.getZ(),
                w: btRotation.getW(),
            };

            transforms[i] = MatrixMultiply(QuaternionToMatrix(rotation), MatrixTranslate(x, y, z));
        }
    }
}

void Block::Pyramid(int blockWidth, int pyramidHeight, int xOffset, int yOffset, int zOffset, Physics& bullet) {
    for(int y = pyramidHeight; y > 0; y--){
        for(int x = y*2 - 1; x > 0; x--){
            collisions.push_back(bullet.CreateBlock(x*blockWidth - y*blockWidth + xOffset, (pyramidHeight + 1 - y)*2 + 2 + yOffset, zOffset));
        }
    }
}

void Block::Unload(){
    for (int i = 0; i < cubes.size(); i++) {
        R3D_UnloadModel(&cubes[i], true);
    }
    UnloadTexture(texture);
}