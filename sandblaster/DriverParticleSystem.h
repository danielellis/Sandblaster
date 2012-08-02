#ifndef DRIVER_PARTICLE_SYSTEM
#define DRIVER_PARTICLE_SYSTEM

#include "Particle.h"
#include <vector>

class Driver;

class DriverParticleSystem {
public:
    void Initialize(Driver *);

    void Update(float);
    void Render();

protected:
    Driver *mDriver;

    std::vector<Particle> mParticles;
    int mParticlesPerSecond;
    float mResidue;
    bool mDontCreate;
    int mMaxParticles;
};

#endif