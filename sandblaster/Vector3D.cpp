//Vector and Coord utilities by Jonathan R. DeKlotz
//Implementation file

#include "Vector3D.h"


void Coord3D::WriteToBinaryStream(std::ostream &ostr) const
{
	ostr.write((char *)&x, sizeof(float));
	ostr.write((char *)&y, sizeof(float));
	ostr.write((char *)&z, sizeof(float));
}

void Coord3D::ReadFromBinaryStream(std::istream &istr)
{
	istr.read((char *)&x, sizeof(float));
	istr.read((char *)&y, sizeof(float));
	istr.read((char *)&z, sizeof(float));
}

std::ostream &operator<<(std::ostream &ostr, Coord3D c3d)
{
	ostr << c3d.x << " " << c3d.y << " " << c3d.z << " ";
	return ostr;
}

Vector3D operator*(Vector3D v3d, float sclr)
{
	return Vector3D(Coord3D(v3d.vec.x * sclr, v3d.vec.y * sclr, v3d.vec.z * sclr) , v3d.pos);
}

Vector3D operator+(Vector3D v3d1, Vector3D v3d2)
{
	return Vector3D(Coord3D(v3d1.vec.x + v3d2.vec.x, v3d1.vec.y + v3d2.vec.y, v3d1.vec.z + v3d2.vec.z) , v3d1.pos);
}

void operator+=(Coord3D &c3d, const Vector3D &v3d)
{
	c3d.x += v3d.vec.x;
	c3d.y += v3d.vec.y;
	c3d.z += v3d.vec.z;
}

void operator+=(Vector3D &v3d1, const Vector3D &v3d2)
{
	v3d1.vec += v3d2;
}

bool VecParallel(Vector3D op1, Vector3D op2)
{
	return op1.vec.x == op2.vec.x
		&& op1.vec.y == op2.vec.y
		&& op1.vec.z == op2.vec.z;
}

float dot(Vector3D op1, Vector3D op2)
{
	return op1.vec.x * op2.vec.x
		 + op1.vec.y * op2.vec.y
		 + op1.vec.z * op2.vec.z;
}

float dot(float *v0, float *v1)
{
	return *v0 * *v1
		 + *(v0 + 1) * *(v1 + 1)
		 + *(v0 + 2) * *(v1 + 2);
}

float dot4(float *v0, float *v1)
{
	return *v0 * *v1
		 + *(v0 + 1) * *(v1 + 1)
		 + *(v0 + 2) * *(v1 + 2)
		 + *(v0 + 3) * *(v1 + 3);
}

Vector3D cross(Vector3D op1, Vector3D op2)
{
	return Vector3D(Coord3D(op1.vec.y * op2.vec.z - op1.vec.z * op2.vec.y,
							op1.vec.z * op2.vec.x - op1.vec.x * op2.vec.z,
							op1.vec.x * op2.vec.y - op1.vec.y * op2.vec.x),	op1.pos);
}

void cross(float *v0, float *v1, float *vr)
{
	vr[0] = v0[1] * v1[2] - v0[2] * v1[1];
	vr[1] = v0[2] * v1[0] - v0[0] * v1[2];
	vr[2] = v0[0] * v1[1] - v0[1] * v1[0];
}

void normalize(float *vec)
{
	float len = length(vec);

	vec[0] /= len;
	vec[1] /= len;
	vec[2] /= len;
}

float length(float *vec)
{
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

float getAngle(Vector3D op1, Vector3D op2)
{
	return float(acos(dot(op1, op2)/(length(op1) * length(op2))) * 57.295779513);
}

bool checkRange(float angle)
{return angle <= 360.0 && angle >= -360.0;}

void normalize(Vector3D &op)
{
	float len = length(op);

	op.vec.x /= len;
	op.vec.y /= len;
	op.vec.z /= len;
}

float length(Vector3D op)
{
	return sqrt(dot(op, op));
}

Vector3D getProjection(Vector3D op1, Vector3D op2)
{
	float len;
	Vector3D resultVec = op2;
	normalize(resultVec);

	len = dot(op1, op2) / length(op2);
	
	resultVec.vec.x *= len;
	resultVec.vec.y *= len;

	return resultVec;
}

void ComputeFaceNormal(float *vtx0, float *vtx1, float *vtx2, float *norm)
{
	float v0[3];
	float v1[3];

	v0[0] = vtx1[0] - vtx0[0];
	v0[1] = vtx1[1] - vtx0[1];
	v0[2] = vtx1[2] - vtx0[2];

	v1[0] = vtx2[0] - vtx0[0];
	v1[1] = vtx2[1] - vtx0[1];
	v1[2] = vtx2[2] - vtx0[2];

	cross(v0, v1, norm);
	normalize(norm);
}