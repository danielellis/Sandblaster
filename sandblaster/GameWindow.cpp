#include "GameWindow.h"

#include "AudioManager.h"
#include "GameModeManager.h"
#include "InputEvent.h"
#include "InputManager.h"
#include "Shader.h"

GameWindow::GameWindow(const string& title, const int width, const int height, const int fps)
	: screen(NULL)
	, windowTitle(title)
	, initWidth(width)
	, initHeight(height)
	, frameDelay(1000/fps)
	, done(false)
    , fullScreen(false)
{
	gameModeManager = new GameModeManager(this);
	inputManager = new InputManager();
    audioManager = new AudioManager();
}

GameWindow::~GameWindow() {
    delete gameModeManager;
    delete inputManager;
    delete audioManager;
}

bool GameWindow::Initialize() {
	InitializeSDL();
	InitializeGL();
	InitializeShaderContext(); // In Shader.cpp

    audioManager->Initialize();
	gameModeManager->Initialize();

	//audioManager->Play("music/grinch.wav", AUDIO_LOCAL);

	return true;
}

void GameWindow::Run() {
	long baseTime, currTime;
	float deltaSeconds;

	baseTime = SDL_GetTicks();

	while (!done) {

		HandleEvents();

		if ((currTime = SDL_GetTicks()) - baseTime > frameDelay) {
			deltaSeconds = (currTime - baseTime) * 0.001f;

			gameModeManager->UpdateCurrentMode(deltaSeconds);
			gameModeManager->RenderCurrentMode();

			baseTime = currTime;
		}
	}
}

bool GameWindow::Shutdown() {
	gameModeManager->Shutdown();

	// Close audio engine
    audioManager->Shutdown();

	SDL_Quit();

	return true;
}

void GameWindow::HandleEvents() {
	SDL_Event event;
	InputEvent *ie;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			//mGameModeManager.Quit();
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (ie = inputManager->GetKeyEvent(event.key.keysym.sym))
				ie->Press();
		}
		else if (event.type == SDL_KEYUP) {
			if (ie = inputManager->GetKeyEvent(event.key.keysym.sym))
				ie->Release();
			//mGameModeManager.HandleKeyUp(event.key.keysym);
		}
		else if (event.type == SDL_MOUSEMOTION) {
			inputManager->MouseMove(IM_MOUSE_MOVE_LEFT, IM_MOUSE_MOVE_RIGHT, event.motion.xrel);
			inputManager->MouseMove(IM_MOUSE_MOVE_UP, IM_MOUSE_MOVE_DOWN, event.motion.yrel);
			//mGameModeManager.HandleMouseMotion(event.motion);
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (ie = inputManager->GetMouseEvent(event.button.button))
				ie->Press();
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			if (ie = inputManager->GetMouseEvent(event.button.button))
				ie->Release();
		}
		else if (event.type == SDL_VIDEORESIZE) {
			HandleResize(event.resize);
		}
	}
}

void GameWindow::HandleResize(const SDL_ResizeEvent &event) {
	int width = event.w;
	int height = event.h;

	if (height == 0) {
		height = 1;
	}

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, width / (float)height, 0.1f, 100.0f);
}

void GameWindow::InitializeSDL() {
	const SDL_VideoInfo *info;
    Uint32 videoModeFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;

    if (fullScreen) {
        videoModeFlags |= SDL_FULLSCREEN;
    }

	// Init SDL
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(-1);
	}

	// Get video info
	if((info = SDL_GetVideoInfo()) == NULL) {
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
		exit(-3);
	}
    
	// Set the window title
	SDL_WM_SetCaption(windowTitle.c_str(), NULL);

	// Initialize the window
	screen = SDL_SetVideoMode(
		initWidth,
		initHeight, 
		info->vfmt->BitsPerPixel,
		videoModeFlags
	);
	if (screen == NULL) {
		fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
		exit(-2);
	}
}

void GameWindow::InitializeGL() {
	// Clear to black
	glClearColor(0, 0, 0, 0);

    // Set the viewport to the entire window
    glViewport(0, 0, screen->w, screen->h);

    // Scale normals for lighting. See:
    // http://www.opengl.org/archives/resources/features/KilgardTechniques/oglpitfall/
	glEnable(GL_NORMALIZE);

    // Best blend function for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up perspective projection matrix
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    gluPerspective(45, screen->w / (float)screen->h, 0.1f, 100);
}

void GameWindow::SetOrthographicProjection() {
    // Switch to the projection matrix
	glMatrixMode(GL_PROJECTION);

    // Push current matrix to the stack, create a new orthographic projection
    // matrix that we will pop off after we are done rendering 2D stuff
	glPushMatrix();

	// Set a 2D orthographic projection
    glLoadIdentity();
	glOrtho(0, screen->w, 0, screen->h, -1, 1);

	// Invert the y axis so that down is now positive (like 2D screen coords)
	glScalef(1, -1, 1);

	// Move the origin from the bottom left corner to the upper left corner
	glTranslatef(0.0, static_cast<float>(screen->h * -1), 0.0);

    // Switch back to the model view matrix
	glMatrixMode(GL_MODELVIEW);
}

void GameWindow::ResetPerspectiveProjection() {
    // Switch to projection matrix
    glMatrixMode(GL_PROJECTION);

    // Pop off the matrix created in SetOrthographicProjection
	glPopMatrix();

    // Switch back to model view matrix
	glMatrixMode(GL_MODELVIEW);
}
