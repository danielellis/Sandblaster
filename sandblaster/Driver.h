#ifndef DRIVER_H
#define DRIVER_H

#include "Entity.h"
#include "Vector3.h"
#include "DriverParticleSystem.h"
#include "FreezeParticleSystem.h"
#include "FreezeParticle.h"
#include "ProjectileWeapon.h"
#include "Shader.h"
#include "MeshObject.h"

class WorldMode;
class DriverParticleSystem;
class FreezeParticleSystem;

//const float PRIMARY_WEAPON_DELAY = .5;

class Driver : public Entity {
public:

   void Initialize(WorldMode *worldMode, int health, int maxHealth, 
    float phi, float theta,
    float boundsRad, float mass, //enum projectile_type primaryWeapon, int secondaryWeaponNum,
    const Vector3 &pos, 
    const Vector3 &prevPos = Vector3(0,0,0),
    const Vector3 &vel = Vector3(0,0,0));

   void Update(float dt);
   virtual void Render() = 0;

   bool IsCollided(const Driver *, Vector3 &normalOut);
   bool IsCollided(const ProjectileWeapon *);

   void FireCurrentWeapon();
   ProjectileType GetCurrentWeapon() {return mCurrentWeapon;}
   void SetCurrentWeapon(ProjectileType type) {mCurrentWeapon = type;}
   //void SetSecondaryWeapon(int type) {mSecondaryWeaponNum = type;}
   //int GetSecondaryWeapon() {return mSecondaryWeaponNum;}
   int GetCurrentWeaponCount() {return mWeaponCounts[mCurrentWeapon];}
   int GetFireCount() {return mWeaponCounts[W_FIREBALL];}
   int GetToxicCount() {return mWeaponCounts[W_TOXICSHOT];}
   int GetIceCount() {return mWeaponCounts[W_ICESHOT];}
   void SetFireCount(int num) {mWeaponCounts[W_FIREBALL] = num;}
   void SetToxicCount(int num) {mWeaponCounts[W_TOXICSHOT] = num;}
   void SetIceCount(int num) {mWeaponCounts[W_ICESHOT] = num;}
   void NextWeapon();
   void PreviousWeapon();
   void DecrementHealth(int amount);
   void IncrementHealth(int amount);

   virtual void StrafeForward() = 0;
   virtual void StrafeBackward() = 0;
   virtual void StrafeLeft() = 0;
   virtual void StrafeRight() = 0;


   void SetHealth(int health)                       {mHealth = health;}
   void SetMaxHealth(int maxHealth)                 {mMaxHealth = maxHealth;}
   
   int GetHealth()           const {return mHealth;}
   int GetMaxHealth()        const {return mMaxHealth;}
   float GetBoundsRadius() const {return mBoundsRadius;}
   Vector3 GetRayPos() {return mRayPos;}
   Vector3 GetRayDir() {return mRayDir;}
   float GetMinX() {return xmin;}
   float GetMaxX() {return xmax;}
   float GetMinY() {return ymin;}
   float GetMaxY() {return ymax;}
   float GetMinZ() {return zmin;}
   float GetMaxZ() {return zmax;}

   bool IsAlive() const {return mHealth > 0;}
   bool OnPlatform() {return onPlatform;}
   void SetOnPlatform(bool on) {onPlatform = on;}
   bool IsFrozen() {return frozen;}
   void SetFrozen(bool set) {frozen = set;}
   bool IsBurned() {return burned;}
   void SetBurned(bool set) {burned = set;}

   void FreezeEffectBegin();
   void FreezeEffectEnd();
   void BurnEffectBegin();
   void BurnEffectEnd();
   static void InitializeWeaponEffects();


protected:
 
   int mHealth, mMaxHealth;
   //int mPrimaryWeaponNum;
   int mWeaponCounts[NUM_WEAPONS];
   ProjectileType mCurrentWeapon;
   float mBoundsRadius;
   float xmin, xmax, ymin, ymax, zmin, zmax;
   Vector3 mRayPos, mRayDir;
   bool onPlatform;
   bool frozen;
   float freezeCounter;
   bool burned;
   float burnCounter;

   DriverParticleSystem mDriverParticleSystem;

   WorldMode *mWorldMode;

   float mTimeSinceWeaponFire;

   //Freeze Effect
    static MeshObject FreezeEffectMesh;
    static CGprofile psFreezeEffectProfile, vsFreezeEffectProfile;
    static CGprogram psFreezeEffectProgram, vsFreezeEffectProgram;
    static CGparameter vsFreezeEffectTextcoord, vsFreezeEffectPosition,
					vsFreezeEffectMVMatrix, vsFreezeEffectGrow;
	static CGparameter FreezeEffectText1, FreezeEffectText2, psFreezeEffectTextcoord;
    float FreezeEffectGrow;
	FreezeParticleSystem mFreezeParticleSystem;
};

#endif