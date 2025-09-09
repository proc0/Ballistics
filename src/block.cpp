#include "block.hpp"

void Block::Init(Physics& bullet){
    // collision = bullet.CreateBlock(2, 4, -10);

    for(int k = 4; k > 0; k--){
        for(int j = 0; j < k; j++){
            collisions.push_back(bullet.CreateBlock(k + 4, j + 4, -10));
        }
        if(k < 3){
            for(int l = 0; l < k; l++){
                collisions.push_back(bullet.CreateBlock(k + 4 - l, l + 4, -10));
            }
        }
    }
    // collisions.push_back(bullet.CreateBlock(2, 4, -10));
    // collisions.push_back(bullet.CreateBlock(0, 4, -10));
    // collisions.push_back(bullet.CreateBlock(-2, 4, -10));

    texture = LoadTexture("assets/uvgrid_1024.png");
    for (int i = 0; i < collisions.size(); i++) {
        if (collisions[i]->getMotionState()) {
            btTransform trans;
            collisions[i]->getMotionState()->getWorldTransform(trans);
            float x = float(trans.getOrigin().getX());
            float y = float(trans.getOrigin().getY());
            float z = float(trans.getOrigin().getZ());
    
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
    // R3D_DrawModelPro(&cube, transform);
}

void Block::Update(){

    // if (collision->getMotionState()) {
    //     btTransform trans;
    //     collision->getMotionState()->getWorldTransform(trans);
    //     float x = float(trans.getOrigin().getX());
    //     float y = float(trans.getOrigin().getY());
    //     float z = float(trans.getOrigin().getZ());

    //     btQuaternion quatRot = trans.getRotation();
    //     // transform = MatrixTranslate(x, y, z);
    //     Quaternion quatRot2 = (Quaternion){
    //         x: quatRot.getX(),
    //         y: quatRot.getY(),
    //         z: quatRot.getZ(),
    //         w: quatRot.getW(),
    //     };
    //     // auto quatTrans = QuaternionFromMatrix(MatrixTranslate(x, y, z));
    //     // transform = QuaternionToMatrix(QuaternionMultiply(quatTrans, quatRot2));
    //     transform = MatrixMultiply(QuaternionToMatrix(quatRot2), MatrixTranslate(x, y, z));
    // }

    for (int i = 0; i < collisions.size(); i++) {
        if (collisions[i]->getMotionState()) {
            btTransform trans;
            collisions[i]->getMotionState()->getWorldTransform(trans);
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
            Matrix transform = MatrixMultiply(QuaternionToMatrix(quatRot2), MatrixTranslate(x, y, z));
            transforms[i] = transform;
        }
    }
}

void Block::Unload(){
    // for (int i = 0; i < collisions.size(); i++) {
    //     delete collisions[i];
    // }
    for (int i = 0; i < cubes.size(); i++) {
        R3D_UnloadModel(&cubes[i], true);
    }
    // R3D_UnloadModel(&cube, true);
    UnloadTexture(texture);
}