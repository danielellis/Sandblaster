#include "GameWindow.h"
#include "InputManager.h"
#include "InputEvent.h"
#include "Audio.h"
#include "Shader.h"

GameWindow::GameWindow(string title, int width, int height, int fps)
	: mScreen(NULL)
	, mWindowTitle(title)
	, mInitWidth(width)
	, mInitHeight(height)
	, mFrameDelay(1000/fps)
	, mDone(false)
{ }

bool GameWindow::Initialize() {
	// initialize sdl
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(-1);
	}

	// get video info
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	if(info == NULL) {
		/* This should probably never happen. */
		fprintf(stderr, "Video query failed: %s\n",
			SDL_GetError());
		exit(-3);
	}

	// specify gl attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// initialize the window
	mScreen = SDL_SetVideoMode(mInitWidth, mInitHeight, 
		info->vfmt->BitsPerPixel, SDL_OPENGL /*| SDL_FULLSCREEN*/);
	if (mScreen == NULL) {
		fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
		exit(-2);
	}
	SDL_WM_SetCaption(mWindowTitle.c_str(), 0);

	//SDL_ShowCursor(false);
	//SDL_WM_GrabInput(SDL_GRAB_ON);

	InitializeGL();
	InitializeShaderContext();

	// Set up audio
	setupaudio();

	mGameModeManager.Initialize(this);

	//playSound("music/grinch.wav", AUDIO_LOCAL);

	return true;
}

void GameWindow::Run() {
	long baseTime, currTime;
	float deltaSeconds;

	baseTime = SDL_GetTicks();

	while (!mDone) {

		HandleEvents();

		if ((currTime = SDL_GetTicks()) - baseTime > mFrameDelay) {
			deltaSeconds = (currTime - baseTime) * 0.001f;

			mGameModeManager.UpdateCurrentMode(deltaSeconds);
			mGameModeManager.RenderCurrentMode();

			baseTime = currTime;
		}
	}
}

bool GameWindow::ShutDown() {
	//mGameModeManager.Shutdown();

	// Close audio engine
	closeaudio();

	SDL_Quit();

	return true;
}

void GameWindow::HandleEvents() {
	SDL_Event event;
	InputEvent *ie;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			//mGameModeManager.Quit();
			mDone = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (ie = mInputManager.GetKeyEvent(event.key.keysym.sym))
				ie->Press();
		}
		else if (event.type == SDL_KEYUP) {
			if (ie = mInputManager.GetKeyEvent(event.key.keysym.sym))
				ie->Release();
			//mGameModeManager.HandleKeyUp(event.key.keysym);
		}
		else if (event.type == SDL_MOUSEMOTION) {
			mInputManager.MouseMove(IM_MOUSE_MOVE_LEFT, IM_MOUSE_MOVE_RIGHT, event.motion.xrel);
			mInputManager.MouseMove(IM_MOUSE_MOVE_UP, IM_MOUSE_MOVE_DOWN, event.motion.yrel);
			//mGameModeManager.HandleMouseMotion(event.motion);
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (ie = mInputManager.GetMouseEvent(event.button.button))
				ie->Press();
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			if (ie = mInputManager.GetMouseEvent(event.button.button))
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

void GameWindow::InitializeGL() {
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_TEXTURE_2D);

	glViewport(0, 0, mScreen->w, mScreen->h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, mScreen->w / (float)mScreen->h, 0.1f, 100);
}

void GameWindow::SetOrthographicProjection() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, mScreen->w, 0, mScreen->h);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// move the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0.0, (float)-mScreen->h, 0.0);
	glMatrixMode(GL_MODELVIEW);
}

void GameWindow::ResetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}