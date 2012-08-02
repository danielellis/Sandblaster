#include "DebugEye.h"

DebugEye::DebugEye() {
    Entity::Initialize(0, -PI/2.0f, 0, 0, Vector3(0, 10, 0), Vector3(0,10,0), Vector3(0,0,0));
}

void DebugEye::Update(float dt) {
    if (mForward)
        mPos += mDir * 10*dt;
    if (mBackward)
        mPos -= mDir * 10*dt;
    if (mRight)
        mPos += GetU() * 10*dt;
    if (mLeft)
        mPos -= GetU() * 10*dt;
    if (mUp)
        mPos += GetV() * 10*dt;
    if (mDown)
        mPos -= GetV() * 10*dt;

    mForward = mBackward = mRight = mLeft = mUp = mDown = false;
}