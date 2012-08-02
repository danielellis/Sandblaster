#ifndef PAUSE_MENU_MODE_H
#define PAUSE_MENU_MODE_H

#include "GameMode.h"
#include "TextureLoader.h"
#include "InputEvent.h"

const int NUM_PAUSE_BUTTONS = 3;

class PauseMenuMode : public GameMode {
public:
   PauseMenuMode(GameWindow *window) : GameMode(window), mNextMode(NULL),
    mResume(IE_INITIAL), mClick(IE_CONTINUOUS), mButtonPressed(-1) {}
   ~PauseMenuMode();

   void SetNeighborModes(const std::vector<GameMode *> &);
   bool LoadResources();
   GameMode *CheckForModeChange() {return mNextMode;}

   void Start();
   void Stop();

   void Update(float timeElapsed);
   void Render();

   void SetBackgroundTexture(GLuint texture) {mBackgroundTexture = texture;}

protected:
   struct Button {
      glTexture mTexture, mPressedTexture;
      int xMin, xMax, yMin, yMax;

      bool IsIn(int x, int y) 
       {return x >= xMin && x <= xMax && y >= yMin && y <= yMax;}
   };

   enum {BTN_RESUME = 0, BTN_OPTIONS = 1, BTN_QUIT = 2};

   GameMode *mNextMode;
   GameMode *mMainMenuMode, *mWorldMode;

   TextureLoader mTextureLoader;
   glTexture mMenuTexture, mBackgroundLightmap;
   Button mButtons[NUM_PAUSE_BUTTONS];
   GLuint mBackgroundTexture;

   int mButtonPressed;

   InputEvent mResume, mClick;
};

#endif