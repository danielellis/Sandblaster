#ifndef TOXIC_PARTICLE
#define TOXIC_PARTICLE

#include "Vector3.h"

struct ToxicParticle {
    void Update(float dt);

    bool IsAlive() {return mLife < 1.0f;}

    Vector3 mPosition, mVelocity;
    float mLife;

    static Vector3 GRAVITY;

};

#endif
