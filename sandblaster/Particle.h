#ifndef PARTICLE_H
#define PARTICLE_H

#include "Entity.h"

class Particle : public Entity {
public:
    void Initialize(float dispersion, float speed);

    void Update(float);
    void Render();

    bool IsAlive() const {return mLife < 1.0f;}
protected:
    float mLife;
    float mDispersion;
    float mSpeed;
};

#endif