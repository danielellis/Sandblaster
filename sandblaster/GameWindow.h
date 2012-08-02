#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <SDL/SDL.h>
#include <cstring>
#include "GameModeManager.h"
#include "InputManager.h"

//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;

//const float MAX_FPS = 30.0;
const int MAX_WINDOW_NAME = 34;


class GameWindow {
public:
   GameWindow(char *title, int width = 1024, int height = 768, int fps = 30)
	 : mScreen(NULL), mFrameDelay(1000/fps), mInitWidth(width), 
    mInitHeight(height), mDone(false)
   {strncpy_s(mWindowTitle, MAX_WINDOW_NAME, title, MAX_WINDOW_NAME);}

   bool Initialize();
   bool ShutDown();

   void Run();
   void QuitGame() {mDone = true;}

   int GetWidth() {return mScreen->w;}
   int GetHeight() {return mScreen->h;}
   long GetFrameDelay() {return mFrameDelay;}
   InputManager *GetInputManager() {return &mInputManager;}

   void SetOrthographicProjection();
   void ResetPerspectiveProjection();

   SDL_Surface *mScreen;

protected:
   void HandleEvents();
   void HandleResize(const SDL_ResizeEvent &event);

   void InitializeGL();

   // window variables
   //SDL_Surface *mScreen;
   long mFrameDelay;           //in milliseconds
   char mWindowTitle[MAX_WINDOW_NAME];
   int mInitWidth, mInitHeight;
   //char mIcon;
   //bool mFullScreen;
   //bool mIsActive;
   bool mDone;

   GameModeManager mGameModeManager;
   InputManager mInputManager;
};

#endif