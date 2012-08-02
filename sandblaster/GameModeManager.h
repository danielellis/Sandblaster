#ifndef GAME_MODE_MANAGER_H
#define GAME_MODE_MANAGER_H

#include "GameMode.h"

class GameWindow;

class GameModeManager {
public:
   GameModeManager();
   virtual ~GameModeManager();

   void Initialize(GameWindow *window);
   void Shutdown();

   void SetMode(GameMode *);
   //void Quit();

   void UpdateCurrentMode(float elapsedTime);
   void RenderCurrentMode() {mCurrentMode->Render();}

protected:
   GameWindow *mGameWindow;
   GameMode *mCurrentMode;

   GameMode *mWorldMode, *mMainMenuMode, *mPauseMenuMode, 
    *mCharacterSelectMode, *mGameOverMode;
};

#endif