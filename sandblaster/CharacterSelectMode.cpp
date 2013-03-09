#include <windows.h>
#include <GL/glee.h>
#include <SDL/SDL.h>
#include <SDL/SDL_OpenGL.h>
#include <assert.h>

#include "CharacterSelectMode.h"
#include "InputManager.h"
#include "GameWindow.h"

CharacterSelectMode::~CharacterSelectMode() {
	mTextureLoader.FreeTexture(&mBackgroundTexture);

	for (int i = 0; i < NUM_CHARACTERS; ++i) {
		mTextureLoader.FreeTexture(&mPortraitTextures[i]);
		mTextureLoader.FreeTexture(&mStatTextures[i]);
		mTextureLoader.FreeTexture(&mDescripTextures[i]);
	}
}

void CharacterSelectMode::SetNeighborModes(const std::vector<GameMode *> &modes) {
	assert(modes.size() == 2);

	mWorldMode = dynamic_cast<WorldMode *>(modes[0]);
	mMainMenuMode = modes[1];
}

bool CharacterSelectMode::LoadResources() {
	mTextureLoader.SetMipMapping(TRUE);
	mTextureLoader.SetTextureFilter(txBilinear);

	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/background.tga", 
		&mBackgroundTexture) == TRUE);

	for (int i = 0; i < NUM_CHARACTERS; ++i) {
		char path[50];
		sprintf_s(path, "assets/textures/menu/select_portrait%d.tga", i);
		assert(mTextureLoader.LoadTextureFromDisk(path, &mPortraitTextures[i]) 
			== TRUE);

		sprintf_s(path, "assets/textures/menu/select_stats%d.tga", i);
		assert(mTextureLoader.LoadTextureFromDisk(path, &mStatTextures[i]) 
			== TRUE);

		sprintf_s(path, "assets/textures/menu/select_descrip%d.tga", i);
		assert(mTextureLoader.LoadTextureFromDisk(path, &mDescripTextures[i]) 
			== TRUE);
	}

	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/select_previous.tga", 
		&mButtons[BTN_PREVIOUS].mTexture) == TRUE);
	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/select_back.tga", 
		&mButtons[BTN_BACK].mTexture) == TRUE);
	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/select_ok.tga", 
		&mButtons[BTN_OK].mTexture) == TRUE);
	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/select_next.tga", 
		&mButtons[BTN_NEXT].mTexture) == TRUE);

	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/select_previous_pressed.tga", 
		&mButtons[BTN_PREVIOUS].mPressedTexture) == TRUE);
	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/select_back_pressed.tga", 
		&mButtons[BTN_BACK].mPressedTexture) == TRUE);
	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/select_ok_pressed.tga", 
		&mButtons[BTN_OK].mPressedTexture) == TRUE);
	assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/select_next_pressed.tga", 
		&mButtons[BTN_NEXT].mPressedTexture) == TRUE);

	int windowWidth = mGameWindow->GetWidth();
	int windowHeight = mGameWindow->GetHeight();

	mButtons[BTN_PREVIOUS].xMin = windowWidth / 85;
	mButtons[BTN_BACK].xMin = 23 * windowWidth / 85;
	mButtons[BTN_OK].xMin = 46 * windowWidth / 85;
	mButtons[BTN_NEXT].xMin = 68 * windowWidth / 85;

	for (int i = 0; i < NUM_SELECT_BUTTONS; ++i) {
		mButtons[i].yMax = windowHeight - 20;
		mButtons[i].yMin = (int)floor(mButtons[i].yMax - 
			(BTN_HEIGHT * (windowHeight / 1200.0f)));

		mButtons[i].xMax = (int)floor(mButtons[i].xMin + 
			floor(BTN_WIDTH * (windowWidth / 1600.0f)));
	}   

	return true;
}

void CharacterSelectMode::Start() {
	mButtonPressed = -1;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mNextMode = NULL;

	InputManager *im = mGameWindow->GetInputManager();

	im->AddMouseEvent(&mClick, IM_MOUSE_LEFT_BUTTON);

	im->ResetAllInputEvents();

	mGameWindow->SetOrthographicProjection();

	Vector3 zeroVec;
	mSarge.Initialize(mWorldMode, 10, 10, 0, 0, 0, 0, zeroVec, zeroVec, 0);
	mScrappy.Initialize(mWorldMode, 10, 10, 0, 0, 0, 0, zeroVec, zeroVec, 0);

}

void CharacterSelectMode::Stop() {
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	mGameWindow->ResetPerspectiveProjection();
}

void CharacterSelectMode::Update(float timeElapsed) {
	if (mClick.IsPressed()) {
		InputManager *im = mGameWindow->GetInputManager();

		int mouseX = im->GetMouseX();
		int mouseY = im->GetMouseY();

		mButtonPressed = -1;
		for (int i = 0; i < NUM_SELECT_BUTTONS; ++i) {
			if (mButtons[i].IsIn(mouseX, mouseY)) {
				mButtonPressed = i;
			}
		}
	}
	else {
		if (mButtonPressed == BTN_PREVIOUS) {
			mCharacterNdx = mCharacterNdx - 1 < 0 ? NUM_CHARACTERS - 1 : 
				mCharacterNdx - 1;
		}
		else if (mButtonPressed == BTN_BACK) {
			mNextMode = mMainMenuMode;
		}
		else if (mButtonPressed == BTN_OK) {
			mWorldMode->Reset();
			mWorldMode->SetPlayerNdx(mCharacterNdx);
			mNextMode = mWorldMode;
		}
		else if (mButtonPressed == BTN_NEXT) {
			mCharacterNdx = mCharacterNdx + 1 > NUM_CHARACTERS - 1 ? 0 : 
				mCharacterNdx + 1;
		}
		mButtonPressed = -1;
	}

	mRotation += 90.0f * timeElapsed;
}

// TODO: refactor
void CharacterSelectMode::Render() {
	int texW, texH;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	// draw background
	glPushMatrix();
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, mBackgroundTexture.TextureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(0, mGameWindow->GetHeight());

	glTexCoord2f(1.0, 0.0);
	glVertex2i(mGameWindow->GetWidth(), mGameWindow->GetHeight());

	glTexCoord2f(1.0, 1.0);
	glVertex2i(mGameWindow->GetWidth(), 0);

	glTexCoord2f(0.0, 1.0);
	glVertex2i(0, 0);
	glEnd();
	glPopMatrix();

	// draw character portrait
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(15, 15, 0);

	glBindTexture(GL_TEXTURE_2D, mPortraitTextures[mCharacterNdx].TextureID);
	texW = (int)floor(mPortraitTextures[mCharacterNdx].Width * 
		(mGameWindow->GetWidth() / 1600.0f));
	texH = (int)floor(mPortraitTextures[mCharacterNdx].Height * 
		(mGameWindow->GetHeight() / 1200.0f));
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

	// draw character stats
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(mGameWindow->GetWidth() - 15.0f, 15, 0);

	glBindTexture(GL_TEXTURE_2D, mStatTextures[mCharacterNdx].TextureID);
	texW = (int)floor(mStatTextures[mCharacterNdx].Width * 
		(mGameWindow->GetWidth() / 1600.0f));
	texH = (int)floor(mStatTextures[mCharacterNdx].Height * 
		(mGameWindow->GetHeight() / 1200.0f));
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(-texW, texH);

	glTexCoord2f(1.0, 0.0);
	glVertex2i(0, texH);

	glTexCoord2f(1.0, 1.0);
	glVertex2i(0, 0);

	glTexCoord2f(0.0, 1.0);
	glVertex2i(-texW, 0);
	glEnd();
	glPopMatrix();

	// draw character description
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(mGameWindow->GetWidth() / 2.0f, 
		mGameWindow->GetHeight() / 2 - 80.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, mDescripTextures[mCharacterNdx].TextureID);
	texW = (int)floor(mDescripTextures[mCharacterNdx].Width * 
		(mGameWindow->GetWidth() / 1600.0f));
	texH = (int)floor(mDescripTextures[mCharacterNdx].Height * 
		(mGameWindow->GetHeight() / 1200.0f));
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(-texW/2.0f, (float)texH);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(texW/2.0f, (float)texH);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(texW/2.0f, 0);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(-texW/2.0f, 0);
	glEnd();
	glPopMatrix();

	// draw previous button
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(mGameWindow->GetWidth() / 85.0f, 
		mGameWindow->GetHeight() - 20.0f, 0.0f);

	texW = (int)floor(mButtons[BTN_PREVIOUS].mTexture.Width * 
		(mGameWindow->GetWidth() / 1600.0f));
	texH = (int)floor(mButtons[BTN_PREVIOUS].mTexture.Height *
		(mGameWindow->GetHeight() / 1200.0f));

	if (mButtonPressed == BTN_PREVIOUS)
		glBindTexture(GL_TEXTURE_2D, 
		mButtons[BTN_PREVIOUS].mPressedTexture.TextureID);
	else
		glBindTexture(GL_TEXTURE_2D,
		mButtons[BTN_PREVIOUS].mTexture.TextureID);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(0, 0);

	glTexCoord2f(1.0, 0.0);
	glVertex2i(texW, 0);

	glTexCoord2f(1.0, 1.0);
	glVertex2i(texW, -texH);

	glTexCoord2f(0.0, 1.0);
	glVertex2i(0, -texH);
	glEnd();
	glPopMatrix();

	// draw back button
	glPushMatrix();
	glLoadIdentity();

	texW = (int)floor(mButtons[BTN_BACK].mTexture.Width * 
		(mGameWindow->GetWidth() / 1600.0f));
	texH = (int)floor(mButtons[BTN_BACK].mTexture.Height *
		(mGameWindow->GetHeight() / 1200.0f));

	glTranslatef(23 * mGameWindow->GetWidth() / 85.0f, 
		mGameWindow->GetHeight() - 20.0f, 0.0f);

	if (mButtonPressed == BTN_BACK)
		glBindTexture(GL_TEXTURE_2D, 
		mButtons[BTN_BACK].mPressedTexture.TextureID);
	else
		glBindTexture(GL_TEXTURE_2D,
		mButtons[BTN_BACK].mTexture.TextureID);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(0, 0);

	glTexCoord2f(1.0, 0.0);
	glVertex2i(texW, 0);

	glTexCoord2f(1.0, 1.0);
	glVertex2i(texW, -texH);

	glTexCoord2f(0.0, 1.0);
	glVertex2i(0, -texH);
	glEnd();
	glPopMatrix();

	// draw ok button
	glPushMatrix();
	glLoadIdentity();

	texW = (int)floor(mButtons[BTN_OK].mTexture.Width * 
		(mGameWindow->GetWidth() / 1600.0f));
	texH = (int)floor(mButtons[BTN_OK].mTexture.Height *
		(mGameWindow->GetHeight() / 1200.0f));

	glTranslatef(46 * mGameWindow->GetWidth() / 85.0f, 
		mGameWindow->GetHeight() - 20.0f, 0.0f);

	if (mButtonPressed == BTN_OK)
		glBindTexture(GL_TEXTURE_2D, 
		mButtons[BTN_OK].mPressedTexture.TextureID);
	else
		glBindTexture(GL_TEXTURE_2D,
		mButtons[BTN_OK].mTexture.TextureID);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(0, 0);

	glTexCoord2f(1.0, 0.0);
	glVertex2i(texW, 0);

	glTexCoord2f(1.0, 1.0);
	glVertex2i(texW, -texH);

	glTexCoord2f(0.0, 1.0);
	glVertex2i(0, -texH);
	glEnd();
	glPopMatrix();

	// draw next button
	glPushMatrix();
	glLoadIdentity();

	texW = (int)floor(mButtons[BTN_NEXT].mTexture.Width * 
		(mGameWindow->GetWidth() / 1600.0f));
	texH = (int)floor(mButtons[BTN_NEXT].mTexture.Height *
		(mGameWindow->GetHeight() / 1200.0f));

	glTranslatef(68 * mGameWindow->GetWidth() / 85.0f, 
		mGameWindow->GetHeight() - 20.0f, 0.0f);

	if (mButtonPressed == BTN_NEXT)
		glBindTexture(GL_TEXTURE_2D, 
		mButtons[BTN_NEXT].mPressedTexture.TextureID);
	else
		glBindTexture(GL_TEXTURE_2D,
		mButtons[BTN_NEXT].mTexture.TextureID);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(0, 0);

	glTexCoord2f(1.0, 0.0);
	glVertex2i(texW, 0);

	glTexCoord2f(1.0, 1.0);
	glVertex2i(texW, -texH);

	glTexCoord2f(0.0, 1.0);
	glVertex2i(0, -texH);
	glEnd();
	glPopMatrix();

	mGameWindow->ResetPerspectiveProjection();
	glClearColor(1, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPushMatrix();
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	glTranslatef(0, -1.5, -15);
	glRotatef(PI/2, 1, 0, 0);
	glRotatef(mRotation, 0, 1, 0);
	if (mCharacterNdx == 0)
		mSarge.Render();
	else if (mCharacterNdx == 1)
		mScrappy.Render();
	glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	mGameWindow->SetOrthographicProjection();

	SDL_GL_SwapBuffers();
}