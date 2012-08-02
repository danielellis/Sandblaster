#ifndef GAME_OVER_MODE_H
#define GAME_OVER_MODE_H

#include "GameMode.h"
#include "TextureLoader.h"
#include "InputEvent.h"

class GameOverMode : public GameMode {
public:
   GameOverMode(GameWindow *window) : GameMode(window), 
    mClick(IE_CONTINUOUS) {}
   virtual ~GameOverMode();

   virtual void SetNeighborModes(const std::vector<GameMode *> &modes);
   virtual bool LoadResources();
   virtual GameMode *CheckForModeChange() {return mNextMode;}

   virtual void Start();
   virtual void Stop();

   virtual void Update(float timeElapsed);
   virtual void Render();

   void SetWin(bool win) {mWin = win;}

protected:
   TextureLoader mTextureLoader;
   glTexture mWinTexture, mLoseTexture, mBackgroundLightmap;
   GLuint mBackgroundTexture;

   GameMode *mMainMenuMode, *mNextMode;

   InputEvent mClick;

   bool mWin;
};

#endif