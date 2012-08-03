#include "DriverParticleSystem.h"
#include "Driver.h"

void DriverParticleSystem::Initialize(Driver *driver) {
	mDriver = driver;
	mMaxParticles = 300;
}

void DriverParticleSystem::Update(float dt) {
	int numParticlesToCreate;
	unsigned i;
	Particle p;

	numParticlesToCreate = (int)floor(mDriver->GetVel().DotProduct(mDriver->GetVel()) * 100);

	for (i = 0; i < mParticles.size(); i++) {
		if (mParticles[i].IsAlive()) {
			mParticles[i].Update(dt);
		}
		else if (numParticlesToCreate > 0) {
			mParticles[i].Initialize((float)rand()/RAND_MAX, (float)rand()/RAND_MAX);
			numParticlesToCreate--;
		}
	}

	while (numParticlesToCreate > 0
		&& mParticles.size() < mMaxParticles)
	{
		p.Initialize((float)rand()/RAND_MAX, (float)rand()/RAND_MAX);
		mParticles.push_back(p);
		numParticlesToCreate--;
	}
}

void DriverParticleSystem::Render() {
	for (unsigned i = 0; i < mParticles.size(); i++) {
		if (mParticles[i].IsAlive()) {
			mParticles[i].Render();
		}
	}
}