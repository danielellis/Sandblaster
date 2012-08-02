#ifndef GAME_MODE_H
#define GAME_MODE_H

#include <vector>

class GameWindow;

class GameMode {
public:
   GameMode(GameWindow *window) : mGameWindow(window) {}
   virtual ~GameMode() {}

   virtual void SetNeighborModes(const std::vector<GameMode *> &modes) = 0;
   virtual bool LoadResources() = 0;
   virtual GameMode *CheckForModeChange() = 0;

   virtual void Start() = 0;
   virtual void Stop() = 0;

   virtual void Update(float timeElapsed) = 0;
   virtual void Render() = 0;

protected:
   GameWindow *mGameWindow;
};

#endif