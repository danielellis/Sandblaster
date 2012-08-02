#ifndef ICESHOT_H
#define ICESHOT_H

#include "ProjectileWeapon.h"
#include "Shader.h"
#include "MeshObject.h"

class Iceshot : public ProjectileWeapon {
public:
    static void InitializeIceshotShaders();

    void Initialize(float phi, float theta, const Vector3 &pos, const Vector3 &vel) {
		shineVal = 0.3f;
		glitterVal = 0.0f;
		shineDir = true;
		glitterDir = true;
		rotate = 0;
        ProjectileWeapon::Initialize(3, 1, phi, theta, pos, vel, W_ICESHOT);
    }

    void Update(float dt);
    void Render();

protected:
	float shineVal;
	float glitterVal;
	int rotate;
	bool shineDir;
	bool glitterDir;

    //Profiles
    static CGprofile vsIceshotProfile;
    static CGprofile psIceshotProfile;

    //Programs
    static CGprogram vsIceshotProgram;
    static CGprogram psIceshotProgram;

    //Parameters
    static CGparameter	position, texVert, mvMatrix;//, move;	
    static CGparameter	texcoord0, shift;

    //Texture parameters 
    static CGparameter text1, text2, text3;
    
    static MeshObject mIceMesh;
};

#endif