#ifndef MAIN_MENU_MODE_H
#define MAIN_MENU_MODE_H

#include <vector>

#include "GameMode.h"
#include "GameWindow.h"
#include "InputEvent.h"
#include "TextureLoader.h"

const int NUM_BUTTONS = 3;

class MainMenuMode : public GameMode {
public:
    MainMenuMode(GameWindow *window);
    ~MainMenuMode();

   void             SetNeighborModes(const std::vector<GameMode *> &);
   bool             LoadResources();
   GameMode *       CheckForModeChange() {return mNextMode;}

   void             Start();
   void             Stop();

   void             Update(float timeElapsed);
   void             Render();

protected:
    struct Button {
        glTexture mTexture, mPressedTexture;
        int xMin, xMax, yMin, yMax;

        bool IsIn(int x, int y)  {
            return x >= xMin && x <= xMax && y >= yMin && y <= yMax;
        }
    };

    enum {
        BTN_PLAY = 0,
        BTN_OPTIONS = 1,
        BTN_QUIT = 2
    };

    InputEvent      mStart,
                    mQuit,
                    mClick;
    TextureLoader   mTextureLoader;
    Button          mButtons[NUM_BUTTONS];
    glTexture       mBackgroundTexture;
    glTexture       mTitleTexture;
    GameMode        *mCharacterSelectMode,
                    *mNextMode;
    int             mButtonPressed;
};

#endif
