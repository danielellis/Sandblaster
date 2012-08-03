#ifndef FREEZE_PARTICLE_SYSTEM
#define FREEZE_PARTICLE_SYSTEM

#include "FreezeParticle.h"
#include <vector>

class Driver;

class FreezeParticleSystem {
public:
    void Initialize(Driver *);

    void Update(float);
    void Render(float x, float y, float z,
				float theta, float thetaX, float thetaY, float thetaZ);
	void Reset();

protected:
    Driver *mDriver;

    std::vector<FreezeParticle> mParticles;
    int mParticlesPerSecond;
    float mResidue;
    bool mDontCreate;
    unsigned int mMaxParticles;
};

#endif

