#include <windows.h>
#include <GL/glee.h>
#include <SDL/SDL_OpenGL.h>
#include <assert.h>

#include "GameOverMode.h"
#include "InputManager.h"
#include "GameWindow.h"


GameOverMode::~GameOverMode() {
   mTextureLoader.FreeTexture(&mWinTexture);
   mTextureLoader.FreeTexture(&mLoseTexture);
}


void GameOverMode::SetNeighborModes(const std::vector<GameMode *> &modes) {
   assert(modes.size() == 1);

   mMainMenuMode = modes[0];
}


bool GameOverMode::LoadResources() {
   mTextureLoader.SetMipMapping(true);
   mTextureLoader.SetTextureFilter(txBilinear);

   assert(mTextureLoader.LoadTextureFromDisk("menu/game_over_win.tga", 
    &mWinTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("menu/game_over_lose.tga", 
    &mLoseTexture) == TRUE);
   assert(mTextureLoader.LoadTextureFromDisk("menu/pause_lightmap.jpg", 
    &mBackgroundLightmap) == TRUE);

   return true;
}


void GameOverMode::Start() {
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

   im->AddMouseEvent(&mClick, IM_MOUSE_LEFT_BUTTON);

   im->ResetAllInputEvents();

   mGameWindow->SetOrthographicProjection();
}


void GameOverMode::Stop() {
   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);

   glDeleteTextures(1, &mBackgroundTexture);

   mGameWindow->ResetPerspectiveProjection();
}


void GameOverMode::Update(float timeElapsed) {
   if (mClick.IsPressed()) {
      mNextMode = mMainMenuMode;
   }
}


void GameOverMode::Render() {
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
      glVertex2f(0, mGameWindow->GetHeight());

      glMultiTexCoord2f(GL_TEXTURE0_ARB, 1.0, 0.0);
      glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 0.0);
      glVertex2f(mGameWindow->GetWidth(), mGameWindow->GetHeight());

      glMultiTexCoord2f(GL_TEXTURE0_ARB, 1.0, 1.0);
      glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 1.0);
      glVertex2f(mGameWindow->GetWidth(), 0);

      glMultiTexCoord2f(GL_TEXTURE0_ARB, 0.0, 1.0);
      glMultiTexCoord2f(GL_TEXTURE1_ARB, 0.0, 1.0);
      glVertex2f(0, 0);
   glEnd();
   glPopMatrix();

   glActiveTexture(GL_TEXTURE1_ARB);
   glDisable(GL_TEXTURE_2D);

   glActiveTexture(GL_TEXTURE0_ARB);

   // draw win/lose
   glPushMatrix();
   glLoadIdentity();
   glTranslatef(mGameWindow->GetWidth()/2, mGameWindow->GetHeight()/2, 0);

   int texW = mWinTexture.Width * (mGameWindow->GetWidth() / 1600.0);
   int texH = mWinTexture.Height * (mGameWindow->GetHeight() / 1200.0);

   if (mWin)
      glBindTexture(GL_TEXTURE_2D, mWinTexture.TextureID);
   else
      glBindTexture(GL_TEXTURE_2D, mLoseTexture.TextureID);
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