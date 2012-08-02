#ifndef SARGE_H
#define SARGE_H

#include "Driver.h"
#include "MeshObject.h"
#include "Shader.h"
#include "TextureLoader.h"
//#include "WorldMode.h"

class WorldMode;

class Sarge : public Driver {
public:
	Sarge(bool renderParticles = true) : mRenderParticles(renderParticles) {}
    static void InitializeSargeShaders();

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
    static MeshObject mMeshObject;

    //Profiles
    static CGprofile vsSargeProfile;
	static CGprofile psSargeProfile;


    //Programs
    static CGprogram vsSargeProgram;
	static CGprogram psSargeProgram;

    //Parameters
    //static CGparameter texcoord0, text1;
	static CGparameter	modelViewProj, lightViewProj, lightPosition, eyevector, normalmap, decalmap, vertin, fragin;

	//textures
	static glTexture sargeColorTexture;
	static glTexture sargeNormalTexture;

	bool mRenderParticles;

	//static WorldMode *mWorldMode;
};

#endif