//MeshObject.h

//Created/Programmed  by Jonathan DeKlotz

//The MeshObject class sits between the GameObject class and the MeshStructure class.
//MeshObject is responsible for loading mesh files into its MeshStructure object and
//it provides methods to draw the object using OpenGL.

#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include "MeshStructure.h"
#include <windows.h>
#include <GL/Glee.h>
#include <GL/gl.h>
#include <fstream>
#include <string>



class MeshObject
{
public:
	MeshObject();
	MeshObject(std::string meshFileName);
	~MeshObject();

	void LoadMeshFromFile(std::string meshFileName);
	void LoadMeshFromStream(std::istream &meshStream);
	void GLDraw();
	void GLDrawAABB();
    void SmoothMesh();
    void Scale(float);

	float GetBottom();

	void GetMinVtx(float vtx[3]);
	void GetMaxVtx(float vtx[3]);

protected:
	void readLine(char *str, std::istream &meshStream);
	//void readLine2(char *str);

	//MeshObject objects will in most cases have their own MeshStructure object,
	//but in some cases they could share (2+ MeshObject objects have the same
	//pointer to a MeshStructure object)
	float AABB_v0[3], AABB_v1[3], AABB_v2[3], AABB_v3[3], 
	 AABB_v4[3], AABB_v5[3], AABB_v6[3], AABB_v7[3];

	MeshStructure *mesh;
	float minVtx[3], maxVtx[3];
    int mVertexID;
};

#endif
