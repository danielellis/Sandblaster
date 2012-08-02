#include "InputManager.h"
#include "InputEvent.h"
#include <SDL/SDL.h>

#include <iostream>
using namespace std;

InputManager::InputManager() {
	ClearAllInputEvents();
}

InputEvent *InputManager::GetKeyEvent(int keyCode) {
	InputEvent *keyEvent = 0;

	if (keyCode < NUM_KEY_CODES) {
		keyEvent = mKeyInputEvents[keyCode];
	}

	return keyEvent;
}

InputEvent *InputManager::GetMouseEvent(int mouseCode) {
   InputEvent *mouseEvent = 0;

   if (mouseCode < NUM_MOUSE_CODES) {
      mouseEvent = mMouseInputEvents[mouseCode];
   }

   return mouseEvent;
}

void InputManager::MouseMove(int mouseNeg, int mousePos, int amount) {
   InputEvent *ie;

   if (amount < 0) {
      ie = mMouseInputEvents[mouseNeg];
   }
   else {
      ie = mMouseInputEvents[mousePos];
   }

   if (ie) {
      if (amount < 0) {
         ie->Press(-amount);
      }
      else {
         ie->Press(amount);
      }
      ie->Release();
   }
}

int InputManager::GetMouseX() {
   int x;

   SDL_GetMouseState(&x, NULL);

   return x;
}

int InputManager::GetMouseY() {
   int y;

   SDL_GetMouseState(NULL, &y);

   return y;
}

void InputManager::AddKeyEvent(InputEvent *ie, int keyCode) {
	mKeyInputEvents[keyCode] = ie;
}

void InputManager::AddMouseEvent(InputEvent *ie, int mouseCode) {
   mMouseInputEvents[mouseCode] = ie;
}

void InputManager::RemoveKeyEvent(InputEvent *ie) {
	for (unsigned i = 0; i < NUM_KEY_CODES; i++) {
		if (mKeyInputEvents[i] == ie)
			mKeyInputEvents[i] = 0;
	}
}

void InputManager::RemoveMouseEvent(InputEvent *ie) {
	for (unsigned i = 0; i < NUM_MOUSE_CODES; i++) {
		if (mMouseInputEvents[i] == ie)
			mMouseInputEvents[i] = 0;
	}
}

void InputManager::RemoveInputEvent(InputEvent *ie) {
   RemoveKeyEvent(ie);
   RemoveMouseEvent(ie);
}

void InputManager::ClearKeyEvents() {
	for (unsigned i = 0; i < NUM_KEY_CODES; i++) {
		mKeyInputEvents[i] = 0;
	}
}

void InputManager::ClearMouseEvents() {
	for (unsigned i = 0; i < NUM_MOUSE_CODES; i++) {
		mMouseInputEvents[i] = 0;
	}
}

void InputManager::ClearAllInputEvents() {
   ClearKeyEvents();
   ClearMouseEvents();
}

void InputManager::ResetAllInputEvents() {
   for (unsigned i = 0; i < NUM_KEY_CODES; i++) {
      if (mKeyInputEvents[i] != 0)
         mKeyInputEvents[i]->Reset();
   }

   for (unsigned j = 0; j < NUM_MOUSE_CODES; j++) {
      if (mMouseInputEvents[j] != 0)
         mMouseInputEvents[j]->Reset();
   }
}
