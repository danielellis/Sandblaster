#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <windows.h>
#include <GL/GLee.h>
#include <SDL/SDL_OpenGL.h>
#include <SDL/SDL.h>
#include <string>

#include "GameModeManager.h"
#include "InputManager.h"

using std::string;

class GameWindow {
public:
	GameWindow(const string& title, const int width, const int height, const int fps = 30);

	// Lifecycle methods
	bool            Initialize();
	void            Run();
	bool            ShutDown();

    int             GetHeight() { return screen->h; }
	int             GetWidth() { return screen->w; }
	InputManager *  GetInputManager() { return inputManager; }
    void            QuitGame() { done = true; }

	void            SetOrthographicProjection();
	void            ResetPerspectiveProjection();

protected:
    // Initialization helpers
	void            InitializeSDL();
	void            InitializeGL();

    // Event handling
	void            HandleEvents();
	void            HandleResize(const SDL_ResizeEvent &event);
	
    //--------------------------------------------------------------------------

    bool            done,
                    fullScreen;
    int             frameDelay, // In milliseconds
                    initWidth,
                    initHeight;
	string          windowTitle;

    SDL_Surface     *screen;
	GameModeManager *gameModeManager;
	InputManager    *inputManager;

	//SDL_Surface   *icon;
};

#endif