#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3.h"

class Entity {
public:
    void Initialize(float phi, float theta, float mass, float drag,
        const Vector3 &pos, 
        const Vector3 &prevPos = Vector3(0,0,0),
        const Vector3 &vel = Vector3(0,0,0))
    {
        mPhi = phi; mTheta = mCamTheta = theta;
        mMass = mass;
        mPos = pos; mPrevPos = prevPos; mVel = vel;

        SetDir(mPhi, mTheta);
		camDir = mDir;
        mDrag = drag;
    }

    virtual void Update(float dt) = 0;

    void ApplyForce(const Vector3 &force) {mForce += force;}
    void RotateYaw(float theta) {mTheta += theta; SetDir(mPhi, mTheta);}
	void RotateCamYaw(float theta) {mCamTheta += (theta*0.85f); SetCamDir(mPhi, mCamTheta);}
    void RotatePitch(float phi) {mPhi += phi; SetDir(mPhi, mTheta);}

    Vector3 GetW() const {return mDir;}
    Vector3 GetU() const {return GetW().CrossProduct(Vector3(0, 1, 0));}
    Vector3 GetV() const {return GetU().CrossProduct(GetW());}
	Vector3 GetCamDir() {return camDir;}

    float GetPhi()            const {return mPhi;}
    float GetTheta()          const {return mTheta;}
    float GetMass() const {return mMass;}
    Vector3 GetPos()     const {return mPos;}
    Vector3 GetPrevPos() const {return mPrevPos;}
    Vector3 GetVel()     const {return mVel;}
    Vector3 GetForce()   const {return mForce;}

    void SetPhi(float phi)                         {mPhi = phi;}
    void SetTheta(float theta)                     {mTheta = theta;}
    void SetMass(float mass)            {mMass = mass;}
    void SetPos(const Vector3 &pos)         {mPos = pos;}
    void SetPrevPos(const Vector3 &prevPos) {mPrevPos = prevPos;}
    void SetVel(const Vector3 &vel)         {mVel = vel;}
    void SetForce(const Vector3 &force) {mForce = force;}
    void SetDir(float phi, float theta);
	void SetCamDir(float phi, float theta);

protected:
   float mPhi, mTheta, mCamTheta;
   float mMass;

   float mDrag;

   Vector3 mPos, mPrevPos, mVel, mForce, mDir, camDir;

};

#endif