#include "MainMenuMode.h"
#include "GameWindow.h"
#include <SDL/SDL.h>
#include <SDL/SDL_OpenGL.h>
#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

MainMenuMode::~MainMenuMode() {
   mTextureLoader.FreeTexture(&mBackgroundTexture);
   mTextureLoader.FreeTexture(&mTitleTexture);

   for (int i = 0; i < NUM_BUTTONS; ++i) {
      mTextureLoader.FreeTexture(&mButtons[i].mTexture);
   }
}


void MainMenuMode::SetNeighborModes(const std::vector<GameMode *> &modes) {
   mCharacterSelectMode = modes[0];
}

bool MainMenuMode::LoadResources() {
   mTextureLoader.SetMipMapping(true);
   mTextureLoader.SetTextureFilter(txBilinear);

   assert(mTextureLoader.LoadTextureFromDisk("menu/background.tga", 
    &mBackgroundTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("menu/title.tga", &mTitleTexture) 
    == TRUE);

   assert(mTextureLoader.LoadTextureFromDisk("menu/play.tga", 
    &mButtons[BTN_PLAY].mTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("menu/options.tga", 
    &mButtons[BTN_OPTIONS].mTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("menu/quit.tga", 
    &mButtons[BTN_QUIT].mTexture) == TRUE);

   assert(mTextureLoader.LoadTextureFromDisk("menu/play_pressed.tga", 
    &mButtons[BTN_PLAY].mPressedTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("menu/options_pressed.tga", 
    &mButtons[BTN_OPTIONS].mPressedTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("menu/quit_pressed.tga", 
    &mButtons[BTN_QUIT].mPressedTexture) == TRUE);

   for (int i = 0; i < NUM_BUTTONS; ++i) {
      int windowWidth = mGameWindow->GetWidth();
      int windowHeight = mGameWindow->GetHeight();

      mButtons[i].xMin = (windowWidth / 2) - 
       ((mButtons[i].mTexture.Width * (windowWidth / 1600.0)) / 2);
      mButtons[i].xMax = (windowWidth / 2) + 
       ((mButtons[i].mTexture.Width * (windowWidth / 1600.0)) / 2);

      mButtons[i].yMin = ((i + 1) * windowHeight / 4) - 
       ((mButtons[i].mTexture.Height * (windowHeight / 1200.0)) / 2);
      mButtons[i].yMax = ((i + 1) * windowHeight / 4) + 
       ((mButtons[i].mTexture.Height * (windowHeight / 1200.0)) / 2);
   }
   return true;
}

void MainMenuMode::Start() {
   mNextMode = NULL;

   InputManager *im = mGameWindow->GetInputManager();

   im->AddKeyEvent(&mStart, SDLK_s);
   im->AddKeyEvent(&mQuit, SDLK_q);

   im->AddMouseEvent(&mClick, IM_MOUSE_LEFT_BUTTON);

   im->ResetAllInputEvents();

   mGameWindow->SetOrthographicProjection();
   glEnable(GL_TEXTURE_2D);
   glEnable(GL_BLEND);
}

void MainMenuMode::Stop() {
   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);

   mGameWindow->ResetPerspectiveProjection();
}

void MainMenuMode::Update(float timeElapsed) {
   if (mClick.IsPressed()) {
      InputManager *im = mGameWindow->GetInputManager();

      int mouseX = im->GetMouseX();
      int mouseY = im->GetMouseY();

      mButtonPressed = -1;
      for (int i = 0; i < NUM_BUTTONS; ++i) {
         if (mButtons[i].IsIn(mouseX, mouseY)) {
            mButtonPressed = i;
         }
      }
   }
   else {
      if (mButtonPressed == BTN_PLAY) {
         mNextMode = mCharacterSelectMode;
      }
      else if (mButtonPressed == BTN_OPTIONS) {
         // TODO: add options menu
      }
      else if (mButtonPressed == BTN_QUIT) {
         mGameWindow->QuitGame();
      }
      mButtonPressed = -1;
   }
}

void MainMenuMode::Render() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);

   int texW, texH;

   // draw the background
   glPushMatrix();
   glLoadIdentity();
   glBindTexture(GL_TEXTURE_2D, mBackgroundTexture.TextureID);
   glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0);
      glVertex2f(0, mGameWindow->GetHeight());

      glTexCoord2f(1.0, 0.0);
      glVertex2f(mGameWindow->GetWidth(), mGameWindow->GetHeight());

      glTexCoord2f(1.0, 1.0);
      glVertex2f(mGameWindow->GetWidth(), 0);

      glTexCoord2f(0.0, 1.0);
      glVertex2f(0, 0);
   glEnd();
   glPopMatrix();

   // draw the title
   texW = 7 * mGameWindow->GetWidth() / 8;
   texH = mTitleTexture.Height * (texW / (float)mTitleTexture.Width);

   glPushMatrix();
   glLoadIdentity();
   glTranslatef(mGameWindow->GetWidth()/2, 0, 0);
   glBindTexture(GL_TEXTURE_2D, mTitleTexture.TextureID);
   glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0);
      glVertex2f(-texW/2, texH);

      glTexCoord2f(1.0, 0.0);
      glVertex2f(texW/2, texH);

      glTexCoord2f(1.0, 1.0);
      glVertex2f(texW/2, 0);

      glTexCoord2f(0.0, 1.0);
      glVertex2f(-texW/2, 0);
   glEnd();
   glPopMatrix();

   // draw play button
   texW = mButtons[0].mTexture.Width * (mGameWindow->GetWidth() / 1600.0);
   texH = mButtons[0].mTexture.Height * (mGameWindow->GetHeight() / 1200.0);

   glPushMatrix();
   glLoadIdentity();
   glTranslatef(mGameWindow->GetWidth()/2, mGameWindow->GetHeight()/4, 0);

   if (mButtonPressed == BTN_PLAY) {
      glBindTexture(GL_TEXTURE_2D, 
       mButtons[BTN_PLAY].mPressedTexture.TextureID);
   }
   else {
      glBindTexture(GL_TEXTURE_2D, mButtons[BTN_PLAY].mTexture.TextureID);
   }
   glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0);
      glVertex2f(-texW/2, texH/2);

      glTexCoord2f(1.0, 0.0);
      glVertex2f(texW/2, texH/2);
      
      glTexCoord2f(1.0, 1.0);
      glVertex2f(texW/2, -texH/2);

      glTexCoord2f(0.0, 1.0);
      glVertex2f(-texW/2, -texH/2);
   glEnd();
   glPopMatrix();

   // draw the options button
   glPushMatrix();
   glLoadIdentity();
   glTranslatef(mGameWindow->GetWidth()/2, 2 * mGameWindow->GetHeight()/4, 0);

   texW = mButtons[1].mTexture.Width * (mGameWindow->GetWidth() / 1600.0);
   texH = mButtons[1].mTexture.Height * (mGameWindow->GetHeight() / 1200.0);

   if (mButtonPressed == BTN_OPTIONS) {
      glBindTexture(GL_TEXTURE_2D, 
       mButtons[BTN_OPTIONS].mPressedTexture.TextureID);
   }
   else {
      glBindTexture(GL_TEXTURE_2D, mButtons[BTN_OPTIONS].mTexture.TextureID);
   }
   glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0);
      glVertex2f(-texW/2, texH/2);

      glTexCoord2f(1.0, 0.0);
      glVertex2f(texW/2, texH/2);
      
      glTexCoord2f(1.0, 1.0);
      glVertex2f(texW/2, -texH/2);

      glTexCoord2f(0.0, 1.0);
      glVertex2f(-texW/2, -texH/2);
   glEnd();
   glPopMatrix();

   // draw the quit button
   glPushMatrix();
   glLoadIdentity();
   glTranslatef(mGameWindow->GetWidth()/2, 3 * mGameWindow->GetHeight()/4, 0);

   texW = mButtons[2].mTexture.Width * (mGameWindow->GetWidth() / 1600.0);
   texH = mButtons[2].mTexture.Height * (mGameWindow->GetHeight() / 1200.0);

   if (mButtonPressed == BTN_QUIT) {
      glBindTexture(GL_TEXTURE_2D, 
       mButtons[BTN_QUIT].mPressedTexture.TextureID);
   }
   else {
      glBindTexture(GL_TEXTURE_2D, mButtons[BTN_QUIT].mTexture.TextureID);
   }
   glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0);
      glVertex2f(-texW/2, texH/2);

      glTexCoord2f(1.0, 0.0);
      glVertex2f(texW/2, texH/2);
      
      glTexCoord2f(1.0, 1.0);
      glVertex2f(texW/2, -texH/2);

      glTexCoord2f(0.0, 1.0);
      glVertex2f(-texW/2, -texH/2);
   glEnd();
   glPopMatrix();

   SDL_GL_SwapBuffers();
}