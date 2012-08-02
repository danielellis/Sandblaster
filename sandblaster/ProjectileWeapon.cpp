#include "ProjectileWeapon.h"

void ProjectileWeapon::Initialize(int damage, int boundsRad, float phi, float theta, 
    const Vector3 &pos, const Vector3 &vel, ProjectileType type) {
    mDamage = damage;
    mBoundsRadius = boundsRad;
    mIsActive = true;
    mIsDead = false;
	mWeaponType = type;

    Entity::Initialize(phi, theta, 1, 0, pos, pos, vel);
}