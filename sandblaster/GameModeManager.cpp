#include <windows.h>
#include <GL/glee.h>
#include "WorldMode.h"
#include "MainMenuMode.h"
#include "PauseMenuMode.h"
#include "CharacterSelectMode.h"
#include "GameModeManager.h"
#include "GameWindow.h"
#include <vector>

using namespace std;

GameModeManager::GameModeManager() {
   // construct all modes and add them to the modes list
}


GameModeManager::~GameModeManager() {
   delete mWorldMode;
   delete mMainMenuMode;
   delete mPauseMenuMode;
   delete mCharacterSelectMode;
   delete mGameOverMode;
}

void GameModeManager::Initialize(GameWindow *window) {
   //modes is used to pass neighboring states to each individual state
   vector<GameMode *> modes;

   mWorldMode = new WorldMode(window);
   mMainMenuMode = new MainMenuMode(window);
   mPauseMenuMode = new PauseMenuMode(window);
   mCharacterSelectMode = new CharacterSelectMode(window);
   mGameOverMode = new GameOverMode(window);

   mGameWindow = window;

   mMainMenuMode->LoadResources();
   mWorldMode->LoadResources();
   mPauseMenuMode->LoadResources();
   mCharacterSelectMode->LoadResources();
   mGameOverMode->LoadResources();
   modes.clear();

   modes.push_back(mCharacterSelectMode);
   mMainMenuMode->SetNeighborModes(modes);
   modes.clear();

   modes.push_back(mPauseMenuMode);
   modes.push_back(mGameOverMode);
   mWorldMode->SetNeighborModes(modes);
   modes.clear();

   modes.push_back(mWorldMode);
   modes.push_back(mMainMenuMode);
   mPauseMenuMode->SetNeighborModes(modes);
   modes.clear();

   modes.push_back(mWorldMode);
   modes.push_back(mMainMenuMode);
   mCharacterSelectMode->SetNeighborModes(modes);
   modes.clear();

   modes.push_back(mMainMenuMode);
   mGameOverMode->SetNeighborModes(modes);
   modes.clear();

   mCurrentMode = mMainMenuMode;
   mCurrentMode->Start();
}

void GameModeManager::Shutdown() {
//delete modes
}

void GameModeManager::SetMode(GameMode *mode) {
   InputManager *im;

   im = mGameWindow->GetInputManager();
   mCurrentMode->Stop();
   im->ClearAllInputEvents();

   mCurrentMode = mode;
   mCurrentMode->Start();
}

//void GameModeManager::Quit() {}

void GameModeManager::UpdateCurrentMode(float deltaSeconds) {
   GameMode *nextMode;

   nextMode = mCurrentMode->CheckForModeChange();
   if (nextMode) {
      SetMode(nextMode);
   }
   mCurrentMode->Update(deltaSeconds);
}

