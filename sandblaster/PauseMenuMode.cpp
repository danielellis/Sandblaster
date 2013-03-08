#include <windows.h>
#include <GL/glee.h>
#include <SDL/SDL.h>
#include <SDL/SDL_OpenGL.h>
#include <assert.h>

#include "PauseMenuMode.h"
#include "InputManager.h"
#include "GameWindow.h"


PauseMenuMode::~PauseMenuMode() {
   mTextureLoader.FreeTexture(&mMenuTexture);
   mTextureLoader.FreeTexture(&mBackgroundLightmap);

   for (int i = 0; i < NUM_PAUSE_BUTTONS; ++i) {
      mTextureLoader.FreeTexture(&mButtons[i].mTexture);
      mTextureLoader.FreeTexture(&mButtons[i].mPressedTexture);
   }
}


void PauseMenuMode::SetNeighborModes(const std::vector<GameMode *> &modes) {
   assert(modes.size() == 2);

   mWorldMode = modes[0];
   mMainMenuMode = modes[1];
}


bool PauseMenuMode::LoadResources() {
   mTextureLoader.SetMipMapping(TRUE);
   mTextureLoader.SetTextureFilter(txBilinear);

   assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/pausemenu.tga", 
    &mMenuTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/pause_lightmap.jpg",
    &mBackgroundLightmap) == TRUE);

   assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/pause_resume.tga", 
    &mButtons[BTN_RESUME].mTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/pause_options.tga", 
    &mButtons[BTN_OPTIONS].mTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/pause_quit.tga", 
    &mButtons[BTN_QUIT].mTexture) == TRUE);

   assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/pause_resume_pressed.tga", 
    &mButtons[BTN_RESUME].mPressedTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/pause_options_pressed.tga", 
    &mButtons[BTN_OPTIONS].mPressedTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("assets/textures/menu/pause_quit_pressed.tga", 
    &mButtons[BTN_QUIT].mPressedTexture) == TRUE);

   for (int i = 0; i < NUM_PAUSE_BUTTONS; ++i) {
      int windowWidth = mGameWindow->GetWidth();
      int windowHeight = mGameWindow->GetHeight();

      mButtons[i].xMin = (int)floor((windowWidth / 2) - 
       ((mButtons[i].mTexture.Width * (windowWidth / 1600.0f)) / 2.0f));
      mButtons[i].xMax = (int)floor((windowWidth / 2) + 
       ((mButtons[i].mTexture.Width * (windowWidth / 1600.0f)) / 2.0f));

      mButtons[i].yMin = (int)floor(((i + 2) * windowHeight / 6) - 
       ((mButtons[i].mTexture.Height * (windowHeight / 1200.0f)) / 2.0f));
      mButtons[i].yMax = (int)floor(((i + 2) * windowHeight / 6) + 
       ((mButtons[i].mTexture.Height * (windowHeight / 1200.0f)) / 2.0f));
   }

   return true;
}


void PauseMenuMode::Start() {
   mButtonPressed = -1;

   // get the last frame rendered
   glReadBuffer(GL_FRONT);
   //glPixelStorei(GL_PACK_ALIGNMENT,1);
   unsigned char *buf = (unsigned char *)malloc(sizeof(char) * 3 * 
    mGameWindow->GetWidth() * mGameWindow->GetHeight());
   glReadPixels(0, 0, mGameWindow->GetWidth(), mGameWindow->GetHeight(), 
    GL_RGB, GL_UNSIGNED_BYTE, buf);

   // bind it to a texture
   glGenTextures(1, &mBackgroundTexture);
   glBindTexture(GL_TEXTURE_2D, mBackgroundTexture);
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, mGameWindow->GetWidth(), 
    mGameWindow->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, buf);
   free(buf);

   glEnable(GL_TEXTURE_2D);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   mNextMode = NULL;

   InputManager *im = mGameWindow->GetInputManager();

   im->AddKeyEvent(&mResume, SDLK_ESCAPE);

   im->AddMouseEvent(&mClick, IM_MOUSE_LEFT_BUTTON);

   im->ResetAllInputEvents();

   mGameWindow->SetOrthographicProjection();
}


void PauseMenuMode::Stop() {
   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);

   glDeleteTextures(1, &mBackgroundTexture);

   mGameWindow->ResetPerspectiveProjection();
}


void PauseMenuMode::Update(float timeElapsed) {
   if (mResume.IsPressed())
      mNextMode = mWorldMode;
   if (mClick.IsPressed()) {
      InputManager *im = mGameWindow->GetInputManager();

      int mouseX = im->GetMouseX();
      int mouseY = im->GetMouseY();

      mButtonPressed = -1;
      for (int i = 0; i < NUM_PAUSE_BUTTONS; ++i) {
         if (mButtons[i].IsIn(mouseX, mouseY)) {
            mButtonPressed = i;
         }
      }
   }
   else {
      if (mButtonPressed == BTN_RESUME) {
         mNextMode = mWorldMode;
      }
      else if (mButtonPressed == BTN_OPTIONS) {
         // TODO: add options menu
      }
      else if (mButtonPressed == BTN_QUIT) {
         mNextMode = mMainMenuMode;
      }
      mButtonPressed = -1;
   }
}


void PauseMenuMode::Render() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);

   // draw background
   glPushMatrix();
   glLoadIdentity();

   glActiveTexture(GL_TEXTURE1_ARB);
   glEnable(GL_TEXTURE_2D);

   glActiveTexture(GL_TEXTURE0_ARB);
   glBindTexture(GL_TEXTURE_2D, mBackgroundTexture);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

   glActiveTexture(GL_TEXTURE1_ARB);
   glBindTexture(GL_TEXTURE_2D, mBackgroundLightmap.TextureID);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   glBegin(GL_QUADS);
      glMultiTexCoord2f(GL_TEXTURE0_ARB, 0.0, 0.0);
      glMultiTexCoord2f(GL_TEXTURE1_ARB, 0.0, 0.0);
      glVertex2i(0, mGameWindow->GetHeight());

      glMultiTexCoord2f(GL_TEXTURE0_ARB, 1.0, 0.0);
      glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 0.0);
      glVertex2i(mGameWindow->GetWidth(), mGameWindow->GetHeight());

      glMultiTexCoord2f(GL_TEXTURE0_ARB, 1.0, 1.0);
      glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 1.0);
      glVertex2i(mGameWindow->GetWidth(), 0);

      glMultiTexCoord2f(GL_TEXTURE0_ARB, 0.0, 1.0);
      glMultiTexCoord2f(GL_TEXTURE1_ARB, 0.0, 1.0);
      glVertex2f(0, 0);
   glEnd();
   glPopMatrix();

   glActiveTexture(GL_TEXTURE1_ARB);
   glDisable(GL_TEXTURE_2D);

   glActiveTexture(GL_TEXTURE0_ARB);

   // draw menu back
   glPushMatrix();
   glLoadIdentity();

   int texW = (int)floor(mMenuTexture.Width * (mGameWindow->GetWidth() / 1600.0f));
   int texH = (int)floor(mMenuTexture.Height * (mGameWindow->GetHeight() / 1200.0f));

   glTranslatef(mGameWindow->GetWidth() / 2.0f, mGameWindow->GetHeight() / 2.0f, 0);
   glBindTexture(GL_TEXTURE_2D, mMenuTexture.TextureID);
   glBegin(GL_QUADS);
      glTexCoord2i(0, 0);
      glVertex2f(-texW / 2.0f, texH / 2.0f);

      glTexCoord2i(1, 0);
      glVertex2f(texW / 2.0f, texH / 2.0f);

      glTexCoord2i(1, 1);
      glVertex2f(texW / 2.0f, -texH / 2.0f);

      glTexCoord2i(0, 1);
      glVertex2f(-texW / 2.0f, -texH / 2.0f);
   glEnd();
   glPopMatrix();

   // draw resume button
   glPushMatrix();
   glLoadIdentity();

   texW = (int)floor(mButtons[BTN_RESUME].mTexture.Width * 
    (mGameWindow->GetWidth() / 1600.0f));
   texH = (int)floor(mButtons[BTN_RESUME].mTexture.Height *
    (mGameWindow->GetHeight() / 1200.0f));

   glTranslatef(mGameWindow->GetWidth() / 2.0f, 2 * mGameWindow->GetHeight() / 6.0f, 0);
   if (mButtonPressed == BTN_RESUME)
      glBindTexture(GL_TEXTURE_2D, 
       mButtons[BTN_RESUME].mPressedTexture.TextureID);
   else
      glBindTexture(GL_TEXTURE_2D, 
       mButtons[BTN_RESUME].mTexture.TextureID);
   glBegin(GL_QUADS);
      glTexCoord2i(0, 0);
      glVertex2f(-texW / 2.0f, texH / 2.0f);

      glTexCoord2i(1, 0);
      glVertex2f(texW / 2.0f, texH / 2.0f);

      glTexCoord2i(1, 1);
      glVertex2f(texW / 2.0f, -texH / 2.0f);

      glTexCoord2i(0, 1);
      glVertex2f(-texW / 2.0f, -texH / 2.0f);
   glEnd();
   glPopMatrix();

   // draw options button
   glPushMatrix();
   glLoadIdentity();

   texW = (int)floor(mButtons[BTN_OPTIONS].mTexture.Width * 
    (mGameWindow->GetWidth() / 1600.0));
   texH = (int)floor(mButtons[BTN_OPTIONS].mTexture.Height *
    (mGameWindow->GetHeight() / 1200.0));

   glTranslatef(mGameWindow->GetWidth() / 2.0f, 3 * mGameWindow->GetHeight() / 6.0f, 0);
   if (mButtonPressed == BTN_OPTIONS)
      glBindTexture(GL_TEXTURE_2D, 
       mButtons[BTN_OPTIONS].mPressedTexture.TextureID);
   else
      glBindTexture(GL_TEXTURE_2D, 
       mButtons[BTN_OPTIONS].mTexture.TextureID);
   glBegin(GL_QUADS);
      glTexCoord2i(0, 0);
      glVertex2f(-texW / 2.0f, texH / 2.0f);

      glTexCoord2i(1, 0);
      glVertex2f(texW / 2.0f, texH / 2.0f);

      glTexCoord2i(1, 1);
      glVertex2f(texW / 2.0f, -texH / 2.0f);

      glTexCoord2i(0, 1);
      glVertex2f(-texW / 2.0f, -texH / 2.0f);
   glEnd();
   glPopMatrix();

   // draw resume button
   glPushMatrix();
   glLoadIdentity();

   texW = (int)floor(mButtons[BTN_QUIT].mTexture.Width * 
    (mGameWindow->GetWidth() / 1600.0f));
   texH = (int)floor(mButtons[BTN_QUIT].mTexture.Height *
    (mGameWindow->GetHeight() / 1200.0f));

   glTranslatef(mGameWindow->GetWidth() / 2.0f, 4 * mGameWindow->GetHeight() / 6.0f, 0);
   if (mButtonPressed == BTN_QUIT)
      glBindTexture(GL_TEXTURE_2D, 
       mButtons[BTN_QUIT].mPressedTexture.TextureID);
   else
      glBindTexture(GL_TEXTURE_2D, 
       mButtons[BTN_QUIT].mTexture.TextureID);
   glBegin(GL_QUADS);
      glTexCoord2i(0, 0);
      glVertex2f(-texW / 2.0f, texH / 2.0f);

      glTexCoord2i(1, 0);
      glVertex2f(texW / 2.0f, texH / 2.0f);

      glTexCoord2i(1, 1);
      glVertex2f(texW / 2.0f, -texH / 2.0f);

      glTexCoord2i(0, 1);
      glVertex2f(-texW / 2.0f, -texH / 2.0f);
   glEnd();
   glPopMatrix();

   SDL_GL_SwapBuffers();
}