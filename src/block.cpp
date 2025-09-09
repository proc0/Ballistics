#include "block.hpp"

void Block::Init(Physics& bullet){
    collision = bullet.CreateBlock(2, 4, -10);

    // for(int i = 4; i == 1; i--){
    //     for(int j = 0; j < i; j++){
    //         collisions.push_back(bullet.CreateBlock(j + 1, i + 1, -10));
    //     }
    // }

    // for (int i = 0; i < collisions.size(); i++) {
    //     if (collisions[i]->getMotionState()) {
    //         btTransform trans;
    //         collisions[i]->getMotionState()->getWorldTransform(trans);
    //         float x = float(trans.getOrigin().getX());
    //         float y = float(trans.getOrigin().getY());
    //         float z = float(trans.getOrigin().getZ());
    
    //         transforms.push_back(MatrixTranslate(x, y, z));
    //     }
    // }
}

void Block::Load(){
    // Create scene objects
    cube = R3D_LoadModel("assets/cube.obj");
    texture = LoadTexture("assets/uvgrid_1024.png");
    cube.materials[0].albedo.texture = texture; 
}

void Block::Render() const {
    // for (int i = 0; i < transforms.size(); i++) {
    //     R3D_DrawModelPro(&cube, transforms[i]);
    // }
    R3D_DrawModelPro(&cube, transform);
}

void Block::Update(){

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
    }

    // for (int i = 0; i < collisions.size(); i++) {
    //     if (collisions[i]->getMotionState()) {
    //         btTransform trans;
    //         collisions[i]->getMotionState()->getWorldTransform(trans);
    //         float x = float(trans.getOrigin().getX());
    //         float y = float(trans.getOrigin().getY());
    //         float z = float(trans.getOrigin().getZ());

    //         btQuaternion quatRot = trans.getRotation();
    //         // transform = MatrixTranslate(x, y, z);
    //         Quaternion quatRot2 = (Quaternion){
    //             x: quatRot.getX(),
    //             y: quatRot.getY(),
    //             z: quatRot.getZ(),
    //             w: quatRot.getW(),
    //         };
    //         // auto quatTrans = QuaternionFromMatrix(MatrixTranslate(x, y, z));
    //         // transform = QuaternionToMatrix(QuaternionMultiply(quatTrans, quatRot2));
    //         Matrix transform = MatrixMultiply(QuaternionToMatrix(quatRot2), MatrixTranslate(x, y, z));
    //         transforms[i] = transform;
    //     }
    // }
}

void Block::Unload(){
    // for (int i = 0; i < collisions.size(); i++) {
    //     delete collisions[i];
    // }
    R3D_UnloadModel(&cube, true);
    UnloadTexture(texture);
}