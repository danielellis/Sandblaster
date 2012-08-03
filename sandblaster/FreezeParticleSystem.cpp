#include "FreezeParticleSystem.h"
#include "Driver.h"

void FreezeParticleSystem::Initialize(Driver *driver) {
    mDriver = driver;
    mMaxParticles = 100;
}

void FreezeParticleSystem::Update(float dt) {
    unsigned i;

    for (i = 0; i < mParticles.size(); i++) {
        if (mParticles[i].IsAlive()) {
            mParticles[i].Update(dt);
        }
    }
}

void FreezeParticleSystem::Render(float x, float y, float z,
				float theta, float thetaX, float thetaY, float thetaZ) {
    for (unsigned i = 0; i < mParticles.size(); i++) {
        if (mParticles[i].IsAlive()) {
            mParticles[i].Render(x,y,z,theta, thetaX, thetaY, thetaZ);
        }
    }
}

void FreezeParticleSystem::Reset() {
	unsigned int i;   
	for (i = 0; i < mParticles.size(); i++) {
		mParticles[i].Reset();
    }

	if(i < mMaxParticles)
	{
		FreezeParticle p;
		p.Initialize(0.7f + 0.3f * (float)rand()/RAND_MAX);
		mParticles.push_back(p);
	}
}
