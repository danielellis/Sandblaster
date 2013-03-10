#ifndef WORLD_MODE_H
#define WORLD_MODE_H

#include "GameMode.h"
#include "InputEvent.h"
#include "FractalTerrain.h"
#include "TextureLoader.h"
#include "AI.h"
#include "ObjLoader.h"
#include "Intersection.h"
#include "Skydome.h"
#include "DebugEye.h"
#include "GameOverMode.h"
#include "SmallHealthPowerup.h"
#include "WeaponPowerUp.h"
#include "Frustum.h"
#include "SBLib.h"
#include "ProjectileWeapon.h"
#include "ToxicParticleSystem.h"
#include <list>

class WorldMode : public GameMode {
public:
    WorldMode(GameWindow *window)
        : GameMode(window)
        , mStrafeLeft()
        , mStrafeRight()
        , mStrafeForward()
        , mStrafeBackward()
        , mStrafeUp()
        , mStrafeDown()
        , mMouseLeft(IE_INITIAL)
        , mMouseRight(IE_INITIAL)
        , mPause(IE_INITIAL)
        , mFireCurrentWeapon()
        , mNextWeapon(IE_INITIAL)
        , mPrevWeapon(IE_INITIAL)
        , mIncHealth(IE_INITIAL)
        , mDecHealth(IE_INITIAL)
        , mDebugEyeMode(IE_INITIAL)
        , mDebugTerrain(IE_INITIAL)
        , mNextMode(NULL)
        , mFractalTerrain(&mFrustum)
    { }
    ~WorldMode();

   void SetNeighborModes(const std::vector<GameMode *> &);
   bool LoadResources();
   GameMode *CheckForModeChange() {return mNextMode;}

   void Start();
   void Stop();
   void Reset();

   void Update(float timeElapsed);
   void Render();

   void AddProjectileWeapon(ProjectileType type, float phi, float theta, Vector3 &pos, Vector3 &vel, Vector3 &dir);

   void SetPlayerNdx(int playerNdx) {mPlayerNdx = playerNdx;}

protected:
   enum health_enum {LIFE_GOOD = 0, LIFE_HALF = 1, LIFE_CRIT = 2};
	enum lens_flare_enum {LF_GLOW = 0, LF_HALO1 = 1, LF_HALO2 = 2, LF_HALO3 = 3}; 
   
   void LoadLensFlareAssets();
   void DrawLensFlare();

   void LoadHUDAssets();
   void DrawHUD();

   void HandleCollisions();

   int mPlayerNdx;
   std::vector<Driver *> mDrivers;
   Driver *mPlayer;
   std::vector<Driver *> mAIDrivers;
   std::vector<AI> mAIs;
   std::vector<ProjectileWeapon *> mProjectileWeapons;
   std::vector<FractalTerrain *> mFractalTerrains;
   std::list<ToxicParticleSystem *> mToxicParticleSystems;

   Skydome mSkydome;
   std::vector<SmallHealthPowerUp *> mHealthBoxes;
   std::vector<WeaponPowerUp *> mWeaponBoxes;
   SmallHealthPowerUp h1, h2, h3, h4;
   WeaponPowerUp w1, w2, w3, w4, w5, w6;
   Vector3 v1, v2, v3, v4;
   Vector3 v1b, v2b, v3b, v4b, v5b, v6b;

   Frustum mFrustum;
   DebugEye mDebugEye;
   bool mDebugEyeOn;

   FractalTerrain mFractalTerrain;

   InputEvent mStrafeLeft, mStrafeRight, mStrafeForward, mStrafeBackward, mStrafeUp, mStrafeDown,
      mMouseLeft, mMouseRight, mMouseUp, mMouseDown, mPause, mNextWeapon, mPrevWeapon, 
    mIncHealth, mDecHealth, mFireCurrentWeapon, mDebugEyeMode, mDebugTerrain;

   GameMode *mPauseMenuMode, *mNextMode;
   GameOverMode *mGameOverMode;

   TextureLoader mTextureLoader;
   glTexture mWeaponTextures[NUM_WEAPONS];
   glTexture mLifebarTextures[3];
	glTexture mLensFlareTextures[4];
   glTexture mLifebarCaseTexture;
   glTexture templeTex;
   glTexture mNumberTextures[10];

   ObjMesh *objmesh;
   ObjFace *surfaces[1000];
   ObjFace *walls[1000];
   int num_walls;
   int num_surfaces;

   // PauseMode *mPauseMode;
   // GameOverMode *mGameOverMode;
};

#endif