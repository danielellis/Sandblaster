#ifndef TOXICSHOT_H
#define TOXICSHOT_H

#include "ProjectileWeapon.h"
#include "MeshObject.h"
#include "Shader.h"

class Toxicshot : public ProjectileWeapon {
public:
    static void InitializeToxicshotShaders();

    void Initialize(float phi, float theta, const Vector3 &pos, const Vector3 &vel) {
		glow = 0.3f;
		glowDir = true;
		deformVal = 0.0f;
		moveGlowVal = 0.0f;
        ProjectileWeapon::Initialize(3, 1, phi, theta, pos, vel, W_TOXICSHOT);
    }

    void Update(float dt);
    void Render();

protected:
	float glow;
	float deformVal;
	float moveGlowVal;
	bool glowDir;

    //Profiles
    static CGprofile vsToxicshotProfile;
    static CGprofile psToxicshotProfile;

    //Programs
    static CGprogram vsToxicshotProgram;
    static CGprogram psToxicshotProgram;

    //Parameters

	static CGparameter vs_texcoord0, position, vs_color , deform, mvMatrix;
	static CGparameter ps_color, ps_texcoord0, useColor, move, alpha;

    //Texture parameters 
    static CGparameter text1;
    
    static MeshObject mToxicShotMesh;
};

#endif