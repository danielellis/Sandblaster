#ifndef GAME_MODE_MANAGER_H
#define GAME_MODE_MANAGER_H

#include "GameMode.h"

class GameWindow;

class GameModeManager {
public:
   GameModeManager(GameWindow *window);

   void             Initialize();
   void             Shutdown();

   void             SetMode(GameMode *mode);

   void             UpdateCurrentMode(float elapsedTime);
   void             RenderCurrentMode() { currentMode->Render(); }

protected:
   GameWindow       *gameWindow;
   GameMode         *currentMode,
                    *worldMode,
                    *mainMenuMode,
                    *pauseMenuMode,
                    *characterSelectMode,
                    *gameOverMode;
};

#endif
