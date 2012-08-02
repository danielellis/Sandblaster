#ifndef FIREBALL_H
#define FIREBALL_H

#include "ProjectileWeapon.h"
#include "Shader.h"
#include "MeshObject.h"

class Fireball : public ProjectileWeapon {
public:
    static void InitializeFireBallShaders();

    void Initialize(float phi, float theta, const Vector3 &pos, const Vector3 &vel) {
        mMove = 0;
        mMoveDir = true;
        ProjectileWeapon::Initialize(3, 1, phi, theta, pos, vel, W_FIREBALL);
    }

    void Update(float dt);
    void Render();

protected:
    float mMove;
    bool mMoveDir;

    //Profiles
    static CGprofile vsFireballProfile;
    static CGprofile psFireballProfile;

    //Programs
    static CGprogram vsFireballProgram;
    static CGprogram psFireballProgram;

    //Parameters
    static CGparameter	position, texVert, mvMatrix, move;	
    static CGparameter	texcoord0, lightdist;

    //Texture parameters 
    static CGparameter text1, text2;
    
    static MeshObject mFireballMesh;
};

#endif