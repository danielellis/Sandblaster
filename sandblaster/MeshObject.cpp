//MeshObject.cpp

//Created/Programmed by Jonathan DeKlotz
// 5/10/04 minor changes ZJW


#include  "MeshObject.h"

using namespace std;

/*
int linenum1 = 1;
int linenum2 = 1;
*/

inline void MeshObject_MakeAABB(float v0[3], float v1[3], float v2[3], float v3[3], float v4[3],
								float v5[3], float v6[3], float v7[3], float min[3], float max[3])
{
	v0[0] = min[0]; v1[0] = min[0]; v2[0] = max[0]; v3[0] = max[0];
	v0[1] = min[1]; v1[1] = min[1]; v2[1] = min[1]; v3[1] = min[1];
	v0[2] = min[2]; v1[2] = max[2]; v2[2] = max[2]; v3[2] = min[2];
	v4[0] = min[0]; v5[0] = min[0]; v6[0] = max[0]; v7[0] = max[0];
	v4[1] = max[1]; v5[1] = max[1]; v6[1] = max[1]; v7[1] = max[1];
	v4[2] = min[2]; v5[2] = max[2]; v6[2] = max[2]; v7[2] = min[2];
}

MeshObject::MeshObject()
{
	mesh = 0;
}

MeshObject::MeshObject(std::string meshFileName)
{
	mesh = 0;
	LoadMeshFromFile(meshFileName);
}

MeshObject::~MeshObject()
{
	mesh ? delete mesh : 0;
}

void MeshObject::LoadMeshFromFile(std::string meshFileName)
{
	mesh ? delete mesh : 0;
	mesh = new MeshStructure;
	mVertexID = 1;

	std::ifstream meshStream(meshFileName.c_str());

	if (!meshStream)
	{
		std::cout << "Error opening file" << meshFileName;
		meshStream.close();
		return;
	}

	LoadMeshFromStream(meshStream);
	meshStream.close();
}

void MeshObject::LoadMeshFromStream(std::istream &meshStream)
{
	char str[256];
	while (meshStream) {
		meshStream >> std::ws;
		meshStream.get(str,sizeof(str));
		if (!meshStream) break;
		meshStream.ignore(9999,'\n');
		readLine(str, meshStream);
	}
	mesh->CenterMesh();
	//mesh->NormalizeMeshScale(12.0f);
	mesh->ComputeVertexNormals();
	mesh->GetAABB(minVtx, maxVtx);
	MeshObject_MakeAABB(AABB_v0, AABB_v1, AABB_v2, AABB_v3, 
		AABB_v4, AABB_v5, AABB_v6, AABB_v7, minVtx, maxVtx);
}

void MeshObject::Scale(float s) {
	mesh->Scale(s);
}

void MeshObject::GLDraw()
{
	if (!mesh)
		return;

	std::list<MeshStructure::Triangle *>::iterator triIter = mesh->TriangleIterBegin();
	std::list<MeshStructure::Triangle *>::iterator triIterEnd = mesh->TriangleIterEnd();
	MeshStructure::Triangle *tri = 0;
	float v0[3], v1[3], v2[3];//, n0[3], n1[3], n2[3];

	/*
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID1);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID2);
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glActiveTextureARB(GL_TEXTURE3_ARB);
	glActiveTextureARB(GL_TEXTURE4_ARB);
	*/


	//ZJW draw as solid
	int draw_mode = 0;
	if(draw_mode == 0 ) {
		glBegin(GL_TRIANGLES);
		for (;triIter != triIterEnd; ++triIter)
		{
			//cout << "drawing a tri " << endl;
			tri = *triIter;

			//get the normal and vertices

			tri->GetVertex(0)->GetVertexLocation(v0);
			tri->GetVertex(1)->GetVertexLocation(v1);
			tri->GetVertex(2)->GetVertexLocation(v2);


			/*
			tri->GetVertex(0)->GetTextureCoordinates(t0);
			tri->GetVertex(1)->GetTextureCoordinates(t1);
			tri->GetVertex(2)->GetTextureCoordinates(t2);
			*/

			//printf("%f %f\n", t1[0], t1[1]);

			//pass to opengl
			//glColor3f(1.0, 0.0, 0.0);

			float interpTexCoords[3][3];


			//interpolate texture coordinates
			interpTexCoords[0][0] = (v0[0] - minVtx[0])/(maxVtx[0] - minVtx[0]);
			interpTexCoords[0][1] = (v0[1] - minVtx[1])/(maxVtx[1] - minVtx[1]);
			interpTexCoords[0][2] = (v0[2] - minVtx[2])/(maxVtx[2] - minVtx[2]);

			//interpolate texture coordinates
			interpTexCoords[1][0] = (v1[0] - minVtx[0])/(maxVtx[0] - minVtx[0]);
			interpTexCoords[1][1] = (v1[1] - minVtx[1])/(maxVtx[1] - minVtx[1]);
			interpTexCoords[1][2] = (v1[2] - minVtx[2])/(maxVtx[2] - minVtx[2]);

			//interpolate texture coordinates
			interpTexCoords[2][0] = (v2[0] - minVtx[0])/(maxVtx[0] - minVtx[0]);
			interpTexCoords[2][1] = (v2[1] - minVtx[1])/(maxVtx[1] - minVtx[1]);
			interpTexCoords[2][2] = (v2[2] - minVtx[2])/(maxVtx[2] - minVtx[2]);


			//printf("%g %g\n", interpTexCoords[2][0], interpTexCoords[2][1]);



			glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, tri->texcoord[0]);
			glMultiTexCoord3fvARB(GL_TEXTURE1_ARB, interpTexCoords[0]);


			glMultiTexCoord3fvARB(GL_TEXTURE2_ARB, tri->normals[0]);
			glMultiTexCoord3fvARB(GL_TEXTURE3_ARB, tri->binormals[0]);
			glMultiTexCoord3fvARB(GL_TEXTURE4_ARB, tri->tangents[0]);

			glVertex3fv(v0);


			glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, tri->texcoord[1]);
			glMultiTexCoord3fvARB(GL_TEXTURE1_ARB, interpTexCoords[1]);

			glMultiTexCoord3fvARB(GL_TEXTURE2_ARB, tri->normals[1]);
			glMultiTexCoord3fvARB(GL_TEXTURE3_ARB, tri->binormals[1]);
			glMultiTexCoord3fvARB(GL_TEXTURE4_ARB, tri->tangents[1]);

			glVertex3fv(v1);


			glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, tri->texcoord[2]);
			glMultiTexCoord3fvARB(GL_TEXTURE1_ARB, interpTexCoords[2]);

			glMultiTexCoord3fvARB(GL_TEXTURE2_ARB, tri->normals[2]);
			glMultiTexCoord3fvARB(GL_TEXTURE3_ARB, tri->binormals[2]);
			glMultiTexCoord3fvARB(GL_TEXTURE4_ARB, tri->tangents[2]);

			glVertex3fv(v2);


			/*
			//glTexCoord2fv(t0);
			glTexCoord3fv(tri->tangents[0]);
			glTexCoord3fv(tri->binormals[0]);
			glTexCoord2fv(interpTexCoords[0]);
			glTexCoord2fv(tri->texcoord[0]);
			glNormal3fv(tri->normals[0]);
			glVertex3fv(v0);



			//glTexCoord2fv(t1);
			glTexCoord3fv(tri->tangents[1]);
			glTexCoord3fv(tri->binormals[1]);
			glTexCoord2fv(interpTexCoords[1]);
			glTexCoord2fv(tri->texcoord[1]);
			glNormal3fv(tri->normals[1]);
			glVertex3fv(v1);


			//glTexCoord2fv(t2);
			glTexCoord3fv(tri->tangents[2]);
			glTexCoord3fv(tri->binormals[2]);
			glTexCoord2fv(interpTexCoords[2]);
			glTexCoord2fv(tri->texcoord[2]);
			glNormal3fv(tri->normals[2]);
			glVertex3fv(v2);
			*/


		}
		glEnd();
	} else {
		//ZJW: just draw edges
		for (;triIter != triIterEnd; ++triIter)
		{
			tri = *triIter;

			//get the normal and vertices
			tri->GetVertex(0)->GetVertexLocation(v0);
			tri->GetVertex(1)->GetVertexLocation(v1);
			tri->GetVertex(2)->GetVertexLocation(v2);
			glBegin(GL_LINE_LOOP);      
			//pass to opengl
			glVertex3fv(v0);
			glVertex3fv(v1);
			glVertex3fv(v2);
			glEnd();
		}
	}

}


void MeshObject::GLDrawAABB()
{
	if (!mesh)
		return;

	glShadeModel(GL_FLAT);
	glDisable(GL_LIGHTING);
	glColor3f(0.15f, 0.15f, 0.9f);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(AABB_v0);
	glVertex3fv(AABB_v1);
	glVertex3fv(AABB_v2);
	glVertex3fv(AABB_v3);
	glVertex3fv(AABB_v0);
	glVertex3fv(AABB_v4);
	glVertex3fv(AABB_v5);
	glVertex3fv(AABB_v6);
	glVertex3fv(AABB_v7);
	glVertex3fv(AABB_v4);
	glEnd();
	glBegin(GL_LINES);
	glVertex3fv(AABB_v5);
	glVertex3fv(AABB_v1);
	glVertex3fv(AABB_v6);
	glVertex3fv(AABB_v2);
	glVertex3fv(AABB_v7);
	glVertex3fv(AABB_v3);
	glEnd();
	glEnable(GL_LIGHTING);
}

void MeshObject::SmoothMesh() {
	//fill in....
}

float MeshObject::GetBottom()
{
	return minVtx[1];
}

void MeshObject::readLine(char *str, std::istream &meshStream)
{
	int indx = 0, vi = 0;
	float x = 0, y = 0, z = 0;

	//printf("reading line\n");

	//cout << "str[0] " << str[0] << endl;

	if (str[0]=='#')
		return;
	else if (str[0]=='v' && !strncmp(str,"v ",2)) 
	{

		//printf("reading line %d\n", linenum);
		sscanf(str, "v %g %g %g", &x, &y, &z);

		//printf("%d: %g %g %g\n", linenum1, x, y, z);


		//get the vertex normal
		char str2[256];
		meshStream >> std::ws;
		meshStream.get(str2,sizeof(str2));
		meshStream.ignore(9999,'\n');

		float n1, n2, n3;
		sscanf(str2, "vn %g %g %g", &n1, &n2, &n3);

		//get the vertex tanget
		meshStream >> std::ws;
		meshStream.get(str2,sizeof(str2));
		meshStream.ignore(9999,'\n');

		float tg1, tg2, tg3;
		sscanf(str2, "#_#tangent %g %g %g", &tg1, &tg2, &tg3);

		//get the binormal
		meshStream >> std::ws;
		meshStream.get(str2,sizeof(str2));
		meshStream.ignore(9999,'\n');

		float bn1, bn2, bn3;
		sscanf(str2, "#_#binormal %g %g %g", &bn1, &bn2, &bn3);

		//get the texture coordinates
		meshStream >> std::ws;
		meshStream.get(str2,sizeof(str2));
		meshStream.ignore(9999,'\n');

		float t1, t2;
		sscanf(str2, "vt %g %g ", &t1, &t2);

		//printf("%g %g \n", t1, t2);

		mesh->AddVertex(x, y, z, mVertexID, n1, n2, n3, t1, t2, tg1, tg2, tg3, bn1, bn2, bn3);

		mVertexID++;

	}


	else if (str[0]=='f' && !strncmp(str,"f ",2)) 
	{
		//printf("reading face\n");


		int id[3];
		int tex[3];
		int norm[3];

		int a = 0;
		int b = 1;
		int c = 2;
		// int tmp;

		sscanf(str, "f %d/%d/%d %d/%d/%d %d/%d/%d", id+a, tex+a, norm+a, id+b, tex+b, norm+b, id+c, tex+c, norm+c);
		//sscanf(str, "f %d %d %d", id, id+1, id+2);

		//printf("%g %g %g\n", mesh->GetNormalLocation(norm[0]), mesh->GetNormalLocation(norm[1]), mesh->GetNormalLocation(norm[2]));

		mesh->AddTriangle(id[0], id[1], id[2], norm[0], norm[1], norm[2], tex[0], tex[1], tex[2]);
	}
}

/*
void MeshObject::readLine2(char *str)
{
int indx = 0, vi = 0;
float x = 0, y = 0, z = 0;

//cout << "str[0] " << str[0] << endl;

if (str[0]=='#')
return;
else if (str[0]=='V' && !strncmp(str,"Vertex ",7)) 
{
sscanf(str, "Vertex %d %g %g %g", &vi, &x, &y, &z);
mesh->AddVertex(x, y, z, vi);
} 
else if (str[0]=='F' && !strncmp(str,"Face ",5)) 
{
int id[3];
int triID, ndx, value;
char *stringLoc = str + 4;
for (ndx = 0;; ndx++)
{
while (*stringLoc && isspace(*stringLoc)) stringLoc++;
if (!*stringLoc) break;
char *valueStr = stringLoc;
while (*stringLoc && isdigit(*stringLoc)) stringLoc++;
value = atoi(valueStr);
!ndx ? triID = value : id[ndx-1] = value;
}
mesh->AddTriangle(id[0], id[1], id[2]);
}
}
*/

void MeshObject::GetMinVtx(float vtx[3])
{
	for (int ndx = 0; ndx < 3; ++ndx) vtx[ndx] = minVtx[ndx];
}

void MeshObject::GetMaxVtx(float vtx[3])
{
	for (int ndx = 0; ndx < 3; ++ndx) vtx[ndx] = maxVtx[ndx];
}
