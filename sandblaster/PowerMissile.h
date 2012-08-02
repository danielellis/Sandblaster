#ifndef POWER_MISSILE_H
#define POWER_MISSILE_H

#include "ProjectileWeapon.h"

#include "SDL/SDL_OpenGL.h"

class PowerMissile : public ProjectileWeapon {
public:
    PowerMissile(int meshID) : mMeshID(meshID) {}

    void Initialize(float phi, float theta, const Vector3 &pos, const Vector3 &vel, ProjectileType type) {
        ProjectileWeapon::Initialize(3, 1, phi, theta, pos, vel, type);
    }

    void Update(float dt) {
        mPrevPos = mPos;

        mPos += mVel * dt;
    }

    void Render() {
        glPushMatrix();
		   glTranslatef(mPos.GetX(), mPos.GetY(), mPos.GetZ());
           //glRotatef(-mTheta * 180/PI - 90, 0, 1, 0);
           glCallList(mMeshID);
        glPopMatrix();
    }

private:
    int mMeshID;
};

#endif