#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <windows.h>
#include <stdlib.h>
#include <gl/Glee.h>
#include <SDL/SDL_OpenGL.h>
#include <SDL/SDL.h>
#include <string>

#include "GameModeManager.h"
#include "InputManager.h"

using std::string;

//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;

//const float MAX_FPS = 30.0;

class GameWindow {
public:
	GameWindow(string title, int width = 1024, int height = 768, int fps = 30);

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

protected:
	void HandleEvents();
	void HandleResize(const SDL_ResizeEvent &event);
	void InitializeGL();

	//char mIcon;
	//bool mFullScreen;

	SDL_Surface *mScreen;
	long mFrameDelay;           //in milliseconds
	string mWindowTitle;
	int mInitWidth, mInitHeight;
	bool mDone;

	GameModeManager mGameModeManager;
	InputManager mInputManager;
};

#endif