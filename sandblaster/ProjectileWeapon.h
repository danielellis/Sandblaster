#ifndef PROJECTILE_WEAPON_H
#define PROJECTILE_WEAPON_H

#include "Entity.h"

enum ProjectileType {W_FIREBALL = 0, W_ICESHOT = 1, W_TOXICSHOT = 2};
const float ProjectileWeaponDelays[] = {0.3f, 1.0f, 0.5f};
const int NUM_WEAPONS = 3;

class ProjectileWeapon : public Entity {
public:
    void Initialize(int damage, int boundsRad, float phi, float theta, 
        const Vector3 &pos, const Vector3 &vel, ProjectileType type);

    virtual void Render() = 0;

    bool IsActive() const {return mIsActive;}
    bool IsDead() const {return mIsDead;}
    int  GetDamage() const {return mDamage;}
    double  GetBoundsRadius() const {return mBoundsRadius;}
	ProjectileType GetWeaponType() {return mWeaponType;}

    void MadeImpact() {mIsActive = mIsDead = false;}

protected:
    int mDamage;
    double mBoundsRadius;
	ProjectileType mWeaponType;

    bool mIsActive;
    bool mIsDead;
};

#endif