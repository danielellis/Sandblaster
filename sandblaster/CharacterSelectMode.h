#ifndef CHARACTER_SELECT_MODE
#define CHARACTER_SELECT_MODE

#include "GameMode.h"
#include "TextureLoader.h"
#include "InputEvent.h"
#include "Sarge.h"
#include "Scrappy.h"
#include "WorldMode.h"
#include "SBLib.h"

const int NUM_SELECT_BUTTONS = 4;
const int BTN_WIDTH = 308;
const int BTN_HEIGHT = 104;

class CharacterSelectMode : public GameMode {
public:
   CharacterSelectMode(GameWindow *window) : GameMode(window), mNextMode(NULL),
    mClick(IE_CONTINUOUS), mButtonPressed(-1), mCharacterNdx(0), 
    mRotation(0), mSarge(false) {}
   ~CharacterSelectMode();

   void SetNeighborModes(const std::vector<GameMode *> &);
   bool LoadResources();
   GameMode *CheckForModeChange() {return mNextMode;}

   void Start();
   void Stop();

   void Update(float timeElapsed);
   void Render();

protected:
   struct Button {
      glTexture mTexture, mPressedTexture;
      int xMin, xMax, yMin, yMax;

      bool IsIn(int x, int y) 
       {return x >= xMin && x <= xMax && y >= yMin && y <= yMax;}
   };

   enum {BTN_PREVIOUS = 0, BTN_BACK = 1, BTN_OK = 2, BTN_NEXT = 3};

   GameMode *mNextMode;
   GameMode *mMainMenuMode;
   WorldMode *mWorldMode;

   TextureLoader mTextureLoader;
   glTexture mBackgroundTexture, mPortraitTextures[NUM_CHARACTERS],
    mStatTextures[NUM_CHARACTERS], mDescripTextures[NUM_CHARACTERS];
   Button mButtons[NUM_SELECT_BUTTONS];

   int mButtonPressed;
   int mCharacterNdx;

   Sarge mSarge;
   Scrappy mScrappy;
   float mRotation;

   InputEvent mClick;
};

#endif