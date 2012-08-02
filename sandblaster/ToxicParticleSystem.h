#ifndef TOXIC_PARTICLE_SYSTEM
#define TOXIC_PARTICLE_SYSTEM

#include "Vector3.h"
#include "ToxicParticle.h"
#include "MeshObject.h"
#include "Shader.h"
#include <list>

class ToxicParticleSystem {
public:
    static void InitializeToxicParticleSystemShader();

    ToxicParticleSystem();
    void Initialize(const Vector3 &pos, const Vector3 &vel);

    void Update(float dt);
    void Render();

    bool IsAlive() {return mParticleCount > 0;}

protected:
    int mParticleCount;

    std::list<ToxicParticle> mParticles;

    float glow;
	float deformVal;
	float moveGlowVal;
	bool glowDir;

    static MeshObject mToxicParticleMesh;
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
};

#endif