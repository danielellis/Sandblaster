#include <vector>

#include "GameModeManager.h"

#include "CharacterSelectMode.h"
#include "GameWindow.h"
#include "GameOverMode.h"
#include "MainMenuMode.h"
#include "PauseMenuMode.h"
#include "WorldMode.h"

GameModeManager::GameModeManager(GameWindow *window)
    : gameWindow(window)
{ }

void GameModeManager::Initialize() {
    // modes is used to pass neighboring states to each individual state
    std::vector<GameMode *> modes;

    // Construct all modes and add them to the modes list
    worldMode           = new WorldMode(gameWindow);
    mainMenuMode        = new MainMenuMode(gameWindow);
    pauseMenuMode       = new PauseMenuMode(gameWindow);
    characterSelectMode = new CharacterSelectMode(gameWindow);
    gameOverMode        = new GameOverMode(gameWindow);

    // TODO push the resource loading back. We don't need all the world mode
    // resources before we load the main menu, for example.
    mainMenuMode->LoadResources();
    worldMode->LoadResources();
    pauseMenuMode->LoadResources();
    characterSelectMode->LoadResources();
    gameOverMode->LoadResources();

    modes.clear();
   
    modes.push_back(characterSelectMode);
    mainMenuMode->SetNeighborModes(modes);

    modes.clear();

    modes.push_back(pauseMenuMode);
    modes.push_back(gameOverMode);
    worldMode->SetNeighborModes(modes);
   
    modes.clear();

    modes.push_back(worldMode);
    modes.push_back(mainMenuMode);
    pauseMenuMode->SetNeighborModes(modes);
   
    modes.clear();

    modes.push_back(worldMode);
    modes.push_back(mainMenuMode);
    characterSelectMode->SetNeighborModes(modes);
   
    modes.clear();

    modes.push_back(mainMenuMode);
    gameOverMode->SetNeighborModes(modes);
   
    modes.clear();

    // Load the first game mode: The Main Menu
    currentMode = mainMenuMode;
    currentMode->Start();
}

void GameModeManager::Shutdown() {
    // Delete modes
    delete worldMode;
    delete mainMenuMode;
    delete pauseMenuMode;
    delete characterSelectMode;
    delete gameOverMode;
}

void GameModeManager::SetMode(GameMode *mode) {
    InputManager *im;

    im = gameWindow->GetInputManager();
    currentMode->Stop();
    im->ClearAllInputEvents();

    currentMode = mode;
    currentMode->Start();
}

void GameModeManager::UpdateCurrentMode(float deltaSeconds) {
    GameMode *nextMode;

    nextMode = currentMode->CheckForModeChange();
    if (nextMode) {
        SetMode(nextMode);
    }
    currentMode->Update(deltaSeconds);
}
