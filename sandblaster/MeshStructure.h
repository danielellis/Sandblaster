//MeshStructure.h
//
//Created/Programmed by Jonathan DeKlotz

#ifndef MESHSTRUCTURE_H
#define MESHSTRUCTURE_H

#include <list>
#include <map>
#include <vector>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "Vector3D.h"

class MeshStructure
{
public:
	class Triangle;
	class Edge;
	class Vertex;
	class Normal;

	class Triangle
	{
		friend  class MeshStructure;

	public:
		Triangle(Vertex *vtx[3]);		
		const Vertex *GetVertex(int ndx);
		const Edge *GetEdge(int ndx);
		void GetNormal(float &x, float &y, float &z) const;

		float texcoord[3][2];
		float normals[3][3];
		float binormals[3][3];
		float tangents[3][3];

	protected:
		Vertex *vertex[3];
		Edge *edge[3];
		float normal[3];

		int refCount;
	};

	class Edge
	{
		friend class MeshStructure;

	public:
		Edge();
		Edge(Vertex *vtx0, Vertex *vtx1);
		const Vertex *GetVertex(int ndx) const;
		const Vertex *GetOtherVertex(const Vertex *vtx) const;
		const Triangle *GetTriangle(int ndx) const;
		const Triangle *GetOtherTriangle(const Triangle *tri) const;

	protected:
		Vertex *vertex[2];
		Triangle *triangle[2];
		int refCount;
	};

	class Vertex
	{
		friend class MeshStructure;

	public:
		Vertex(int id);
		Vertex(float x, float y, float z, int id);
		void GetVertexLocation(float &x, float &y, float &z) const;
		void GetVertexLocation(float loc[3]) const;
		void GetVertexNormal(float &x, float &y, float &z) const;
		void GetVertexNormal(float vec[3]) const;
		void GetVertexBinormal(float vec[3]) const;
		void GetVertexTangent(float vec[3]) const;
		void GetTextureCoordinates(float vec[2]) const;
		int GetID() const;
		std::list<Edge *>::iterator EdgeIterBegin();
		std::list<Edge *>::iterator EdgeIterEnd();
		std::list<Triangle *>::iterator TriangleIterBegin();
		std::list<Triangle *>::iterator TriangleIterEnd();
		//ZJW added for smoothing
		void SetVertexPosition(float new_pos[3]);    

	protected:
		float pos[3];
		float normal[3];
		float texcoord[2];
		float tangent[3];
		float binormal[3];
		int ID;
		bool vtxUsed;
		std::list<Edge *> adjacentEdgeList;
		std::list<Triangle *> adjacentTriangleList;
	};



protected:
	class MapKey
	{
	public:
		MapKey(Vertex *ptr0, Vertex *ptr1);
		MapKey(Vertex *ptr[2]);
		bool operator<(const MapKey &op) const;

	protected:
		Vertex *vertex[2];
	};

public:
	MeshStructure();
	~MeshStructure();

	//SD - added to scale meshes to desired size
	void Scale(float scale);


	void AddVertex(float x, float y, float z, int vtxID);
	void MeshStructure::AddVertex(float x, float y, float z, int vtxID, float n1, float n2, float n3, float t1, float t2, 
		float tg1, float tg2, float tg3, float bn1, float bn2, float bn3);
	void AddTriangle(int vtxID0, int vtxID1, int vtxID2);
	void AddTriangle(int vtxID0, int vtxID1, int vtxID2,
		float normX, float normY, float normZ);
	void AddTriangle(int vtxID0, int vtxID1, int vtxID2,
		int nmlID0, int nmlID1, int nmlID2,
		int texID0, int texID1, int texID2);

	void ComputeVertexNormals();
	void CenterMesh();
	void NormalizeMeshScale(float scale);

	//COLLAPSE-EDGE FUNCTION ADDED 4/12/04 
	//this checks for manifold property
	//returns true if it was able to collapse the edge, false if failed (and nothing is done)
	//expects a new vertex (required)
	bool CollapseEdge(Edge *edge, float newVtxPos[3]); 

	//checks reference counts and outputs to a file for debuggin
	void ReferenceOutput();
	//checks a single edges local topology (data structure point to the right place)
	//called in ReferenceOutput();
	bool CheckEdge(Edge *edge);

	Edge *GetEdge(Vertex *vtx0, Vertex *vtx1) const;

	void GetAABB(float min[3], float max[3]) const;

	int GetVertexCount() const;
	int GetEdgeCount() const;
	int GetTriangleCount() const;

	std::list<Triangle *>::iterator TriangleIterBegin();
	std::list<Triangle *>::iterator TriangleIterEnd();
	std::list<Vertex *>::iterator VertexIterBegin();
	std::list<Vertex *>::iterator VertexIterEnd();
	std::list<Edge *>::iterator EdgeIterBegin();
	std::list<Edge *>::iterator EdgeIterEnd();

protected:
	Edge *AddEdge(Vertex *vtx0, Vertex *vtx1);
	void MapRemoveEdge(Edge *edge);
	void MapUpdateEdge(Edge *edge, Vertex *upd_vtx0, Vertex *upd_vtx1);

	float minVtx[3], maxVtx[3], centerPos[3];
	int numVertices;

	std::list<Triangle *> triangleList;
	std::list<Vertex *> vertexList;
	std::list<Edge *> edgeList;
	std::map<MapKey, Edge *> edgeMap;
	std::map<int, Vertex *> vertexMap;
};

#endif
