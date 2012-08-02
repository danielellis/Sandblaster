/*
 * InputEvent.h
 * 
 * This class represents an in game event that occurs when the
 * user supplies input. InputEvents are mapped to a physical
 * input from the user such as a key press or mouse movement.
 *
 * by: Scott Daniel
 * CPE 476 Spring '07
 */


#ifndef INPUTEVENT_H
#define INPUTEVENT_H

// InputEvent Behaviors
#define IE_CONTINUOUS 0
#define IE_INITIAL    1

// InputEvent States
#define IE_RELEASED   0
#define IE_PRESSED    1
#define IE_WAIT_FOR_RELEASE 2

class InputEvent {
	int mBehavior;
	int mAmount;
	int mState;

public:
   InputEvent() : mBehavior(IE_CONTINUOUS) {Reset();}
	InputEvent(int behavior) : mBehavior(behavior) {Reset();}

    /* Returns state of event, most likely used for keys */
	bool IsPressed() {return GetAmount() != 0;}

    /* Returns amount event has fired, most likely used for amount of mouse motion */
	int GetAmount() {
		int retAmt = mAmount;
		if (retAmt != 0) {
         if (mState == IE_RELEASED) {
				mAmount = 0;
         }
			else if (mBehavior == IE_INITIAL) {
				mState = IE_WAIT_FOR_RELEASE;
				mAmount = 0;
			}
		}
		return retAmt;
	}

	void Reset() {mState = IE_RELEASED; mAmount = 0;}

	void Tap() {Press(); Release();}
	void Press() {Press(1);}
	void Press(int amt) {
		if (mState != IE_WAIT_FOR_RELEASE) {
			mAmount += amt;
			mState = IE_PRESSED;
		}
	}
	void Release() {mState = IE_RELEASED;}
};

#endif