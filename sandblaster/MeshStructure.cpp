//MeshStructure.cpp
//
//Created/Programmed by Jonathan DeKlotz

#include "MeshStructure.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

#define PUSH_BOUNDS(max, min, val) max = val > max ? val : max; min = val < min ? val : min

void MeshStructure::Scale(float scale) {
	float x, y, z;

	maxVtx[0] = maxVtx[1] = maxVtx[2] = -(minVtx[0] = minVtx[1] = minVtx[2] = 1000000.0f);
	Vertex *vertex;

	for (std::list<Vertex *>::iterator vtxIter = vertexList.begin();
	 vtxIter != vertexList.end(); ++vtxIter)
	{
		vertex = *vtxIter;
		x = vertex->pos[0] *= scale;
		y = vertex->pos[1] *= scale;
		z = vertex->pos[2] *= scale;
		PUSH_BOUNDS(maxVtx[0], minVtx[0], x);
		PUSH_BOUNDS(maxVtx[1], minVtx[1], y);
		PUSH_BOUNDS(maxVtx[2], minVtx[2], z);
	}
}

MeshStructure::MapKey::MapKey(Vertex *ptr0, Vertex *ptr1)
{
	vertex[0] = ptr0;
	vertex[1] = ptr1;
}

MeshStructure::MapKey::MapKey(Vertex *ptr[2])
{
	vertex[0] = ptr[0];
	vertex[1] = ptr[1];
}

bool MeshStructure::MapKey::operator<(const MapKey &op) const
{
	if (__int64(vertex[0]) == __int64(op.vertex[0]))
		if (__int64(vertex[1]) == __int64(op.vertex[1]))
			return false;
		else
			return __int64(vertex[1]) < __int64(op.vertex[1]);
	else
		return __int64(vertex[0]) < __int64(op.vertex[0]);
}

MeshStructure::Triangle::Triangle(Vertex *vtx[3]) : refCount(0)
{
	vertex[0] = vtx[0];
	vertex[1] = vtx[1];
	vertex[2] = vtx[2];

}
		
const MeshStructure::Vertex *MeshStructure::Triangle::GetVertex(int ndx)
{
	return ndx < 3 && ndx >= 0 ? vertex[ndx] : 0;
}

const MeshStructure::Edge *MeshStructure::Triangle::GetEdge(int ndx)
{
	return ndx < 3 && ndx >= 0 ? edge[ndx] : 0;
}

void MeshStructure::Triangle::GetNormal(float &x, float &y, float &z) const
{
	x = normal[0];
	y = normal[1];
	z = normal[2];
}

MeshStructure::Edge::Edge() : refCount(0) {}

MeshStructure::Edge::Edge(Vertex *vtx0, Vertex *vtx1) : refCount(0)
{
	vertex[0] = vtx0;
	vertex[1] = vtx1;
	triangle[0] = triangle[1] = 0;
}

const MeshStructure::Vertex *MeshStructure::Edge::GetVertex(int ndx) const
{
	return ndx < 2 && ndx >= 0 ? vertex[ndx] : 0;
}

const MeshStructure::Vertex *MeshStructure::Edge::GetOtherVertex(const Vertex *vtx) const
{
	return vertex[0] == vtx ? vertex[1] : vertex[0];
}

const MeshStructure::Triangle *MeshStructure::Edge::GetTriangle(int ndx) const
{
	return ndx < 2 && ndx >= 0 ? triangle[ndx] : 0;
}

const MeshStructure::Triangle *MeshStructure::Edge::GetOtherTriangle(const Triangle *tri) const
{
	return triangle[0] == tri ? triangle[1] : triangle[0];
}

MeshStructure::Vertex::Vertex(int id) : ID(id), vtxUsed(false)
{
	normal[0] = normal[1] = normal[2] = 0.0f;
}

MeshStructure::Vertex::Vertex(float x, float y, float z, int id) : ID(id), vtxUsed(false)
{
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;

	normal[0] = normal[1] = normal[2] = 0.0f;
}

void MeshStructure::Vertex::GetVertexLocation(float &x, float &y, float &z) const
{
	x = pos[0];
	y = pos[1];
	z = pos[2];
}

void MeshStructure::Vertex::SetVertexPosition(float new_pos[3]) 
{
	pos[0] = new_pos[0];
	pos[1] = new_pos[1];
	pos[2] = new_pos[2];
}

void MeshStructure::Vertex::GetVertexLocation(float loc[3]) const
{
	for (int ndx = 0; ndx < 3; ++ndx) loc[ndx] = pos[ndx];
}

void MeshStructure::Vertex::GetVertexNormal(float &x, float &y, float &z) const
{
	x = normal[0];
	y = normal[1];
	z = normal[2];
}

void MeshStructure::Vertex::GetVertexNormal(float vec[3]) const
{
	for (int ndx = 0; ndx < 3; ++ndx) vec[ndx] = normal[ndx];
}

void MeshStructure::Vertex::GetTextureCoordinates(float vec[2]) const
{
	for (int ndx = 0; ndx < 2; ++ndx) vec[ndx] = texcoord[ndx];
}

void MeshStructure::Vertex::GetVertexBinormal(float vec[3]) const
{
	for (int ndx = 0; ndx < 3; ++ndx) vec[ndx] = binormal[ndx];
}

void MeshStructure::Vertex::GetVertexTangent(float vec[3]) const
{
	for (int ndx = 0; ndx < 3; ++ndx) vec[ndx] = tangent[ndx];
}

int MeshStructure::Vertex::GetID() const
{
	return ID;
}

std::list<MeshStructure::Edge *>::iterator MeshStructure::Vertex::EdgeIterBegin()
{
	return adjacentEdgeList.begin();
}

std::list<MeshStructure::Edge *>::iterator MeshStructure::Vertex::EdgeIterEnd()
{
	return adjacentEdgeList.end();
}

std::list<MeshStructure::Triangle *>::iterator MeshStructure::Vertex::TriangleIterBegin()
{
	return adjacentTriangleList.begin();
}

std::list<MeshStructure::Triangle *>::iterator MeshStructure::Vertex::TriangleIterEnd()
{
	return adjacentTriangleList.end();
}

void MeshStructure::ComputeVertexNormals()
{
	int adjTriCount;
	Vertex *vertex;
	std::list<Triangle *>::iterator triIter, triIterEnd;
	float x, y, z;
	for (std::list<Vertex *>::iterator vtxIter = vertexList.begin();
	 vtxIter != vertexList.end(); ++vtxIter)
	{
		vertex = *vtxIter;
		adjTriCount = 0;
		vertex->normal[0] = vertex->normal[1] = vertex->normal[2] = 0;
		triIterEnd = vertex->TriangleIterEnd();
		for (triIter = vertex->TriangleIterBegin(); triIter != triIterEnd; ++triIter)
		{
			(*triIter)->GetNormal(x, y, z);
			vertex->normal[0] += x;
			vertex->normal[1] += y;
			vertex->normal[2] += z;
			++adjTriCount;
		}
		vertex->normal[0] /= adjTriCount;
		vertex->normal[1] /= adjTriCount;
		vertex->normal[2] /= adjTriCount;
		normalize(vertex->normal);
	}
}

//ZJW note actually repositions vertices...
void MeshStructure::CenterMesh()
{
	float x, y, z;

	maxVtx[0] = maxVtx[1] = maxVtx[2] = -(minVtx[0] = minVtx[1] = minVtx[2] = 1000000.0f);
	Vertex *vertex;

	for (std::list<Vertex *>::iterator vtxIter = vertexList.begin();
		vtxIter != vertexList.end(); ++vtxIter)
	{
		vertex = *vtxIter;
		x = vertex->pos[0] -= centerPos[0];
		y = vertex->pos[1] -= centerPos[1];
		z = vertex->pos[2] -= centerPos[2];
		PUSH_BOUNDS(maxVtx[0], minVtx[0], x);
		PUSH_BOUNDS(maxVtx[1], minVtx[1], y);
		PUSH_BOUNDS(maxVtx[2], minVtx[2], z);
	}

	centerPos[0] = centerPos[1] = centerPos[2] = 0.0f;
}

void MeshStructure::NormalizeMeshScale(float scale)
{
	float vec[3];
	vec[0] = maxVtx[0] - minVtx[0];
	vec[1] = maxVtx[1] - minVtx[1];
	vec[2] = maxVtx[2] - minVtx[2];

	float objectSize = length(vec);
	float scaleFactor = scale / objectSize;
	float x, y, z;

	maxVtx[0] = maxVtx[1] = maxVtx[2] = -(minVtx[0] = minVtx[1] = minVtx[2] = 1000000.0f);
	Vertex *vertex;

	for (std::list<Vertex *>::iterator vtxIter = vertexList.begin();
	 vtxIter != vertexList.end(); ++vtxIter)
	{
		vertex = *vtxIter;
		x = vertex->pos[0] *= scaleFactor;
		y = vertex->pos[1] *= scaleFactor;
		z = vertex->pos[2] *= scaleFactor;
		PUSH_BOUNDS(maxVtx[0], minVtx[0], x);
		PUSH_BOUNDS(maxVtx[1], minVtx[1], y);
		PUSH_BOUNDS(maxVtx[2], minVtx[2], z);
	}
}

bool MeshStructure::CheckEdge(Edge *edge)
{
	Vertex *v00 = 0, *v01 = 0, *v10 = 0, *v11 = 0;
	Triangle *t0 = 0, *t1 = 0, *nt00 = 0, *nt01 = 0, *nt10 = 0, *nt11 = 0;
	Edge *e00 = 0, *e01 = 0, *e10 = 0, *e11 = 0;
	Triangle **pt00 = 0, **pt10 = 0;
	Edge **pe01 = 0, **pe11 = 0;
	Vertex **pv01 = 0, **pv11 = 0;
	Edge *modEdge0, *modEdge1;
	v00 = (Vertex *)edge->GetVertex(0);
	v01 = (Vertex *)edge->GetVertex(1);

	if (edge != GetEdge(v00, v01))
		return false;
	
	t0 = (Triangle *)edge->GetTriangle(0);
	t1 = (Triangle *)edge->GetTriangle(1);

	if (t0)
		for (register int ndx = 0; ndx < 3; ++ndx)
			if (t0->vertex[ndx] != v00 && t0->vertex[ndx] != v01)
				v10 = t0->vertex[ndx];
	if (t1)
		for (register int ndx = 0; ndx < 3; ++ndx)
			if (t1->vertex[ndx] != v00 && t1->vertex[ndx] != v01)
				v11 = t1->vertex[ndx];
	if (t0)
	{
		e00 = GetEdge(v01, v10);
		e01 = GetEdge(v00, v10);
	}

	if (t1)
	{
		e10 = GetEdge(v01, v11);
		e11 = GetEdge(v00, v11);
	}
	
	if (t0)
	{
		nt00 = e00->triangle[0] != t0 ? (pt00 = &(modEdge0 = e00)->triangle[1], e00->triangle[0])
		 : (pt00 = &(modEdge0 = e00)->triangle[0], e00->triangle[1]);
		nt01 = e01->triangle[0] != t0 ? e01->triangle[0] : e01->triangle[1];

		if (nt01)
			for (register int ndx = 0; ndx < 3; ++ndx)
				if (nt01->edge[ndx] == e01)
					pe01 = &nt01->edge[ndx];
	}

	if (t1)
	{
		nt10 = e10->triangle[0] != t1 ? (pt10 = &(modEdge1 = e10)->triangle[1], e10->triangle[0])
		 : (pt10 = &(modEdge1 = e10)->triangle[0], e10->triangle[1]);
		nt11 = e11->triangle[0] != t1 ? e11->triangle[0] : e11->triangle[1];

		if (nt11)
			for (register int ndx = 0; ndx < 3; ++ndx)
				if (nt11->edge[ndx] == e11)
					pe11 = &nt11->edge[ndx];
	}
	
	if (t0)
		for (register int ndx = 0; ndx < 3; ++ndx)
			if (t0->edge[ndx] != e00 && t0->edge[ndx] != edge && t0->edge[ndx] != e01)
				return false;
	
	if (t1)
		for (register int ndx = 0; ndx < 3; ++ndx)
			if (t1->edge[ndx] != e10 && t1->edge[ndx] != edge && t1->edge[ndx] != e11)
				return false;

	return true;
}


	//COLLAPSE-EDGE FUNCTION ADDED 4/12/04 
	//this checks for manifold property
	//returns true if it was able to collapse the edge, false if failed (and nothing is done)
	//expects a new vertex (required)
bool MeshStructure::CollapseEdge(Edge *edge, float newVtxPos[3])
{
	Vertex *v00 = 0, *v01 = 0, *v10 = 0, *v11 = 0;
	Triangle *t0 = 0, *t1 = 0, *nt00 = 0, *nt01 = 0, *nt10 = 0, *nt11 = 0;
	Edge *e00 = 0, *e01 = 0, *e10 = 0, *e11 = 0;

	//pointers to the edge's pointers that point to t0 and t1
	Triangle **pt00 = 0, **pt10 = 0;
	
	//pointers to nt01's and nt11's pointers to their edges adjacent to t0 and t1 respectively
	Edge **pe01 = 0, **pe11 = 0;

	//pointers to nt01's and nt11's pointers to vertices v01 and v00 respectively
	Vertex **pv01 = 0, **pv11 = 0;

	//for debugging only
	Edge *modEdge0, *modEdge1;

	//Initialize local surface pointers
	v00 = (Vertex *)edge->GetVertex(0);
	v01 = (Vertex *)edge->GetVertex(1);

	//for debug purposes (should always find an edge)
	if (edge != GetEdge(v00, v01))
		return false;
	
	t0 = (Triangle *)edge->GetTriangle(0);
	t1 = (Triangle *)edge->GetTriangle(1);

	//MANIFOLD TEST (1-ring intersection test)=================================================

    //You must first test to see if this is a valid collapse before you proceed with the
	//rest of this code - write a test to see if the surface will be a manifold after the
	//edge collpase (as discussed in class) - return false here if the edge cannot be
	//collapsed - otherwise proceed...

	//=========================================================================================

	if (t0)
		for (register int ndx = 0; ndx < 3; ++ndx)
			if (t0->vertex[ndx] != v00 && t0->vertex[ndx] != v01)
				v10 = t0->vertex[ndx];
	if (t1)
		for (register int ndx = 0; ndx < 3; ++ndx)
			if (t1->vertex[ndx] != v00 && t1->vertex[ndx] != v01)
				v11 = t1->vertex[ndx];
	if (t0)
	{
		e00 = GetEdge(v01, v10);
		e01 = GetEdge(v00, v10);
	}

	if (t1)
	{
		e10 = GetEdge(v01, v11);
		e11 = GetEdge(v00, v11);
	}
	
	if (t0)
	{
		nt00 = e00->triangle[0] != t0 ? (pt00 = &(modEdge0 = e00)->triangle[1], e00->triangle[0])
		 : (pt00 = &(modEdge0 = e00)->triangle[0], e00->triangle[1]);
		nt01 = e01->triangle[0] != t0 ? e01->triangle[0] : e01->triangle[1];

		if (nt01)
			for (register int ndx = 0; ndx < 3; ++ndx)
				if (nt01->edge[ndx] == e01)
					pe01 = &nt01->edge[ndx];

		//partial manifold test
		if ((!nt00 && !nt01) || nt00 == nt01)
			return false;
	}

	if (t1)
	{
		nt10 = e10->triangle[0] != t1 ? (pt10 = &(modEdge1 = e10)->triangle[1], e10->triangle[0])
		 : (pt10 = &(modEdge1 = e10)->triangle[0], e10->triangle[1]);
		nt11 = e11->triangle[0] != t1 ? e11->triangle[0] : e11->triangle[1];

		if (nt11)
			for (register int ndx = 0; ndx < 3; ++ndx)
				if (nt11->edge[ndx] == e11)
					pe11 = &nt11->edge[ndx];

		//partial manifold test
		if ((!nt10 && !nt11) || nt10 == nt11)
			return false;
	}

	//MANIFOLD TEST (Shark-fin test)===========================================================
	if (nt00)
		for (register int ndx = 0; ndx < 3; ++ndx)
			if (nt00->GetEdge(ndx)->GetOtherTriangle(nt00) == nt01)
				return false;

	if (nt10)
		for (register int ndx = 0; ndx < 3; ++ndx)
			if (nt10->GetEdge(ndx)->GetOtherTriangle(nt10) == nt11)
				return false;

	//=========================================================================================

	//Perform the collapse
	if (t0)
	{
		//update e00's triangle pointer
		if (*pt00)
			--((*pt00)->refCount);
		(*pt00) = nt01;
		if (nt01)
			++(nt01->refCount);

		//update nt01's edge pointer
		if (nt01)
		{
			if (*pe01)
				--((*pe01)->refCount);
			(*pe01) = e00;
			if (e00)
				++(e00->refCount);
		}
	}

	if (t1)
	{
		//update e10's triangle pointer
		if (*pt10)
			--((*pt10)->refCount);
		(*pt10) = nt11;
		if (nt11)
			++(nt11->refCount);

		//update nt11's edge pointer
		if (nt11)
		{
			if (*pe11)
				--((*pe11)->refCount);
			(*pe11) = e10;
			if (e10)
				++(e10->refCount);
		}
	}

	//remove the collapsed edge from v00's adjacency list
	if (edge->triangle[0])
		--(edge->triangle[0]->refCount);
	if (edge->triangle[1])
		--(edge->triangle[1]->refCount);

	edge->triangle[0] = 0;
	edge->triangle[1] = 0;

	v00->adjacentEdgeList.remove(edge);
	v01->adjacentEdgeList.remove(edge);

	//remove the collapsed edge from the data structure
	edgeList.remove(edge);
	MapRemoveEdge(edge);
	delete edge;

	//remove triangles and edges from data structure
	if (t0)
	{
		if (e01->triangle[0])
			--(e01->triangle[0]->refCount);
		if (e01->triangle[1])
			--(e01->triangle[1]->refCount);

		e01->triangle[0] = 0;
		e01->triangle[1] = 0;

		v00->adjacentTriangleList.remove(t0);
		v01->adjacentTriangleList.remove(t0);
		v10->adjacentTriangleList.remove(t0);

		if (e01->triangle[0])
			--(e01->triangle[0]->refCount);
		if (e01->triangle[1])
			--(e01->triangle[1]->refCount);
		v00->adjacentEdgeList.remove(e01);
		v10->adjacentEdgeList.remove(e01);

		if (t0->edge[0] && t0->edge[0] != edge)
			--(t0->edge[0]->refCount);
		if (t0->edge[1] && t0->edge[1] != edge)
			--(t0->edge[1]->refCount);
		if (t0->edge[2] && t0->edge[2] != edge)
			--(t0->edge[2]->refCount);

		edgeList.remove(e01);
		MapRemoveEdge(e01);
		e01->triangle[0] = e01->triangle[1] = 0;
		delete e01;
		triangleList.remove(t0);
		delete t0;
	}

	if (t1)
	{
		if (e11->triangle[0])
			--(e11->triangle[0]->refCount);
		if (e11->triangle[1])
			--(e11->triangle[1]->refCount);

		e11->triangle[0] = 0;
		e11->triangle[1] = 0;

		v00->adjacentTriangleList.remove(t1);
		v01->adjacentTriangleList.remove(t1);
		v11->adjacentTriangleList.remove(t1);

		if (e11->triangle[0])
			--(e11->triangle[0]->refCount);
		if (e11->triangle[1])
			--(e11->triangle[1]->refCount);
		v00->adjacentEdgeList.remove(e11);
		v11->adjacentEdgeList.remove(e11);

		if (t1->edge[0] && t1->edge[0] != edge)
			--(t1->edge[0]->refCount);
		if (t1->edge[1] && t1->edge[1] != edge)
			--(t1->edge[1]->refCount);
		if (t1->edge[2] && t1->edge[2] != edge)
			--(t1->edge[2]->refCount);

		edgeList.remove(e11);
		MapRemoveEdge(e11);
		e11->triangle[0] = e11->triangle[1] = 0;
		delete e11;
		triangleList.remove(t1);
		delete t1;
	}

	//update triangle's vertex pointers
	for (std::list<Triangle *>::iterator iter = v00->adjacentTriangleList.begin();
	 iter != v00->adjacentTriangleList.end(); ++iter)
		for (register int ndx = 0; ndx < 3; ++ndx)
			if ((*iter)->vertex[ndx] == v00)
				(*iter)->vertex[ndx] = v01;

	//update edge's vertex pointers
	for (std::list<Edge *>::iterator iter = v00->adjacentEdgeList.begin();
	 iter != v00->adjacentEdgeList.end(); ++iter)
		if ((*iter)->vertex[0] == v00)
			MapUpdateEdge(*iter, v01, (*iter)->vertex[1]);
		else if ((*iter)->vertex[1] == v00)
			MapUpdateEdge(*iter, (*iter)->vertex[0], v01);

	//splice the removed vertex's edge and vertex lists into v01's lists
	v01->adjacentEdgeList.splice(v01->adjacentEdgeList.end(), v00->adjacentEdgeList);
	v01->adjacentTriangleList.splice(v01->adjacentTriangleList.end(), v00->adjacentTriangleList);

	//remove vertex v00
	vertexList.remove(v00);
	delete v00;
	--numVertices;

	//finally, set the new position of the vertex
	v01->pos[0] = newVtxPos[0];
	v01->pos[1] = newVtxPos[1];
	v01->pos[2] = newVtxPos[2];

	return true;
}

MeshStructure::Edge *MeshStructure::GetEdge(Vertex *vtx0, Vertex *vtx1) const
{
	bool edgeInList = false;

	std::map<MapKey, Edge *>::const_iterator edgeIter 
	 = edgeMap.find(MapKey(vtx0, vtx1));

	edgeInList = edgeIter != edgeMap.end();

	if (!edgeInList)
	{
		edgeIter = edgeMap.find(MapKey(vtx1, vtx0));
		edgeInList = edgeIter != edgeMap.end();
	}

	return edgeInList ? edgeIter->second : 0;
}

void MeshStructure::GetAABB(float min[3], float max[3]) const
{
	for (int ndx = 0; ndx < 3; ++ndx) min[ndx] = minVtx[ndx], max[ndx] = maxVtx[ndx];
}

int MeshStructure::GetVertexCount() const
{
	return int(vertexList.size());
}

int MeshStructure::GetEdgeCount() const
{
	return int(edgeList.size());
}

int MeshStructure::GetTriangleCount() const
{
	return int(triangleList.size());
}

MeshStructure::MeshStructure()
{
	centerPos[0] = centerPos[1] = centerPos[2] = 0.0f;
	numVertices = 0;
}

MeshStructure::~MeshStructure()
{
	for (std::list<Triangle *>::iterator triIter = triangleList.begin();
	 triIter != triangleList.end(); ++triIter)
		delete *triIter;

	triangleList.clear();

	for (std::list<Vertex *>::iterator vtxIter = vertexList.begin();
	 vtxIter != vertexList.end(); ++vtxIter)
		delete *vtxIter;

	vertexList.clear();

	for (std::list<Edge *>::iterator edgeIter = edgeList.begin();
	 edgeIter != edgeList.end(); ++edgeIter)
		delete *edgeIter;

	edgeList.clear();

	edgeMap.clear();
	vertexMap.clear();
}

void MeshStructure::AddVertex(float x, float y, float z, int vtxID)
{
	Vertex *vtx = new Vertex(x, y, z, vtxID);
	vertexList.push_back(vtx);
	vertexMap.insert(std::pair<int, Vertex *> (vtxID, vtx));

	numVertices++;
			
	centerPos[0] = ((float(numVertices - 1) / float(numVertices)) * centerPos[0]) 
	 + ((1.0f / float(numVertices)) * x);
	centerPos[1] = ((float(numVertices - 1) / float(numVertices)) * centerPos[1]) 
	 + ((1.0f / float(numVertices)) * y);
	centerPos[2] = ((float(numVertices - 1) / float(numVertices)) * centerPos[2]) 
	 + ((1.0f / float(numVertices)) * z);

	PUSH_BOUNDS(maxVtx[0], minVtx[0], x);
	PUSH_BOUNDS(maxVtx[1], minVtx[1], y);
	PUSH_BOUNDS(maxVtx[2], minVtx[2], z);
}

void MeshStructure::AddVertex(float x, float y, float z, int vtxID, float n1, float n2, float n3, float t1, float t2, 
							  float tg1, float tg2, float tg3, float bn1, float bn2, float bn3)
{
	Vertex *vtx = new Vertex(x, y, z, vtxID);
	
	vtx->normal[0] = n1;
	vtx->normal[1] = n2;
	vtx->normal[2] = n3;

	vtx->texcoord[0] = t1;
	vtx->texcoord[1] = t2;

	vtx->tangent[0] = tg1;
	vtx->tangent[1] = tg2;
	vtx->tangent[2] = tg3;

	vtx->binormal[0] = bn1;
	vtx->binormal[1] = bn2;
	vtx->binormal[2] = bn3;



	vertexList.push_back(vtx);
	vertexMap.insert(std::pair<int, Vertex *> (vtxID, vtx));

	numVertices++;
			
	centerPos[0] = ((float(numVertices - 1) / float(numVertices)) * centerPos[0]) 
	 + ((1.0f / float(numVertices)) * x);
	centerPos[1] = ((float(numVertices - 1) / float(numVertices)) * centerPos[1]) 
	 + ((1.0f / float(numVertices)) * y);
	centerPos[2] = ((float(numVertices - 1) / float(numVertices)) * centerPos[2]) 
	 + ((1.0f / float(numVertices)) * z);

	PUSH_BOUNDS(maxVtx[0], minVtx[0], x);
	PUSH_BOUNDS(maxVtx[1], minVtx[1], y);
	PUSH_BOUNDS(maxVtx[2], minVtx[2], z);
}

void MeshStructure::AddTriangle(int vtxID0, int vtxID1, int vtxID2)
{
	float norm[3];

	Vertex *vertexArray[3];
	std::map<int, Vertex *>::iterator vertexMapIter;

	vertexMapIter = vertexMap.find(vtxID0);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexArray[0] = (Vertex *)vertexMapIter->second;
	

	vertexMapIter = vertexMap.find(vtxID1);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexArray[1] = (Vertex *)vertexMapIter->second;

	vertexMapIter = vertexMap.find(vtxID2);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexArray[2] = (Vertex *)vertexMapIter->second;

	vertexArray[0]->vtxUsed = true;
	vertexArray[1]->vtxUsed = true;
	vertexArray[2]->vtxUsed = true;
	
    Triangle *triangle = new Triangle(vertexArray);
	triangleList.push_back(triangle);

	int vtxNdx[3][2] = {{0,1},{1,2},{2,0}};

	for (int ndx = 0; ndx < 3; ++ndx)
	{
		triangle->edge[ndx] = GetEdge(vertexArray[vtxNdx[ndx][0]], 
			vertexArray[vtxNdx[ndx][1]]);
		if (!triangle->edge[ndx])
		{
			triangle->edge[ndx] = AddEdge(vertexArray[vtxNdx[ndx][0]], 
				vertexArray[vtxNdx[ndx][1]]);
			triangle->edge[ndx]->triangle[0] = triangle;
		}
		else
		{
			triangle->edge[ndx]->triangle[1] = triangle;
		}
		++(triangle->refCount);
		++(triangle->edge[ndx]->refCount);
	}

	ComputeFaceNormal(vertexArray[0]->pos, vertexArray[1]->pos, vertexArray[2]->pos, norm);

	triangle->vertex[0]->adjacentTriangleList.push_back(triangle);
	triangle->vertex[1]->adjacentTriangleList.push_back(triangle);
	triangle->vertex[2]->adjacentTriangleList.push_back(triangle);
	triangle->normal[0] = norm[0];
	triangle->normal[1] = norm[1];
	triangle->normal[2] = norm[2];
}

void MeshStructure::AddTriangle(int vtxID0, int vtxID1, int vtxID2,
								float normX, float normY, float normZ)
{
	Vertex *vertexArray[3];
	std::map<int, Vertex *>::iterator vertexMapIter;

	vertexMapIter = vertexMap.find(vtxID0);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexArray[0] = (Vertex *)vertexMapIter->second;
	

	vertexMapIter = vertexMap.find(vtxID1);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexArray[1] = (Vertex *)vertexMapIter->second;

	vertexMapIter = vertexMap.find(vtxID2);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexArray[2] = (Vertex *)vertexMapIter->second;

	vertexArray[0]->vtxUsed = true;
	vertexArray[1]->vtxUsed = true;
	vertexArray[2]->vtxUsed = true;
	
    Triangle *triangle = new Triangle(vertexArray);
	triangleList.push_back(triangle);

	int vtxNdx[3][2] = {{0,1},{1,2},{2,0}};

	for (int ndx = 0; ndx < 3; ++ndx)
	{
		triangle->edge[ndx] = GetEdge(vertexArray[vtxNdx[ndx][0]], 
			vertexArray[vtxNdx[ndx][1]]);
		if (!triangle->edge[ndx])
		{
			triangle->edge[ndx] = AddEdge(vertexArray[vtxNdx[ndx][0]], 
				vertexArray[vtxNdx[ndx][1]]);
			triangle->edge[ndx]->triangle[0] = triangle;
		}
		else
		{
			triangle->edge[ndx]->triangle[1] = triangle;
		}
		++(triangle->refCount);
		++(triangle->edge[ndx]->refCount);
	}

	triangle->vertex[0]->adjacentTriangleList.push_back(triangle);
	triangle->vertex[1]->adjacentTriangleList.push_back(triangle);
	triangle->vertex[2]->adjacentTriangleList.push_back(triangle);
	triangle->normal[0] = normX;
	triangle->normal[1] = normY;
	triangle->normal[2] = normZ;
}

void MeshStructure::AddTriangle(int vtxID0, int vtxID1, int vtxID2,
								int nmlID0, int nmlID1, int nmlID2,
								int texID0, int texID1, int texID2)
{
	Vertex *vertexArray[3];
	std::map<int, Vertex *>::iterator vertexMapIter;

	vertexMapIter = vertexMap.find(vtxID0);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexArray[0] = (Vertex *)vertexMapIter->second;
	

	vertexMapIter = vertexMap.find(vtxID1);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexArray[1] = (Vertex *)vertexMapIter->second;

	vertexMapIter = vertexMap.find(vtxID2);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexArray[2] = (Vertex *)vertexMapIter->second;

	vertexArray[0]->vtxUsed = true;
	vertexArray[1]->vtxUsed = true;
	vertexArray[2]->vtxUsed = true;
	
    Triangle *triangle = new Triangle(vertexArray);
	triangleList.push_back(triangle);

	//put the texture coordinates in the triangle
	vertexMapIter = vertexMap.find(texID0);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexMapIter->second->GetTextureCoordinates(triangle->texcoord[0]);

	vertexMapIter = vertexMap.find(texID1);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexMapIter->second->GetTextureCoordinates(triangle->texcoord[1]);

	vertexMapIter = vertexMap.find(texID2);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexMapIter->second->GetTextureCoordinates(triangle->texcoord[2]);

	//put the normals in the triangle
	vertexMapIter = vertexMap.find(nmlID0);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexMapIter->second->GetVertexNormal(triangle->normals[0]);
	vertexMapIter->second->GetVertexBinormal(triangle->binormals[0]);
	vertexMapIter->second->GetVertexTangent(triangle->tangents[0]);


	vertexMapIter = vertexMap.find(nmlID1);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexMapIter->second->GetVertexNormal(triangle->normals[1]);
	vertexMapIter->second->GetVertexBinormal(triangle->binormals[1]);
	vertexMapIter->second->GetVertexTangent(triangle->tangents[1]);

	vertexMapIter = vertexMap.find(nmlID2);
	if (vertexMapIter == vertexMap.end())
		return;
	vertexMapIter->second->GetVertexNormal(triangle->normals[2]);
	vertexMapIter->second->GetVertexBinormal(triangle->binormals[2]);
	vertexMapIter->second->GetVertexTangent(triangle->tangents[2]);

	//printf("%g %g %g\n", triangle->tangents[0][0], triangle->tangents[0][1], triangle->tangents[0][2]);

	



	int vtxNdx[3][2] = {{0,1},{1,2},{2,0}};

	for (int ndx = 0; ndx < 3; ++ndx)
	{
		triangle->edge[ndx] = GetEdge(vertexArray[vtxNdx[ndx][0]], 
			vertexArray[vtxNdx[ndx][1]]);
		if (!triangle->edge[ndx])
		{
			triangle->edge[ndx] = AddEdge(vertexArray[vtxNdx[ndx][0]], 
				vertexArray[vtxNdx[ndx][1]]);
			triangle->edge[ndx]->triangle[0] = triangle;
		}
		else
		{
			triangle->edge[ndx]->triangle[1] = triangle;
		}
		++(triangle->refCount);
		++(triangle->edge[ndx]->refCount);
	}

	triangle->vertex[0]->adjacentTriangleList.push_back(triangle);
	triangle->vertex[1]->adjacentTriangleList.push_back(triangle);
	triangle->vertex[2]->adjacentTriangleList.push_back(triangle);
	//triangle->normal[0] = normX;
	//triangle->normal[1] = normY;
	//triangle->normal[2] = normZ;

}

std::list<MeshStructure::Triangle *>::iterator MeshStructure::TriangleIterBegin()
{
	return triangleList.begin();
}

std::list<MeshStructure::Triangle *>::iterator MeshStructure::TriangleIterEnd()
{
	return triangleList.end();
}

std::list<MeshStructure::Vertex *>::iterator MeshStructure::VertexIterBegin()
{
	return vertexList.begin();
}

std::list<MeshStructure::Vertex *>::iterator MeshStructure::VertexIterEnd()
{
	return vertexList.end();
}

std::list<MeshStructure::Edge *>::iterator MeshStructure::EdgeIterBegin()
{
	return edgeList.begin();
}

std::list<MeshStructure::Edge *>::iterator MeshStructure::EdgeIterEnd()
{
	return edgeList.end();
}

MeshStructure::Edge *MeshStructure::AddEdge(Vertex *vtx0, Vertex *vtx1)
{
	Edge *edge = new Edge(vtx0, vtx1);
	edge->triangle[0] = edge->triangle[1] = 0;
	edgeList.push_back(edge);
	edgeMap.insert(std::pair<MapKey, Edge *>(MapKey(vtx0, vtx1), edge));
	vtx0->adjacentEdgeList.push_back(edge);
	vtx1->adjacentEdgeList.push_back(edge);
	return edge;
}

void MeshStructure::MapRemoveEdge(Edge *edge)
{
	Vertex *vtx0 = edge->vertex[0], *vtx1 = edge->vertex[1];
	MapKey mk0(vtx0, vtx1);
	MapKey mk1(vtx1, vtx0);

	edgeMap.erase(mk0);
	edgeMap.erase(mk1);
}

void MeshStructure::MapUpdateEdge(Edge *edge, Vertex *upd_vtx0, Vertex *upd_vtx1)
{
	MapRemoveEdge(edge);

	edge->vertex[0] = upd_vtx0;
	edge->vertex[1] = upd_vtx1;

	edgeMap.insert(std::pair<MapKey, Edge *>(MapKey(upd_vtx0, upd_vtx1), edge));
}

void MeshStructure::ReferenceOutput()
{
	std::ofstream logStream;
	logStream.open("ref_log.dat", std::ios::out);
	Edge *edge;
	Triangle *tri;
	int count = 0;

	logStream << "EDGES" << std::endl;
	for (std::list<Edge *>::iterator edgeIter = edgeList.begin();
	 edgeIter != edgeList.end(); ++edgeIter)
	{
		edge = *edgeIter;
		if (!CheckEdge(edge))
			logStream << "Discontinuity 0" << std::endl;

		logStream << "Edge refCount = " << (*edgeIter)->refCount << std::endl;
		if ((*edgeIter)->refCount > 2)
			logStream << "Discontinuity 1" << std::endl;

		if (edge->triangle[0])
		{
			count = 0;
			for (register int ndx = 0; ndx < 3; ++ndx)
				if (edge->triangle[0]->edge[ndx] == edge)
					++count;

			if (count != 1)
				logStream << "Discontinuity 2" << std::endl;
		}
		if (edge->triangle[1])
		{
			count = 0;
			for (register int ndx = 0; ndx < 3; ++ndx)
				if (edge->triangle[1]->edge[ndx] == edge)
					++count;

			if (count != 1)
				logStream << "Discontinuity 3" << std::endl;
		}
	}

	logStream << std::endl << "TRIANGLES" << std::endl;
	for (std::list<Triangle *>::iterator triIter = triangleList.begin();
	 triIter != triangleList.end(); ++triIter)
	{
		tri = *triIter;
		logStream << "Triangle refCount = " << tri->refCount << std::endl;
		if (tri->refCount > 3)
			logStream << "Discontinuity 4" << std::endl;

		for (register int ndx = 0; ndx < 3; ++ndx)
		{
			count = 0;
			if (tri->edge[ndx]->triangle[0] == tri)
				++count;
			if (tri->edge[ndx]->triangle[1] == tri)
				++count;

			if (count != 1)
				logStream << "Discontinuity 5" << std::endl;
		}
	}

	logStream.close();
}
