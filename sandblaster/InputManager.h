/*
 * InputManager.h
 * 
 * This class handles all the input events for the game. Input events
 * include key events and mouse events.
 *
 * by: Scott Daniel
 * CPE 476 Spring '07
 */

//Mouse support is not yet implemented

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

//#include "InputEvent.h"

#include <string>

#define NUM_KEY_CODES   300
#define NUM_MOUSE_CODES 10

#define IM_MOUSE_LEFT_BUTTON      1
#define IM_MOUSE_MIDDLE_BUTTON    2
#define IM_MOUSE_RIGHT_BUTTON     3
#define IM_MOUSE_WHEELUP_BUTTON	 4
#define IM_MOUSE_WHEELDOWN_BUTTON 5
#define IM_MOUSE_MOVE_LEFT        6
#define IM_MOUSE_MOVE_RIGHT       7
#define IM_MOUSE_MOVE_UP          8
#define IM_MOUSE_MOVE_DOWN        9

class InputEvent;

class InputManager {
	InputEvent *mKeyInputEvents[NUM_KEY_CODES];
   InputEvent *mMouseInputEvents[NUM_MOUSE_CODES];


	//InputManager();
	//static InputManager *mInputManager;
public:
   InputManager();

   InputEvent *GetKeyEvent(int);
	InputEvent *GetMouseEvent(int);
   void MouseMove(int, int, int);
   int GetMouseX();
   int GetMouseY();
	//static InputManager *getInputManager() {return mInputManager;}
	//static void Initialize();

	//std::string getKeyName(in);

	//void HandleInput();

	void AddKeyEvent(InputEvent *, int);
   void AddMouseEvent(InputEvent *, int);

	void RemoveKeyEvent(InputEvent *);
	void RemoveMouseEvent(InputEvent *);
	void RemoveInputEvent(InputEvent *);

	void ClearKeyEvents();
	void ClearMouseEvents();
	void ClearAllInputEvents();

	void ResetAllInputEvents();

};

#endif