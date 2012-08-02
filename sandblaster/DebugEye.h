#ifndef DEBUG_EYE_H
#define DEBUG_EYE_H

#include "Entity.h"
#include "SBLib.h"

class DebugEye : public Entity {
public:
    DebugEye();
    void Update(float dt);

    void MoveForward() {mForward = true;}
    void MoveBackward() {mBackward = true;}
    void MoveRight() {mRight = true;}
    void MoveLeft() {mLeft = true;}
    void MoveUp() {mUp = true;}
    void MoveDown() {mDown = true;}

private:
    bool mForward, mBackward, mRight, mLeft, mUp, mDown;
};

#endif