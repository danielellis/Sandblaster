//Vector and Coord utilities by Jonathan R. DeKlotz

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>
#include <iostream>

typedef struct Coord3D
{
	Coord3D(float ix, float iy, float iz)
	{x = ix; y = iy; z = iz;}
	Coord3D() {x = y = z = 0.0;}
	static Coord3D Origin() {return Coord3D(0.0, 0.0, 0.0);}
	void WriteToBinaryStream(std::ostream &ostr) const;
	void ReadFromBinaryStream(std::istream &istr);

	float x, y, z;
}Coord3D;

typedef struct Vector3D
{
	Vector3D(Coord3D ivec, Coord3D ipos)
	{vec = ivec; pos = ipos;}
	Vector3D() {}
	float ComputeLength() {return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);}
	
	static Vector3D VecBetweenPoints(Coord3D pt1, Coord3D pt2)
	{return Vector3D(Coord3D(pt2.x - pt1.x, pt2.y - pt1.y, pt2.z - pt1.z) , pt1);}

	Coord3D vec;
	Coord3D pos;
}Vector3D;

std::ostream &operator<<(std::ostream &ostr, Coord3D c3d);
Vector3D operator*(Vector3D v3d, float sclr);
Vector3D operator+(Vector3D v3d1, Vector3D v3d2);
void operator+=(Coord3D &c3d, const Vector3D &v3d);
void operator+=(Vector3D &v3d1, const Vector3D &v3d2);
bool VecParallel(Vector3D op1, Vector3D op2);
float dot(Vector3D op1, Vector3D op2);
Vector3D cross(Vector3D op1, Vector3D op2);
float getAngle(Vector3D op1, Vector3D op2);
bool checkRange(float angle);
void normalize(Vector3D &op);
float length(Vector3D op);
Vector3D getProjection(Vector3D op1, Vector3D op2);

//faster versions that don't use Vector3D and Coord3D objects:
void ComputeFaceNormal(float *, float *, float *, float *);
void cross(float *, float *, float *);
float dot(float *, float *);
float dot4(float *, float *);
void normalize(float *);
float length(float *);

#endif