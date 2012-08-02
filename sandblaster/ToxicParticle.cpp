#include "ToxicParticle.h"
#include <cstdlib>

Vector3 ToxicParticle::GRAVITY(0,-2,0);

void ToxicParticle::Update(float dt) {
    mPosition += mVelocity * dt;

    mVelocity += dt*GRAVITY;
    
    mLife += (rand()/RAND_MAX) * dt;
}
