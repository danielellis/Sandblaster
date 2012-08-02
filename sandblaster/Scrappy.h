#ifndef SCRAPPY_H
#define SCRAPPY_H

#include "Driver.h"
#include "MeshObject.h"
#include "Shader.h"

class Scrappy : public Driver {
public:
	Scrappy(bool renderParticles = true) : mRenderParticles(renderParticles) {}
    static void InitializeScrappyShaders();
		
    void Initialize(WorldMode *worldMode, int health, int maxHealth, 
        float phi, float theta,
        float boundsRad, float mass, //enum projectile_type primaryWeapon, int secondaryWeaponNum,
        const Vector3 &pos, 
        const Vector3 &prevPos,
		const Vector3 &vel);

    virtual void StrafeForward();
    virtual void StrafeBackward();
    virtual void StrafeLeft();
    virtual void StrafeRight();
		
    virtual void Render();

protected:
	float shift;
    static MeshObject mMeshObject;

    //Profiles
    static CGprofile psScrappyProfile, vsScrappyProfile;

    //Programs
    static CGprogram psScrappyProgram, vsScrappyProgram;

    //Parameters
    static CGparameter texcoord0, texcoord1, textShift, text1, text2, text3, text4, text5, text6;
	static CGparameter vsModelViewProj, vsLightViewProj, lightPosition, psNormalMap, vertin,
			lightViewProj, psLightVector, colorIN;

	bool mRenderParticles;
};

#endif