#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <windows.h>
#include <GL/Glee.h>
#include <SDL/SDL_OpenGL.h>
#include <SDL/SDL.h>
#include <string>

#include "GameModeManager.h"
#include "InputManager.h"

using std::string;

class GameWindow {
public:
	GameWindow(const string& title, const int width, const int height, const int fps = 30);

	bool Initialize();
	void Run();
	bool ShutDown();

	void QuitGame() {mDone = true;}
	int GetWidth() {return mScreen->w;}
	int GetHeight() {return mScreen->h;}
	InputManager *GetInputManager() {return &mInputManager;}

	void SetOrthographicProjection();
	void ResetPerspectiveProjection();

protected:
	void HandleEvents();
	void HandleResize(const SDL_ResizeEvent &event);
	void InitializeSDL();
	void InitializeGL();

	//bool mFullScreen;
	//SDL_Surface *mIcon;

	SDL_Surface *mScreen;
	long mFrameDelay;           //in milliseconds
	string mWindowTitle;
	int mInitWidth, mInitHeight;
	bool mDone;

	GameModeManager mGameModeManager;
	InputManager mInputManager;
};

#endif