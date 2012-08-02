#ifndef FREEZE_PARTICLE_H
#define FREEZE_PARTICLE_H

#include "Entity.h"
#include "Shader.h"

class FreezeParticle : public Entity {
public:
    void Initialize(float speed);
	static void InitializeFreezeParticleShader();
    void Update(float);
    
	void Render(float x, float y, float z,
				float theta, float thetaX, float thetaY, float thetaZ);
	void Reset();

    bool IsAlive() const {return mLife < 30.0f;}
	
	

protected:
    float mLife;
    float mDispersion;
    float mSpeed;
	float mRotate;

	static CGprogram FreezeParticleProgram;
	static CGprofile FreezeParticleProfile;
	static CGparameter FreezeParticleText1, FreezeParticleCoord0;
};

#endif