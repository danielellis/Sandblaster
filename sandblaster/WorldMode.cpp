#include <windows.h>
#include <GL/glee.h>
#include <SDL/SDL.h>
#include <SDL/SDL_OpenGL.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cassert>

#include "Entity.h"
#include "WorldMode.h"
#include "Materials.h"
#include "GameWindow.h"
#include "Audio.h"
#include "FractalTerrain.h"
#include "SBLib.h"
#include "Fireball.h"
#include "Iceshot.h"
#include "Toxicshot.h"
#include "Intersection.h"
#include "ObjLoader.h"
#include "Sarge.h"
#include "Scrappy.h"
#include "FreezeParticle.h"

using namespace std;

WorldMode::~WorldMode() {
	for (int i = 0; i < NUM_WEAPONS; ++i) {
		mTextureLoader.FreeTexture(&mWeaponTextures[i]);
	}
	for (int i = 0; i < 3; ++i) {
		mTextureLoader.FreeTexture(&mLifebarTextures[i]);
	}
	mTextureLoader.FreeTexture(&mLifebarCaseTexture);

	for (int i = 0; i < 10; ++i) {
		mTextureLoader.FreeTexture(&mNumberTextures[i]);
	}
}


void WorldMode::SetNeighborModes(const vector<GameMode *> &modes) {
	mPauseMenuMode = modes[0];
	mGameOverMode = dynamic_cast<GameOverMode *>(modes[1]);
}

bool WorldMode::LoadResources() {

	num_walls = 0;
	num_surfaces = 0;

	Fireball::InitializeFireBallShaders();
	Iceshot::InitializeIceshotShaders();
	Toxicshot::InitializeToxicshotShaders();
	Skydome::InitializeSkydomeShaders();
	Sarge::InitializeSargeShaders();
	Scrappy::InitializeScrappyShaders();
	Driver::InitializeWeaponEffects();
	FreezeParticle::InitializeFreezeParticleShader();


	//load temple obj
	objmesh = LoadOBJ("models/temple.obj");

	for (unsigned int i = 0; i < objmesh->m_iNumberOfFaces; i++)
	{
		ObjFace *pf = &objmesh->m_aFaces[i];

		if (objmesh->m_aNormalArray[pf->m_aNormalIndices[0]].y > 0
			|| objmesh->m_aNormalArray[pf->m_aNormalIndices[1]].y > 0
			|| objmesh->m_aNormalArray[pf->m_aNormalIndices[2]].y > 0)
		{
			surfaces[num_surfaces] = pf;
			num_surfaces++;
		}
	}

	for (unsigned int j = 0; j < objmesh->m_iNumberOfFaces; j++)
	{
		ObjFace *pf = &objmesh->m_aFaces[j];

		if (objmesh->m_aNormalArray[pf->m_aNormalIndices[0]].y >= -0.05 && objmesh->m_aNormalArray[pf->m_aNormalIndices[0]].y < 0.05
			|| objmesh->m_aNormalArray[pf->m_aNormalIndices[1]].y >= -0.05 && objmesh->m_aNormalArray[pf->m_aNormalIndices[0]].y < 0.05
			|| objmesh->m_aNormalArray[pf->m_aNormalIndices[2]].y >= -0.05 && objmesh->m_aNormalArray[pf->m_aNormalIndices[0]].y < 0.05)
		{
			walls[num_walls] = pf;
			num_walls++;
		}
	}

	mTextureLoader.LoadTextureFromDisk("buildingtextures/templetex.tga", &templeTex);

	// Load the graphics for 2D top layer effects (Lens flare and HUD)
	LoadLensFlareAssets();
	LoadHUDAssets();

	mDrivers.push_back(new Sarge());
	//mDrivers[SARGE]->Initialize(this, 100, 100, 0, -PI/2.0f, 3, 1, 
	// W_FIREBALL, 0, Vector3(-50,1.5,50), Vector3(0,1,0), Vector3(0,0,0));

	mDrivers.push_back(new Scrappy());
	//mDrivers[SCRAPPY]->Initialize(this, 100, 100, 0, -PI/2.0f, 3, 1, 
	// W_FIREBALL, 0, Vector3(-50,1.5,-50), Vector3(0,1,-50), Vector3(0,0,0));

	mDrivers.push_back(new Sarge());

	//mDrivers.push_back(new Scrappy());

	//mDrivers.push_back(new Tank());
	mAIs.push_back(AI(mDrivers[SARGE]));
	mAIs.push_back(AI(mDrivers[SCRAPPY]));
	mAIs.push_back(AI(mDrivers[2]));
	//mAIs.push_back(AI(mDrivers[3]));



	//mAIs.push_back(AI(mDrivers[TANK]));

	mFractalTerrains.push_back(&mFractalTerrain);

	mSkydome.Initialize(128.0f, 5.0f, 5.0f, 1.0f, 1.0f);
	v1.SetX(0.0);
	v1.SetY(1.0);
	v1.SetZ(-80.0);
	h1.Initialize(v1, 3.0);
	v2.SetX(90.03f);
	v2.SetY(1.5);
	v2.SetZ(29.65f);
	h2.Initialize(v2, 3.0);
	v3.SetX(-1.89f);
	v3.SetY(6.5);
	v3.SetZ(49.71f);
	h3.Initialize(v3, 3.0);
	v4.SetX(0.13f);
	v4.SetY(6.35f);
	v4.SetZ(24.37f);
	h4.Initialize(v4, 3.0);
	mHealthBoxes.push_back(&h1);
	mHealthBoxes.push_back(&h2);
	mHealthBoxes.push_back(&h3);
	mHealthBoxes.push_back(&h4);

	v1b.SetX(20.0);
	v1b.SetY(1.0);
	v1b.SetZ(-20.0);
	w1.Initialize(v1b, 3.0);
	v2b.SetX(0);
	v2b.SetY(1.0);
	v2b.SetZ(80.65f);
	w2.Initialize(v2b, 3.0);
	v3b.SetX(-1.89f);
	v3b.SetY(20.0);
	v3b.SetZ(0);
	w3.Initialize(v3b, 3.0);
	v4b.SetX(80.0);
	v4b.SetY(1.0);
	v4b.SetZ(0.0);
	w4.Initialize(v4b, 3.0);
	v5b.SetX(-30.0);
	v5b.SetY(6.5);
	v5b.SetZ(0);
	w5.Initialize(v5b, 3.0);
	v6b.SetX(-30.0);
	v6b.SetY(1.0);
	v6b.SetZ(95.0);
	w6.Initialize(v6b, 3.0);
	mWeaponBoxes.push_back(&w1);
	mWeaponBoxes.push_back(&w2);
	mWeaponBoxes.push_back(&w3);
	mWeaponBoxes.push_back(&w4);
	mWeaponBoxes.push_back(&w5);
	mWeaponBoxes.push_back(&w6);

	mDebugEyeOn = false;

	return true;
}

void WorldMode::Reset() {
	vector<ProjectileWeapon *>::iterator pwItr;

	mDrivers[SARGE]->Initialize(this, 100, 100, 0, -PI/2.0f, 3, 1, 
		/*W_FIREBALL, 0,*/ Vector3(-50,1.5,50), Vector3(0,1,0), Vector3(0,0,0));
	mDrivers[SCRAPPY]->Initialize(this, 100, 100, 0, PI/2.0f, 3, 1, 
		/*W_FIREBALL, 0,*/ Vector3(-50,1.5,-50), Vector3(0,1,-50), Vector3(0,0,0));
	mDrivers[2]->Initialize(this, 100, 100, 0, PI/2.0f, 3, 1, 
		/*W_FIREBALL, 0,*/ Vector3(50,1.5,-50), Vector3(0,1,-50), Vector3(0,0,0));
	//   mDrivers[3]->Initialize(this, 100, 100, 0, PI/2.0f, 3, 1, 
	//    /*W_FIREBALL, 0,*/ Vector3(50,1.5,50), Vector3(0,1,-50), Vector3(0,0,0));

	mSkydome.Initialize(128.0f, 5.0f, 5.0f, 1.0f, 1.0f);

	mHealthBoxes.push_back(&h1);
	mHealthBoxes.push_back(&h2);
	mHealthBoxes.push_back(&h3);
	mHealthBoxes.push_back(&h4);

	mWeaponBoxes.push_back(&w1);
	mWeaponBoxes.push_back(&w2);
	mWeaponBoxes.push_back(&w3);

	//Fireball::InitializeFireBallShaders();
	//Skydome::InitializeSkydomeShaders();
	//Sarge::InitializeSargeShaders();

	for (pwItr = mProjectileWeapons.begin(); pwItr != mProjectileWeapons.end(); pwItr++)
		(*pwItr)->MadeImpact();
}

void WorldMode::Start() {
	mNextMode = NULL;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//mPlayer.Initialize(this, 100, 100, 0, -PI/2.0f, 3, 1, PW_POWER_MISSLE, 0, Vector3(-50,1.5,50), Vector3(0,1,0), Vector3(0,0,0));
	//mAIDriver.Initialize(this, 100, 100, 0, -PI/2.0f, 3, 1, PW_POWER_MISSLE, 0, Vector3(-50,1.5,-50), Vector3(0,1,-50), Vector3(0,0,0));
	//mSkydome.Initialize(128.0f, 5.0f, 5.0f, 1.0f, 1.0f);

	//Fireball::InitializeFireBallShaders();
	//Skydome::InitializeSkydomeShaders();
	//Sarge::InitializeSargeShaders();

	//for (pwItr = mProjectileWeapons.begin(); pwItr != mProjectileWeapons.end(); pwItr++)
	//   (*pwItr)->MadeImpact();

	InputManager *im = mGameWindow->GetInputManager();

	im->AddKeyEvent(&mStrafeLeft, SDLK_a);
	im->AddKeyEvent(&mStrafeRight, SDLK_d);
	im->AddKeyEvent(&mStrafeBackward, SDLK_s);
	im->AddKeyEvent(&mStrafeForward, SDLK_w);
	im->AddKeyEvent(&mPause, SDLK_ESCAPE);
	im->AddKeyEvent(&mDebugEyeMode, SDLK_m);
	im->AddKeyEvent(&mDebugTerrain, SDLK_l);
	im->AddKeyEvent(&mStrafeUp, SDLK_r);
	im->AddKeyEvent(&mStrafeDown, SDLK_f);

	im->AddMouseEvent(&mFireCurrentWeapon, IM_MOUSE_LEFT_BUTTON);
	im->AddMouseEvent(&mMouseLeft, IM_MOUSE_MOVE_LEFT);
	im->AddMouseEvent(&mMouseRight, IM_MOUSE_MOVE_RIGHT);
	im->AddMouseEvent(&mMouseUp, IM_MOUSE_MOVE_UP);
	im->AddMouseEvent(&mMouseDown, IM_MOUSE_MOVE_DOWN);

	im->AddMouseEvent(&mNextWeapon, IM_MOUSE_WHEELUP_BUTTON);
	im->AddMouseEvent(&mPrevWeapon, IM_MOUSE_WHEELDOWN_BUTTON);

	im->AddKeyEvent(&mIncHealth, SDLK_EQUALS);
	im->AddKeyEvent(&mDecHealth, SDLK_MINUS);

	im->ResetAllInputEvents();

	SDL_ShowCursor(false);
	SDL_WM_GrabInput(SDL_GRAB_ON);

	glViewport(0, 0, mGameWindow->mScreen->w, mGameWindow->mScreen->h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, mGameWindow->mScreen->w / (float)mGameWindow->mScreen->h, 0.1f, 500.0f);
}

void WorldMode::Stop() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	SDL_ShowCursor(true);
	SDL_WM_GrabInput(SDL_GRAB_OFF);
}

void WorldMode::Update(float timeElapsed) {
	int mouseHorzMovement, mouseVertMovement;
	vector<Driver *>::iterator driverItr;
	vector<AI *>::iterator aiItr;
	vector<ProjectileWeapon *>::iterator pwItr;
	vector<SmallHealthPowerUp *>::iterator hbItr;
	vector<WeaponPowerUp *>::iterator weapItr;

	if (mDebugEyeMode.IsPressed())
		mDebugEyeOn = !mDebugEyeOn;
	if (mDebugTerrain.IsPressed())
		mFractalTerrain.ChangeRenderMode();

	//Handle player input
	if (mPause.IsPressed())
		mNextMode = mPauseMenuMode;
	if (mStrafeForward.IsPressed()) {
		if (mDebugEyeOn)
			mDebugEye.MoveForward();
		else
			if (!mDrivers[mPlayerNdx]->IsFrozen())
				mDrivers[mPlayerNdx]->StrafeForward();
	}
	if (mStrafeBackward.IsPressed()) {
		if (mDebugEyeOn)
			mDebugEye.MoveBackward();
		else
			if (!mDrivers[mPlayerNdx]->IsFrozen())
				mDrivers[mPlayerNdx]->StrafeBackward();
	}
	if (mStrafeRight.IsPressed()) {
		if (mDebugEyeOn)
			mDebugEye.MoveRight();
		else
			if (!mDrivers[mPlayerNdx]->IsFrozen())
				mDrivers[mPlayerNdx]->StrafeRight();
	}
	if (mStrafeLeft.IsPressed()) {
		if (mDebugEyeOn)
			mDebugEye.MoveLeft();
		else
			if (!mDrivers[mPlayerNdx]->IsFrozen())
				mDrivers[mPlayerNdx]->StrafeLeft();
	}
	if (mStrafeUp.IsPressed()) {
		if (mDebugEyeOn)
			mDebugEye.MoveUp();
	}
	if (mStrafeDown.IsPressed()) {
		if (mDebugEyeOn)
			mDebugEye.MoveDown();
	}

	if (mNextWeapon.IsPressed()) {
		mDrivers[mPlayerNdx]->NextWeapon();
	}
	if (mPrevWeapon.IsPressed()) {
		mDrivers[mPlayerNdx]->PreviousWeapon();
	}
	if (mFireCurrentWeapon.IsPressed()) {
		if (!mDrivers[mPlayerNdx]->IsFrozen())
			mDrivers[mPlayerNdx]->FireCurrentWeapon();
	}

	//Handle player look direction
	mouseHorzMovement = mMouseRight.GetAmount() - mMouseLeft.GetAmount();

	if (mDebugEyeOn) {
		mouseVertMovement = mMouseUp.GetAmount() - mMouseDown.GetAmount();
		mDebugEye.RotateYaw(mouseHorzMovement * PI / mGameWindow->GetWidth());
		mDebugEye.RotatePitch(mouseVertMovement * PI/ mGameWindow->GetHeight());
		mDebugEye.Update(timeElapsed);
	}
	else {
		if (!mDrivers[mPlayerNdx]->IsFrozen())
		{
			mDrivers[mPlayerNdx]->RotateYaw(mouseHorzMovement * PI / mGameWindow->GetWidth());
			mDrivers[mPlayerNdx]->RotateCamYaw(mouseHorzMovement * PI / mGameWindow->GetWidth());
		}
	}

	//Let AI make a decision
	for (int i = 0; i < NUM_CHARACTERS; ++i) {
		if (i != mPlayerNdx && mDrivers[i]->GetHealth() > 0) {
			mAIs[i].MakeDecision(mDrivers[mPlayerNdx]);
		}
	}

	//cout << "DebugMode: " << mDebugEyeOn << " phealth: " << mPlayer.GetHealth() << " aihealth: " << mAIDriver.GetHealth() << endl;
	// Update Drivers and WeaponProjectiles
	for (driverItr = mDrivers.begin(); driverItr != mDrivers.end(); driverItr++) {
		(*driverItr)->Update(timeElapsed);
	}

	bool allAIsDead = true;
	for (int i = 0; i < NUM_CHARACTERS; ++i) {
		if (i == mPlayerNdx && mDrivers[i]->GetHealth() <= 0) {
			mGameOverMode->SetWin(false);
			mNextMode = mGameOverMode;
			break;
		}
		else if (i != mPlayerNdx && mDrivers[i]->GetHealth() > 0) {
			allAIsDead = false;
		}
	}

	if (allAIsDead && mNextMode == NULL) {
		mGameOverMode->SetWin(true);
		mNextMode = mGameOverMode;
	}

	for (pwItr = mProjectileWeapons.begin(); pwItr != mProjectileWeapons.end(); pwItr++)
		(*pwItr)->Update(timeElapsed);

	for (hbItr = mHealthBoxes.begin(); hbItr != mHealthBoxes.end(); hbItr++) {
		(*hbItr)->Update(timeElapsed);
	}

	for (weapItr = mWeaponBoxes.begin(); weapItr != mWeaponBoxes.end(); weapItr++) {
		(*weapItr)->Update(timeElapsed);
	}

	HandleCollisions();

	mSkydome.Update(timeElapsed);
}


void WorldMode::Render() {
	vector<Driver *>::iterator driverItr;
	vector<ProjectileWeapon *>::iterator pwItr;
	vector<FractalTerrain *>::iterator ftItr;
	vector<SmallHealthPowerUp *>::iterator hbItr;
	vector<WeaponPowerUp *>::iterator weapItr;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector3 pos = mDrivers[mPlayerNdx]->GetPos();
	//Vector3 eye = pos - 10 * mDrivers[mPlayerNdx]->GetW();
	Vector3 eye = pos - 10 * mDrivers[mPlayerNdx]->GetCamDir();

	if (mDebugEyeOn) {
		Vector3 LaPos = mDebugEye.GetPos() + mDebugEye.GetW();
		gluLookAt(mDebugEye.GetPos().GetX(), mDebugEye.GetPos().GetY(), mDebugEye.GetPos().GetZ(),
			LaPos.GetX(), LaPos.GetY(), LaPos.GetZ(),
			0, 1, 0);
	}
	else
		gluLookAt(eye.GetX(), eye.GetY() + 5, eye.GetZ(),
		pos.GetX(), pos.GetY()+2, pos.GetZ(),
		0, 1, 0);

	mFrustum.Update();

	//cout << "mPlayerPos: " << pos << endl;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, templeTex.TextureID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	DrawOBJ(objmesh->m_iMeshID);

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// Render Terrain, Drivers and WeaponProjectiles
	for (ftItr = mFractalTerrains.begin(); ftItr != mFractalTerrains.end(); ftItr++) {
		(*ftItr)->RenderTerrain(pos);
	}

	for (pwItr = mProjectileWeapons.begin(); pwItr != mProjectileWeapons.end(); pwItr++)
	{
		if ((*pwItr)->IsActive())
			(*pwItr)->Render();
	}

	for (hbItr = mHealthBoxes.begin(); hbItr != mHealthBoxes.end(); hbItr++) {
		if ((*hbItr)->isActive())
			(*hbItr)->Render();
	}

	for (weapItr = mWeaponBoxes.begin(); weapItr != mWeaponBoxes.end(); weapItr++) {
		if ((*weapItr)->isActive())
			(*weapItr)->Render();
	}

	for (driverItr = mDrivers.begin(); driverItr != mDrivers.end(); driverItr++) {
		(*driverItr)->Render();
	}

	mSkydome.Render();
	DrawLensFlare();
	DrawHUD();

	SDL_GL_SwapBuffers();
}

void WorldMode::LoadLensFlareAssets() {
	assert(mTextureLoader.LoadTextureFromDisk("LensFlare/glow.tga",
		&mLensFlareTextures[LF_GLOW]) == TRUE);
	assert(mTextureLoader.LoadTextureFromDisk("LensFlare/halo1.tga",
		&mLensFlareTextures[LF_HALO1]) == TRUE);
	assert(mTextureLoader.LoadTextureFromDisk("LensFlare/halo2.tga",
		&mLensFlareTextures[LF_HALO2]) == TRUE);
	assert(mTextureLoader.LoadTextureFromDisk("LensFlare/halo3.tga",
		&mLensFlareTextures[LF_HALO3]) == TRUE);
	return;
}

void WorldMode::DrawLensFlare() {
	int centerX, centerY, i;
	float texW, texH;
	double sunX, sunY;

	Vector3 sunPos;
	GLdouble modelMatrix[16], projMatrix[16];
	GLint viewport[4];
	GLdouble screenCoords[3];

	float flare_size[7] = {1.0f, 0.5f, 0.25f, 1.0f, 0.5f, 0.25f, 0.25f};
	float flare_rel_pos[7] = {1.0f, 0.5f, 0.33f, 0.125f, -0.5f, -0.25f, -0.19f};
	double flare_pos[7][2];
	enum lens_flare_enum flare_type[7] = {LF_GLOW, LF_HALO1, LF_GLOW, LF_HALO2, LF_GLOW, LF_HALO3, LF_GLOW};

	/* Determine if the lens flare will be drawn based on the
	* position and direction of the viewer and the position of
	* the sun on the skydome
	*/
	sunPos = mSkydome.getSunPos();
	glGetIntegerv(GL_VIEWPORT, viewport);				// get current viewport transformation
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);	// get current model view xformation
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);	// get current projection xformation

	/* If the projection failed, then don't render the lens flare */
	if (gluProject(sunPos.GetX(), sunPos.GetY(), sunPos.GetZ(),
		modelMatrix, projMatrix, viewport,
		&(screenCoords[0]), &(screenCoords[1]), &(screenCoords[2])) == GL_FALSE)
	{
		return;
	}

	/* If the screen coords are not valid, then don't render the lens flare */
	//cout << "Screen Coords: (" << screenCoords[0] << " , " << screenCoords[1] << " , " << screenCoords[2] << ")\n";
	if (screenCoords[2] > 1)
	{
		return;
	}

	/* Compute where all the lens flare components will be placed and draw them to screen */
	centerX = (int)(mGameWindow->GetWidth()/2);
	centerY = (int)(mGameWindow->GetHeight()/2);
	sunX = screenCoords[0];
	sunY = screenCoords[1];

	for (i=0; i<7; i++)
	{
		flare_pos[i][0] = centerX - ((centerX - sunX) * flare_rel_pos[i]) - (64 * flare_size[i]);
		flare_pos[i][1] = centerY + ((centerY - sunY) * flare_rel_pos[i]) - (64 * flare_size[i]);
	}

	/* Now draw the flare components to the screen */
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	mGameWindow->SetOrthographicProjection();

	for (i=0; i<7; i++)
	{
		glPushMatrix();
		glLoadIdentity();
		texW = mLensFlareTextures[flare_type[i]].Width * flare_size[i];
		texH = mLensFlareTextures[flare_type[i]].Height * flare_size[i];

		glTranslated(flare_pos[i][0], flare_pos[i][1], 0);
		glBindTexture(GL_TEXTURE_2D, mLensFlareTextures[flare_type[i]].TextureID);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(0, texH);
			glTexCoord2f(1.0, 0.0);
			glVertex2f(texW, texH);
			glTexCoord2f(1.0, 1.0);
			glVertex2f(texW, 0);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(0, 0);
		glEnd();
		glPopMatrix();
	}

	mGameWindow->ResetPerspectiveProjection();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glBlendFunc(GL_ONE, GL_ZERO);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void WorldMode::LoadHUDAssets() {
	mTextureLoader.SetMipMapping(true);
	mTextureLoader.SetTextureFilter(txBilinear);

	for (unsigned int i = 0; i < NUM_WEAPONS; ++i) {
		char path[20];
		sprintf_s(path, "assets/textures/hud/weapon%d.tga", i);
		assert(mTextureLoader.LoadTextureFromDisk(path, &mWeaponTextures[i]) 
			== TRUE);
	}

	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/hud/lifebar_good.tga",
		&mLifebarTextures[LIFE_GOOD]) == TRUE);
	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/hud/lifebar_half.tga",
		&mLifebarTextures[LIFE_HALF]) == TRUE);
	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/hud/lifebar_crit.tga",
		&mLifebarTextures[LIFE_CRIT]) == TRUE);

	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/hud/lifebar_case.tga",
		&mLifebarCaseTexture) == TRUE);

	for (int i = 0; i < 10; ++i) {
		char path[15];
		sprintf(path, "assets/textures/hud/%d.tga", i);
		assert(mTextureLoader.LoadTextureFromDisk(path, &mNumberTextures[i]) 
			== TRUE);
	}
}


void WorldMode::DrawHUD() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	mGameWindow->SetOrthographicProjection();

	glActiveTexture(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);

	// draw the current weapon image
	glPushMatrix();
	glLoadIdentity();
	int texW = mWeaponTextures[mDrivers[mPlayerNdx]->GetCurrentWeapon()].Width;
	int texH = mWeaponTextures[mDrivers[mPlayerNdx]->GetCurrentWeapon()].Height;
	int weaponW = texW;
	int weaponH = texH;

	glTranslatef(5, 5, 0);
	glBindTexture(GL_TEXTURE_2D, 
		mWeaponTextures[mDrivers[mPlayerNdx]->GetCurrentWeapon()].TextureID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2i(0, texH);
		glTexCoord2f(1.0, 0.0);
		glVertex2i(texW, texH);
		glTexCoord2f(1.0, 1.0);
		glVertex2i(texW, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex2i(0, 0);
	glEnd();
	glPopMatrix();

	// draw the weapon count
	int count = mDrivers[mPlayerNdx]->GetCurrentWeaponCount();
	// draw the 1s place
	glPushMatrix();
	glLoadIdentity();

	texW = mNumberTextures[0].Width;
	texH = mNumberTextures[0].Height;
	glTranslatef(5.0f + weaponW - texW, 5.0f + weaponH - texH - 10, 0);

	glBindTexture(GL_TEXTURE_2D, mNumberTextures[count % 10].TextureID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2i(0, texH);
		glTexCoord2f(1.0, 0.0);
		glVertex2i(texW, texH);
		glTexCoord2f(1.0, 1.0);
		glVertex2i(texW, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex2i(0, 0);
	glEnd();

	glPopMatrix();

	// draw the 10s place if there is one
	if (count / 10 > 0) {
		glPushMatrix();
		glLoadIdentity();

		glTranslatef(5.0f + weaponW - texW - 23, 5.0f + weaponH - texH - 10, 0);

		glBindTexture(GL_TEXTURE_2D, mNumberTextures[count / 10].TextureID);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex2i(0, texH);
			glTexCoord2f(1.0, 0.0);
			glVertex2i(texW, texH);
			glTexCoord2f(1.0, 1.0);
			glVertex2i(texW, 0);
			glTexCoord2f(0.0, 1.0);
			glVertex2i(0, 0);
		glEnd();

		glPopMatrix();
	}

	// draw the health bar case
	texW = mLifebarCaseTexture.Width + 10;
	texH = mLifebarCaseTexture.Height + 10;

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(weaponW + 10.0f, 5.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, mLifebarCaseTexture.TextureID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2i(0, texH);
		glTexCoord2f(1.0, 0.0);
		glVertex2i(texW, texH);
		glTexCoord2f(1.0, 1.0);
		glVertex2i(texW, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex2i(0, 0);
	glEnd();

	glPopMatrix();

	// draw the health bar
	float healthPercent = mDrivers[mPlayerNdx]->GetHealth() /
		(float)mDrivers[mPlayerNdx]->GetMaxHealth();
	int status;
	if (healthPercent <= .11)
		status = LIFE_CRIT;
	else if (healthPercent <= .51)
		status = LIFE_HALF;
	else
		status = LIFE_GOOD;

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(weaponW + 15.0f, 10.0f, 0.0f);

	texW = mLifebarTextures[status].Width;
	texH = mLifebarTextures[status].Height;

	glBindTexture(GL_TEXTURE_2D, mLifebarTextures[status].TextureID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2i(0, texH);
		glTexCoord2f(healthPercent, 0.0);
		glVertex2f(texW * healthPercent, (float)texH);
		glTexCoord2f(healthPercent, 1.0);
		glVertex2f(texW * healthPercent, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(0, 0);
	glEnd();
	glPopMatrix();

	mGameWindow->ResetPerspectiveProjection();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glBlendFunc(GL_ONE, GL_ZERO);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void WorldMode::AddProjectileWeapon(ProjectileType w_type, float phi, float theta, Vector3 &pos, Vector3 &vel, Vector3 &dir) {
	//unsigned int i;
	Iceshot *pm_i;
	Toxicshot *pm_t;
	Fireball *pm_f;


	switch (w_type)
	{
	case W_ICESHOT:
		pm_i = new Iceshot();
		//3 should be radius of player
		pm_i->Initialize(phi, theta, pos + 3*dir, 2*dir);
		mProjectileWeapons.push_back(pm_i);
		break;
	case W_TOXICSHOT:
		pm_t = new Toxicshot();
		//3 should be radius of player
		pm_t->Initialize(phi, theta, pos + 3*dir, 2*dir);
		mProjectileWeapons.push_back(pm_t);
		break;
	default:
		pm_f = new Fireball();
		//3 should be radius of player
		pm_f->Initialize(phi, theta, pos + 3*dir, 2*dir);
		mProjectileWeapons.push_back(pm_f);
		break;
	}    
}

void WorldMode::HandleCollisions() {
	vector<Driver *>::iterator driverItr, driverItr2;
	vector<ProjectileWeapon *>::iterator pwItr;
	vector<FractalTerrain *>::iterator terrainItr;
	vector<SmallHealthPowerUp *>::iterator hbItr;
	vector<WeaponPowerUp *>::iterator weapItr;
	Vector3 collisionNormal, vel, normalVel, relVel;
	float impulse;

	int intersection = 0;
	int wallIntersection = 0;
	int weaponIntersect = 0;
	float t,u,v;

	//Handle Collisions between drivers and drivers
	for (driverItr = mDrivers.begin(); driverItr != mDrivers.end(); driverItr++) {
		driverItr2 = driverItr;
		for (driverItr2++; driverItr2 != mDrivers.end(); driverItr2++) {
			if ((*driverItr)->IsCollided(*driverItr2, collisionNormal)) {
				relVel = (*driverItr)->GetVel() - (*driverItr2)->GetVel();

				playSound("sfx/collide_driver_driver.wav", 0.5);

				impulse = -1.5f*relVel.DotProduct(collisionNormal)/(1/(*driverItr)->GetMass() + 1/(*driverItr2)->GetMass());
				(*driverItr)->SetVel((*driverItr)->GetVel() + (impulse/(*driverItr)->GetMass())*collisionNormal);
				(*driverItr2)->SetVel((*driverItr2)->GetVel() - (impulse/(*driverItr2)->GetMass())*collisionNormal);
			}
		}
	}

	//Handle Collisions between drivers and weapons
	for (driverItr = mDrivers.begin(); driverItr != mDrivers.end(); driverItr++) {
		pwItr = mProjectileWeapons.begin();
		while (pwItr != mProjectileWeapons.end()) {
			if ((*driverItr)->IsCollided((*pwItr))) {
				(*pwItr)->MadeImpact();

				if ((*pwItr)->GetWeaponType() == W_FIREBALL)
					(*driverItr)->DecrementHealth((*pwItr)->GetDamage());
				else if ((*pwItr)->GetWeaponType() == W_ICESHOT)
				{
					(*driverItr)->SetVel(0.0);
					(*driverItr)->SetFrozen(true);
				}
				else if ((*pwItr)->GetWeaponType() == W_TOXICSHOT)
				{
					(*driverItr)->DecrementHealth(3*(*pwItr)->GetDamage());
				}

				playSound("sfx/collide_weapon_driver.wav", 1.0);

				delete *pwItr;
				pwItr = mProjectileWeapons.erase(pwItr);
			}
			else {
				pwItr++;
			}
		}
	}

	//Handle Collisions between weapons and terrain
	for (terrainItr = mFractalTerrains.begin(); terrainItr != mFractalTerrains.end(); terrainItr++) {
		pwItr = mProjectileWeapons.begin();
		while (pwItr != mProjectileWeapons.end()) {
			if ((*terrainItr)->IsCollided(*pwItr)) {
				(*pwItr)->MadeImpact();

				playSound("sfx/collide_weapon_terrain.wav", 0.5); 

				delete *pwItr;
				pwItr = mProjectileWeapons.erase(pwItr);
			}
			else {
				pwItr++;
			}
		}
	}

	//Handle collisions between temple and drivers
	for (driverItr = mDrivers.begin(); driverItr != mDrivers.end(); driverItr++) {
		for (int i = 0; i < num_surfaces; i++)
		{
			if (intersect_triangle((*driverItr)->GetPos(), Vector3(0,-1,0), 
				Vector3(objmesh->m_aVertexArray[surfaces[i]->m_aVertexIndices[0]].x, 
				objmesh->m_aVertexArray[surfaces[i]->m_aVertexIndices[0]].y, 
				objmesh->m_aVertexArray[surfaces[i]->m_aVertexIndices[0]].z),
				Vector3(objmesh->m_aVertexArray[surfaces[i]->m_aVertexIndices[1]].x, 
				objmesh->m_aVertexArray[surfaces[i]->m_aVertexIndices[1]].y, 
				objmesh->m_aVertexArray[surfaces[i]->m_aVertexIndices[1]].z),
				Vector3(objmesh->m_aVertexArray[surfaces[i]->m_aVertexIndices[2]].x, 
				objmesh->m_aVertexArray[surfaces[i]->m_aVertexIndices[2]].y, 
				objmesh->m_aVertexArray[surfaces[i]->m_aVertexIndices[2]].z),
				&t, &u, &v))
			{
				if (t < 5.0 && t > 0)
				{
					intersection = 1;
					break;
				}
			}
		}

		if (intersection == 1)
		{
			if (t > 0)
			{
				if (((*driverItr)->GetPos().GetY() - t + 1.0) >= 1.0)
					(*driverItr)->SetPos(Vector3((*driverItr)->GetPos().GetX(),((*driverItr)->GetPos().GetY() - t + 1.5f),(*driverItr)->GetPos().GetZ()));

				(*driverItr)->SetOnPlatform(true);
			}
			else
			{
				(*driverItr)->SetOnPlatform(false);
			}
		}
		else if (intersection == 0 && (*driverItr)->GetPos().GetY() > 1.5)
		{
			(*driverItr)->SetPos(Vector3((*driverItr)->GetPos().GetX(),(*driverItr)->GetPos().GetY()-0.3f,(*driverItr)->GetPos().GetZ()));
			(*driverItr)->SetOnPlatform(false);
		}

		for (int j = 0; j < num_walls; j++)
		{
			if (intersect_triangle((*driverItr)->GetPos(), (*driverItr)->GetVel(), 
				Vector3(objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[0]].x, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[0]].y, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[0]].z),
				Vector3(objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[1]].x, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[1]].y, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[1]].z),
				Vector3(objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[2]].x, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[2]].y, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[2]].z),
				&t, &u, &v) && t < 0.2 && t > 0)
			{
				wallIntersection = 1;
				break;
			}
		}
		if (wallIntersection == 1)
		{
			(*driverItr)->SetVel(-1.5*((*driverItr)->GetVel()));
		}

		intersection = wallIntersection = 0;
	}

	//Handle collisions between projectiles and temple
	pwItr = mProjectileWeapons.begin();

	while (pwItr != mProjectileWeapons.end())
	{
		for (int j = 0; j < num_walls; j++)
		{
			if (intersect_triangle((*pwItr)->GetPos(), (*pwItr)->GetVel(), 
				Vector3(objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[0]].x, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[0]].y, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[0]].z),
				Vector3(objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[1]].x, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[1]].y, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[1]].z),
				Vector3(objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[2]].x, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[2]].y, 
				objmesh->m_aVertexArray[walls[j]->m_aVertexIndices[2]].z),
				&t, &u, &v) && t < 1.0 && t > -6.5)
			{
				(*pwItr)->MadeImpact();
				delete *pwItr;
				pwItr = mProjectileWeapons.erase(pwItr);
				weaponIntersect = 1;
				break;
			}
			else
				weaponIntersect = 0;
		}
		if (weaponIntersect == 0)
			pwItr++;
	}

	//Handle Collisions between drivers and terrain
	for (terrainItr = mFractalTerrains.begin(); terrainItr != mFractalTerrains.end(); terrainItr++) {
		for (driverItr = mDrivers.begin(); driverItr != mDrivers.end(); driverItr++) {
			if ((*terrainItr)->IsCollided(*driverItr, collisionNormal)) {
				vel = (*driverItr)->GetVel();
				normalVel = vel.DotProduct(collisionNormal) * collisionNormal;
				(*driverItr)->SetVel(vel - 2*normalVel);

				playSound("sfx/collide_driver_terrain.wav", 1.0);
			}
		}
	}

	//Handle Collisions between drivers and health boxes
	for (driverItr = mDrivers.begin(); driverItr != mDrivers.end(); driverItr++) {
		hbItr = mHealthBoxes.begin();
		while (hbItr != mHealthBoxes.end()) {
			if ((*hbItr)->isActive() && (*hbItr)->IsCollided((*driverItr)->GetPos(), (*driverItr)->GetBoundsRadius())) {
				(*driverItr)->IncrementHealth(25);

				//playSound("sfx/collide_weapon_driver.wav", 1.0);

				//delete *hbItr;
				//hbItr = mHealthBoxes.erase(hbItr);
				//hbItr++;
				break;
			}
			else {
				hbItr++;
			}
		}
	}

	//Handle Collisions between drivers and weapon powerups

	for (driverItr = mDrivers.begin(); driverItr != mDrivers.end(); driverItr++) {
		weapItr = mWeaponBoxes.begin();
		while (weapItr != mWeaponBoxes.end()) {
			if ((*weapItr)->isActive() && (*weapItr)->IsCollided((*driverItr)->GetPos(), (*driverItr)->GetBoundsRadius())) {              
				//playSound("sfx/collide_weapon_driver.wav", 1.0);

				int weapon = (rand())%3;

				(*driverItr)->SetCurrentWeapon((ProjectileType)weapon);
				//(*driverItr)->SetSecondaryWeapon((projectile_type)weapon);

				if (weapon == 0)
					(*driverItr)->SetFireCount((*driverItr)->GetFireCount()+8);
				else if (weapon == 1)
					(*driverItr)->SetIceCount((*driverItr)->GetIceCount()+1);
				else if (weapon == 2)
					(*driverItr)->SetToxicCount((*driverItr)->GetToxicCount()+4);

				//delete *hbItr;
				//weapItr = mWeaponBoxes.erase(weapItr);
				//hbItr++;
				break;
			}
			else {
				weapItr++;
			}
		}
	}
}
