#include "Vector3.h"
#include <cmath>
#include <assert.h>


float Vector3::DotProduct(const Vector3 &rhs) const {
   return mX * rhs.mX + mY * rhs.mY + mZ * rhs.mZ;
}


Vector3 Vector3::CrossProduct(const Vector3 &rhs) const {
   return Vector3(mY * rhs.mZ - mZ * rhs.mY,
      mZ * rhs.mX - mX * rhs.mZ,
      mX * rhs.mY - mY * rhs.mX);
}


// The length of a vector is the square root of the sum of the sqaure
// of the elements
float Vector3::Length() const {
   return sqrtf(mX * mX + mY * mY + mZ * mZ);
}

float Vector3::DistanceTo(const Vector3 &other) const { 
	float dx, dy, dz; 
	dx = other.mX - mX; 
	dy = other.mY - mY; 
	dz = other.mZ - mZ; 
   return sqrtf((dx*dx) + (dy*dy) + (dz*dz)); 
} 

// To normalize a vector, divide its elements by its length
void Vector3::Normalize() {
   float length = Length();
   if (length != 0) {
      mX /= length;
      mY /= length;
      mZ /= length;
   }
}

Vector3 Vector3::Normalized() const {
   Vector3 v;
   float length = Length();

   if (length != 0) {
      v.mX = mX / length;
      v.mY = mY / length;
      v.mZ = mZ / length;
   }

   return v;
}

void Vector3::Zero() {
   mX = mY = mZ = 0;
}

Vector3 Vector3::operator+(const Vector3 &rhs) const {
   return Vector3(mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ);
}


Vector3 Vector3::operator-(const Vector3 &rhs) const {
   return Vector3(mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ);
}

Vector3 Vector3::operator/(const Vector3 &rhs) const {
    return Vector3(mX/rhs.mX, mY/rhs.mY, mZ/rhs.mZ);
}

Vector3 Vector3::operator*(const Vector3 &rhs) const {
    return Vector3(mX*rhs.mX, mY*rhs.mY, mZ*rhs.mZ);
}


Vector3 Vector3::operator/(float rhs) {
   return Vector3(mX / rhs, mY / rhs, mZ / rhs);
}

Vector3 Vector3::operator*(float rhs) {
   return Vector3(mX * rhs, mY * rhs, mZ * rhs);
}

Vector3 Vector3::operator+=(const Vector3 &rhs) {
   mX += rhs.mX; mY += rhs.mY; mZ += rhs.mZ;
   return *this;
}

Vector3 Vector3::operator-=(const Vector3 &rhs) {
   mX -= rhs.mX; mY -= rhs.mY; mZ -= rhs.mZ;
   return *this;
}

Vector3 operator-(const Vector3 &rhs) {
    return Vector3(-rhs.mX, -rhs.mY, -rhs.mZ);
}

Vector3 operator*(float lhs, const Vector3 &rhs) {
   return Vector3(lhs * rhs.mX, lhs * rhs.mY, lhs * rhs.mZ);
}