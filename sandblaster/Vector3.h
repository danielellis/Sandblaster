#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>

class Vector3 {
public:
   Vector3(float x = 0, float y = 0, float z = 0) : mX(x), mY(y), mZ(z) {}

   float GetX() const {return mX;}
   float GetY() const {return mY;}
   float GetZ() const {return mZ;}

   void SetX(float x) {mX = x;}
   void SetY(float y) {mY = y;}
   void SetZ(float z) {mZ = z;}

   float DotProduct(const Vector3 &rhs) const;
   Vector3 CrossProduct(const Vector3 &rhs) const;
   float DistanceTo(const Vector3 &other) const;
   float Length() const;
   void Normalize();
   Vector3 Normalized() const;
   void Zero();

   Vector3 operator+(const Vector3 &rhs) const;
   Vector3 operator-(const Vector3 &rhs) const;
   Vector3 operator*(const Vector3 &rhs) const;
   Vector3 operator/(const Vector3 &rhs) const;
   Vector3 operator/(float rhs);
   Vector3 operator*(float rhs);
   Vector3 operator+=(const Vector3 &rhs);
   Vector3 operator-=(const Vector3 &rhs);

   friend Vector3 operator-(const Vector3 &rhs);
   friend Vector3 operator*(float lhs, const Vector3 &rhs);

   friend std::ostream &operator <<(std::ostream &os, const Vector3 &rhs) {
        return os << "<" << rhs.mX << ", " << rhs.mY << ", " << rhs.mZ << ">";
   }

protected:
   float mX, mY, mZ;
};

#endif