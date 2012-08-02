#include "Particle.h"
#include "SBLib.h"
#include "SDL/SDL_OpenGl.h"
#include <cstdlib>
#include <cmath>

void Particle::Initialize(float dispersion, float speed) {
    mLife = 0;
    mDispersion = dispersion;
    mSpeed = speed;

    mPos = Vector3(0,0,0);

    dispersion = (1-(2.0f*((float)rand())/RAND_MAX)) * (180 *.0174);

    float angle = PI/2.0f + dispersion*mDispersion;

    mVel = Vector3(cos(angle), sin(angle), 1);

    mVel = (((float)rand())/RAND_MAX)*speed*5 * mVel;

    glPointSize(2.0f);
}

void Particle::Update(float dt) {
    mPrevPos = mPos;
    mPos += dt*mVel;
    mLife += dt*2*(((float)rand())/RAND_MAX);
}

void Particle::Render() {
    glBegin(GL_POINTS);
        glVertex3f(mPos.GetX(), mPos.GetY(), mPos.GetZ());
    glEnd();
}