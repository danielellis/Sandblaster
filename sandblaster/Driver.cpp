#include "Driver.h"
#include "WorldMode.h"
#include "SBLib.h"
#include "Audio.h"
#include <SDL/SDL_OpenGL.h>
#include <iostream>
#include "BMPLoader.h"

using namespace std;

//freeze effect
CGprofile Driver::psFreezeEffectProfile, Driver::vsFreezeEffectProfile;
CGprogram Driver::psFreezeEffectProgram, Driver::vsFreezeEffectProgram;
CGparameter Driver::vsFreezeEffectTextcoord, Driver::vsFreezeEffectPosition,
		Driver::vsFreezeEffectMVMatrix, Driver::vsFreezeEffectGrow;
CGparameter Driver::FreezeEffectText1, Driver::FreezeEffectText2, Driver::psFreezeEffectTextcoord;

MeshObject Driver::FreezeEffectMesh("assets/models/iceshot.obj");

void Driver::Initialize(WorldMode *worldMode, int health, int maxHealth, 
    float phi, float theta,
    float boundsRad, float mass, //enum projectile_type primaryWeapon, int secondaryWeaponNum,
    const Vector3 &pos, 
    const Vector3 &prevPos,
    const Vector3 &vel)
{
   mWorldMode = worldMode;
   mHealth = health;
   mMaxHealth = maxHealth;
   mBoundsRadius = boundsRad; 
   mCurrentWeapon = W_FIREBALL;
   //mSecondaryWeaponNum = secondaryWeaponNum;
   mWeaponCounts[W_FIREBALL] = 8;
   mWeaponCounts[W_TOXICSHOT] = 0;
   mWeaponCounts[W_ICESHOT] = 1;
   Entity::Initialize(phi, theta, mass, (float)0.12, pos, prevPos, vel);
   mDriverParticleSystem.Initialize(this);
   mTimeSinceWeaponFire = ProjectileWeaponDelays[mCurrentWeapon];//PRIMARY_WEAPON_DELAY;
   mRayDir = Vector3(0,-1,0);
   xmin = mPos.GetX() - 1.0f;
   xmax = mPos.GetX() + 1.0f;
   ymin = mPos.GetY() - 0.5f;
   ymax = mPos.GetY() + 0.5f;
   zmin = mPos.GetZ() + 2.0f;
   zmax = mPos.GetZ() - 2.0f;
   frozen = false;
   burned = false;
   freezeCounter = 0;
   burnCounter = 0;
   FreezeEffectGrow = 0;
   mFreezeParticleSystem.Initialize(this);
   FreezeParticle::InitializeFreezeParticleShader();
}

void Driver::Update(float dt) {
    Vector3 acceleration, vVelDir;

	if (frozen)
	{
		freezeCounter+=dt;	
	}

	if (burned)
	{
		burnCounter+=dt;
	}

	if (freezeCounter > 8.0f)
	{
		//Reset particle system
		freezeCounter = 0;
		frozen = false;
	}

	if (burnCounter > 8.0f)
	{
		//Reset particle system
		burnCounter = 0;
		burned = false;
	}



	if ((mTheta - mCamTheta) > 0)
		RotateCamYaw(1.0f - fabs(mDir.DotProduct(camDir)));
	else if ((mTheta - mCamTheta) < 0)
		RotateCamYaw(-(1.0f - fabs(mDir.DotProduct(camDir))));

    vVelDir = mVel.Normalized();
    mPrevPos = mPos;

    acceleration = (mForce - mDrag * mVel.DotProduct(mVel) * vVelDir)/mMass;
    mPos += mVel * dt + 0.5 * acceleration * dt * dt;

    mVel += acceleration * dt;

    if (mVel.Length() < 1) {
        mVel.Zero();
    }

	if (mPos.GetY() < 1.5)
	{
		mPos.SetY(1.5);
	}

    mDriverParticleSystem.Update(dt);

    /*
    if (mVel.Length() > 10) {
        mVel = 10 * vDir;
    }
    */

    //cout << "velocity: " << mVel << endl;

	mRayPos = mPos;

	xmin = mPos.GetX() - 1.0f;
	xmax = mPos.GetX() + 1.0f;
	ymin = mPos.GetY() - 0.5f;
	ymax = mPos.GetY() + 0.5f;
	zmin = mPos.GetZ() + 2.0f;
	zmax = mPos.GetZ() - 2.0f;

    mForce.Zero();

    mTimeSinceWeaponFire += dt;
}

bool Driver::IsCollided(const Driver *oth, Vector3 &normalOut) {
    Vector3 separation = mPos - oth->mPos;

    separation.SetY(0);
    normalOut = separation.Normalized();

    return separation.DotProduct(separation) < (mBoundsRadius + oth->mBoundsRadius) * (mBoundsRadius + oth->mBoundsRadius);
}

bool Driver::IsCollided(const ProjectileWeapon *oth) {
    Vector3 separation = mPos - oth->GetPos();

    return separation.DotProduct(separation) < (mBoundsRadius + oth->GetBoundsRadius()) * (mBoundsRadius + oth->GetBoundsRadius());
}

void Driver::FireCurrentWeapon() {
   if (mTimeSinceWeaponFire >= ProjectileWeaponDelays[mCurrentWeapon] && mWeaponCounts[mCurrentWeapon] > 0) {
      mWorldMode->AddProjectileWeapon(mCurrentWeapon, mPhi, mTheta, mPos + mBoundsRadius*mDir, mVel, mDir);
      //TODO change the sounds and volumes for sound effects
      playSound("assets/sfx/fire_primary.wav", 0.3);
      mTimeSinceWeaponFire = 0;

	   --mWeaponCounts[mCurrentWeapon];
   }
}

void Driver::DecrementHealth(int amount) {
   mHealth = (mHealth - amount < 0) ? 0 : mHealth - amount;
   playSound("assets/sfx/health_down.wav", 0.3); 
}

void Driver::IncrementHealth(int amount) {
   mHealth = (mHealth + amount > mMaxHealth) ? mMaxHealth : mHealth + amount;
   playSound("assets/sfx/health_up.wav", 0.3); 
}

void Driver::NextWeapon() {
	switch(mCurrentWeapon)
	{
	case W_FIREBALL:
		mCurrentWeapon = W_ICESHOT;
		break;
	case W_ICESHOT:
		mCurrentWeapon = W_TOXICSHOT;
		break;
	case W_TOXICSHOT:
		mCurrentWeapon = W_FIREBALL;
		break;
	}
   playSound("assets/sfx/weapon_switch_up.wav", AUDIO_LOCAL); 
}


void Driver::PreviousWeapon() {
	switch(mCurrentWeapon)
	{
	case W_FIREBALL:
		mCurrentWeapon = W_TOXICSHOT;
		break;
	case W_ICESHOT:
		mCurrentWeapon = W_FIREBALL;
		break;
	case W_TOXICSHOT:
		mCurrentWeapon = W_ICESHOT;
		break;
	}
	playSound("assets/sfx/weapon_switch_down.wav", AUDIO_LOCAL); 
}

void Driver::FreezeEffectBegin()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);	
	glEnable(GL_BLEND);

	glPushMatrix();
	cgGLEnableProfile(psFreezeEffectProfile);
    cgGLBindProgram(psFreezeEffectProgram);
	
	cgGLEnableProfile(vsFreezeEffectProfile);
    cgGLBindProgram(vsFreezeEffectProgram);

	if(freezeCounter <= 8.0f/3.0f)
	{
		FreezeEffectGrow = freezeCounter;
	}
	else if(freezeCounter <= 16.0f/3.0f)
	{
		//FreezeEffectGrow
		mFreezeParticleSystem.Reset();
	}
	else if(freezeCounter <= 8.0f)
	{
		mFreezeParticleSystem.Update(1);
		FreezeEffectGrow = 8.0f - freezeCounter;
	}

	cgSetParameter1f(vsFreezeEffectGrow,FreezeEffectGrow);
	cgGLEnableTextureParameter(FreezeEffectText1);
	cgGLEnableTextureParameter(FreezeEffectText2);
}


void Driver::FreezeEffectEnd()
{
    if (freezeCounter < (20.0f/3.0f))
        FreezeEffectMesh.GLDraw();	
	glPopMatrix();


	cgGLDisableProfile(vsFreezeEffectProfile);
	cgGLDisableProfile(psFreezeEffectProfile);
}

void Driver::BurnEffectBegin()
{

}

void Driver::BurnEffectEnd()
{

}


void Driver::InitializeWeaponEffects()
{
	  GLuint FreezeEffectTexIDs[2];
    BMPImage *FreezeEffectTexture1, *FreezeEffectTexture2;

    //Vertext Shader profile
    vsFreezeEffectProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
    //Set profile
    cgGLSetOptimalOptions(vsFreezeEffectProfile);
    CheckForCgError("ice shot", "selecting vertex profile");

    //Load vertex shader
    vsFreezeEffectProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "vertexshaders/vs_freezeeffect.cg", vsFreezeEffectProfile, "vertMain", 0);

    CheckForCgError("ice shot", "creating vertex program from file");
    cgGLLoadProgram(vsFreezeEffectProgram);
    CheckForCgError("ice shot", "loading vertex program");

    
    vsFreezeEffectTextcoord			= cgGetNamedParameter(vsFreezeEffectProgram, "texCoordIN");
    vsFreezeEffectPosition			= cgGetNamedParameter(vsFreezeEffectProgram, "posIN");
    vsFreezeEffectMVMatrix			= cgGetNamedParameter(vsFreezeEffectProgram, "ModelViewProj");
	vsFreezeEffectGrow				= cgGetNamedParameter(vsFreezeEffectProgram, "grow");
    
    /////////////////////////////
    //Pixel Shader
	  
    psFreezeEffectProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(psFreezeEffectProfile);
    CheckForCgError("ice shot", "selecting fragment profile");

    psFreezeEffectProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "pixelshaders/ps_freezeeffect.cg", psFreezeEffectProfile, "pixelMain", 0);
    CheckForCgError("ice shot", "creating fragment program from file");
    cgGLLoadProgram(psFreezeEffectProgram);
    CheckForCgError("ice shot", "loading fragment program");
	
    glGenTextures(2, FreezeEffectTexIDs);
 
	psFreezeEffectTextcoord			= cgGetNamedParameter(vsFreezeEffectProgram, "texCoordIN");
    
    FreezeEffectTexture1 = BMPLoader::LoadTexture("assets/textures/weapon/freeze1.bmp", FreezeEffectTexIDs[0]);
    FreezeEffectTexture2 = BMPLoader::LoadTexture("assets/textures/weapon/freeze2.bmp", FreezeEffectTexIDs[1]);

    FreezeEffectText1 = cgGetNamedParameter(psFreezeEffectProgram, "text1");
    cgGLSetTextureParameter(FreezeEffectText1, FreezeEffectTexIDs[0]);
    FreezeEffectText2 = cgGetNamedParameter(psFreezeEffectProgram, "text2");
    cgGLSetTextureParameter(FreezeEffectText2, FreezeEffectTexIDs[1]);
    CheckForCgError("ice shot", "setting decal 2D texture");

   delete FreezeEffectTexture1->data;
   delete FreezeEffectTexture2->data;
   delete FreezeEffectTexture1;
   delete FreezeEffectTexture2;
}