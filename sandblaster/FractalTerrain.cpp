#include "FractalTerrain.h"
#include "Materials.h"
#include "Driver.h"
#include "ProjectileWeapon.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

int numTris;

CGprofile FractalTerrain::vsFractalProfile, FractalTerrain::psFractalProfile;
CGprogram FractalTerrain::vsFractalProgram, FractalTerrain::psFractalProgram;
CGparameter FractalTerrain::psHeight, FractalTerrain::FractalModelView, 
	FractalTerrain::vsPosIN, FractalTerrain::terrainTexture, FractalTerrain::vsHeightMax,
	FractalTerrain::vsTextCoord;
void FractalTerrain::ChangeThresh(bool v) {if (v) mRenderThresh++; else if (mRenderThresh > 0) mRenderThresh--; cout << "Thresh: " << mRenderThresh << endl;}

FractalTerrain::FractalTerrain(Frustum *vf) : mVF(vf), mPlains(0), mMountains(0), mGameTerrain(0), 
    mLeftRoot(0), mRightRoot(0), mCurVariancePtr(0), mNextNode(-1), mRenderThresh(RENDER_THRESH), mRenderMode(0)
    {
    TextureLoader textureLoader;
	
        
        /*
    mMap1 = new float *[mMapSize + 1];
    mMap2 = new float *[mMapSize + 1];

    for (int i = 0; i < mMapSize + 1; i++) {
        mMap1[i] = new float[mMapSize + 1];
        mMap2[i] = new float[mMapSize + 1];
        for (int j = 0; j < mMapSize + 1; j++) {
            mMap1[i][j] = LOW;
            mMap2[i][j] = LOW;
        }
    }
    */

	textureLoader.LoadTextureFromDisk("terraintextures/sand.tga", &mTexture);
    GenerateMap();

    mLeftRoot = &mNodePool[0];
    mRightRoot = &mNodePool[1];

    mCurVariancePtr = mLeftVariance;
    ComputeVariance(0, 0, mGameTerrain[0][0], MAP_SIZE, MAP_SIZE, mGameTerrain[MAP_SIZE][MAP_SIZE], 0, MAP_SIZE, mGameTerrain[0][MAP_SIZE], 1);
    mCurVariancePtr = mRightVariance;
    ComputeVariance(MAP_SIZE, MAP_SIZE, mGameTerrain[MAP_SIZE][MAP_SIZE], 0, 0, mGameTerrain[0][0], MAP_SIZE, 0, mGameTerrain[MAP_SIZE][0], 1);
}

FractalTerrain::~FractalTerrain() {
    /*
    for (int i = 0; i < mMapSize + 1; i++) {
        delete [] mMap1[i];
        delete [] mMap2[i];
    }

    delete [] mMap1;
    delete [] mMap2;
    */
}

void FractalTerrain::GenerateMap() {
    float **mMountains;
    //float Map2[MAP_SIZE+1][MAP_SIZE+1];
    int i, j;
    int mapRad = MAP_SIZE/2;
    float planeRad = 7.0f*mapRad/8.0f;
    float planeRad2 = planeRad*planeRad;

    mMountains = new float *[MAP_SIZE + 1];
    mPlains = new float *[MAP_SIZE + 1];
    mGameTerrain = new float*[MAP_SIZE + 1];
    for (int i = 0; i < MAP_SIZE + 1; i++) {
        mMountains[i] = new float[MAP_SIZE + 1];
        mPlains[i] = new float[MAP_SIZE + 1];
        mGameTerrain[i] = new float[MAP_SIZE + 1];
        for (int j = 0; j < MAP_SIZE + 1; j++) {
            mMountains[i][j] = LOW;
            mPlains[i][j] = LOW;
        }
    }


   //mMountains - Mountains
   for (int n = 0; n < MAP_SIZE + 1; n++) {
      for (int m = 0; m < MAP_SIZE + 1; m++) {
         mMountains[n][m] = LOW;
      }
   }

   for (int u = 0; u < MAP_SIZE + 1; u++) {
      for (int v = 0; v < MAP_SIZE + 1; v++) {
         if ((u-MAP_SIZE/2)*(u-MAP_SIZE/2) + (v-MAP_SIZE/2)*(v-MAP_SIZE/2) < planeRad2) {
            mMountains[u][v] = 0;
         }
      }
   }

   for (int k = 0; k < MAP_SIZE + 1; k += MAP_SIZE/4) {
      for (int l = 0; l < MAP_SIZE + 1; l += ((k == 0 || k == MAP_SIZE) ? MAP_SIZE/4 : MAP_SIZE)) {
         mMountains[k][l] = 25;
      }
   }

   DiamondSquare(mMountains, 20);

   //Map2 - Plain
   for (i = 0; i < MAP_SIZE + 1; i++) {
      for (int j = 0; j < MAP_SIZE + 1; j++) {
         mPlains[i][j] = LOW;
      }
   }

   mPlains[0][0] = mPlains[MAP_SIZE][0] = mPlains[0][MAP_SIZE] = mPlains[MAP_SIZE][MAP_SIZE] = 0;

   DiamondSquare(mPlains, 5);

   mMaxHeight = 0;
   for (i = 0; i < MAP_SIZE + 1; i++) {
		for (j = 0; j < MAP_SIZE + 1; j++) {
			if (mMountains[i][j] != 0) {
				mGameTerrain[i][j] = mMountains[i][j];
				if (mGameTerrain[i][j] > mMaxHeight)
					mMaxHeight = mGameTerrain[i][j];
			}
			else {
				mGameTerrain[i][j] = mPlains[i][j];
			}
		}
   }

   /*take out the +1 in the i loop test condition
    and use this code
    for (j = 0; j < MAP_SIZE + 1; j++) {
        mGameTerrain[MAP_SIZE][j] = mGameTerrain[MAP_SIZE -1][j];
    }
    */
}

void FractalTerrain::DiamondSquare(float **Map, int arg) {
   int boxlen = MAP_SIZE, c1, c2, c3, c4;

   float range = 1.0f, h = 1.0f;

   while (boxlen > 1) {
      //perform diamond step
      for (int i = 0; i < MAP_SIZE; i += boxlen) {
         for (int j = 0; j < MAP_SIZE; j += boxlen) {
            if (Map[i+boxlen/2][j+boxlen/2] == LOW) {
               Map[i+boxlen/2][j+boxlen/2] = 0.25f * 
                  (Map[i][j]
                  + Map[i][j+boxlen]
                  + Map[i+boxlen][j+boxlen]
                  + Map[i+boxlen][j]) + arg * (1 - rand() % 3)/range;
            }
         }
      }

      //perform box step
      int k = 0;
      for (int u = 0; u < MAP_SIZE + 1; u += boxlen/2, k++) {
         for (int v = (((k % 2) == 0) ? boxlen/2 : 0); v < MAP_SIZE + 1; v += boxlen) {
            if (Map[u][v] == LOW) {
               c1 = v + boxlen/2;
               if (c1 > MAP_SIZE)
                  c1 = v + boxlen/2 - MAP_SIZE;
               c2 = v - boxlen/2;
               if (c2 < 0)
                  c2 = MAP_SIZE + (v - boxlen/2);
               c3 = u + boxlen/2;
               if (c3 > MAP_SIZE)
                  c3 = u + boxlen/2 - MAP_SIZE;
               c4 = u - boxlen/2;
               if (c4 < 0)
                  c4 = MAP_SIZE + (u - boxlen/2);

               Map[u][v] = 0.25f * (Map[u][c1]
                                 + Map[u][c2]
                                 + Map[c3][v]
                                 + Map[c4][v])  + arg * (1 - rand() % 3)/range;
            }
         }
      }

      //decrease range
      boxlen /= 2;
      range *= pow(2, h);
   }
}

bool FractalTerrain::IsCollided(const Driver *driver, Vector3 &normalOut) const {
    Vector3 separation;
    float myrad = 7.0f*MAP_SIZE/16.0f;
    
    separation = driver->GetPos();
    separation.SetY(0);

    normalOut = -separation.Normalized();

    return separation.DotProduct(separation) > myrad * myrad;
}


bool FractalTerrain::IsCollided(const ProjectileWeapon *projectileWeapon) const {
    Vector3 separation;
    float myrad = 7.0f*MAP_SIZE/16.0f;
    
    separation = projectileWeapon->GetPos();

    return separation.DotProduct(separation) > myrad * myrad;
}

float FractalTerrain::GetBiLinIntHeight(float x, float z) const {
    /*
    int xPos = int(x + (MAP_SIZE >> 1));
    int zPos = int(z + (MAP_SIZE >> 1));
    float y11 = mGameTerrain[xPos][zPos];
    float y21 = mGameTerrain[xPos+1][zPos];
    float y12 = mGameTerrain[xPos][zPos+1];
    float y22 = mGameTerrain[xPos+1][zPos+1];
    float r1 = y11 + (x-xPos) * (y21 - y11);
    float r2 = y12 + (x-xPos) * (y22 - y12);

    return r1 + (z-zPos) * (r2 - r1);
    */
    float h = mGameTerrain[(int)(z+MAP_SIZE/2)][(int)(x+MAP_SIZE/2)];

    return h;
}

void FractalTerrain::Reset() {
    mNextNode = 2;

    mLeftRoot->BottomNeighbor = mRightRoot;
    mRightRoot->BottomNeighbor = mLeftRoot;

    mLeftRoot->LeftNeighbor = mLeftRoot->RightNeighbor = mLeftRoot->LeftChild = mLeftRoot->RightChild = 0;
    mRightRoot->LeftNeighbor = mRightRoot->RightNeighbor = mRightRoot->LeftChild = mRightRoot->RightChild = 0;

    //mRenderList.clear();
}

int FractalTerrain::RenderTerrain(const Vector3 eye) {
	float red[4] = {1,0,0,1}, green[4] = {0,1,0,1};
    float position[4] = {0,-30,0,0};
    float diffuse[4] = {1,1,1,1};
    //list<BTTNode *>::iterator itr;

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mTexture.TextureID);

    //glEnable(GL_BLEND);

	numTris = 0;
    Vector3 mapEye(MAP_SIZE*0.5, 0, MAP_SIZE*0.5);

    mapEye += eye;

    Reset();
    //mVF->Update();

    mCurVariancePtr = mLeftVariance;
    Tessellate(mLeftRoot, mapEye, 0, 0, MAP_SIZE, MAP_SIZE, 0, MAP_SIZE, 1);

    mCurVariancePtr = mRightVariance;
    Tessellate(mRightRoot, mapEye, MAP_SIZE, MAP_SIZE, 0, 0, MAP_SIZE, 0, 1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);


    glPushMatrix();
        glTranslatef(-MAP_SIZE*0.5f, 0, -MAP_SIZE*0.5f);

		glBegin(GL_TRIANGLES);

                //glColor3f(0,0,0);
                //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
                Render(mLeftRoot, 0, 0, MAP_SIZE, MAP_SIZE, 0, MAP_SIZE); 

                //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
                Render(mRightRoot, MAP_SIZE, MAP_SIZE, 0, 0, MAP_SIZE, 0);
            
            glEnd();

        /*
        if (mRenderMode == 0) {
	        glBegin(GL_TRIANGLES);

                //glColor3f(0,0,0);
                //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
                Render(mLeftRoot, 0, 0, MAP_SIZE, MAP_SIZE, 0, MAP_SIZE); 

                //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
                Render(mRightRoot, MAP_SIZE, MAP_SIZE, 0, 0, MAP_SIZE, 0);
            
            glEnd();
        }
        else {
        */
            /*
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_LIGHTING);
            glColor3f(0,0,0);
            glBegin(GL_TRIANGLES);

                //glColor3f(0,0,0);
                //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
                Render(mLeftRoot, 0, 0, MAP_SIZE, MAP_SIZE, 0, MAP_SIZE); 

                //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
                Render(mRightRoot, MAP_SIZE, MAP_SIZE, 0, 0, MAP_SIZE, 0);
            
            glEnd();

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            */
        //}
        

        /*
        glColor3f(1,0,0);
        glBegin(GL_POINTS);
        for (int i = 0; i < MAP_SIZE; i++) {
            for (int j = 0; j < MAP_SIZE; j++) {
                glVertex3f(i, mGameTerrain[i][j], j);
            }
        }
        glEnd();
        */

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    return numTris;
    //return size;
}

bool FractalTerrain::NodeVFIntersect(int lX, int lZ, int rX, int rZ, int aX, int aZ) {
	//bool outside;
    
	int vfndx[4] = {0,1,4,5};

    lX -= 0.5*MAP_SIZE;
    lZ -= 0.5*MAP_SIZE;
    rX -= 0.5*MAP_SIZE;
    rZ -= 0.5*MAP_SIZE;
    aX -= 0.5*MAP_SIZE;
    aZ -= 0.5*MAP_SIZE;

	for (int i = 0; i < 4; i++) {
		//outside = true;
        if ((mVF->Planes[vfndx[i]][0]*lX
				//+ mVF->Planes[vfndx[i]][1]*node->corners[j].y
				+ mVF->Planes[vfndx[i]][2]*lZ
				+ mVF->Planes[vfndx[i]][3] < 0) 
            && (mVF->Planes[vfndx[i]][0]*rX
				//+ mVF->Planes[vfndx[i]][1]*node->corners[j].y
				+ mVF->Planes[vfndx[i]][2]*rZ
				+ mVF->Planes[vfndx[i]][3] < 0)

            && (mVF->Planes[vfndx[i]][0]*aX
				//+ mVF->Planes[vfndx[i]][1]*node->corners[j].y
				+ mVF->Planes[vfndx[i]][2]*aZ
				+ mVF->Planes[vfndx[i]][3] < 0))
            return false;
        /*
		for (int j = 0; j < 3 && outside; j++) {
			outside = mVF->Planes[vfndx[i]][0]*node->corners[j].x
				//+ mVF->Planes[vfndx[i]][1]*node->corners[j].y
				+ mVF->Planes[vfndx[i]][2]*node->corners[j].z
				+ mVF->Planes[vfndx[i]][3] < 0;
		}
		if (outside)
			return false;
            */
	}

	return true;
}

/*
bool FractalTerrain::NodeVFIntersect(BTTNode *node) {
	bool outside;
	int vfndx[4] = {0,1,4,5};

	for (int i = 0; i < 4; i++) {
		outside = true;
		for (int j = 0; j < 3 && outside; j++) {
			outside = mVF->Planes[vfndx[i]][0]*node->corners[j].x
				+ mVF->Planes[vfndx[i]][1]*node->corners[j].y
				+ mVF->Planes[vfndx[i]][2]*node->corners[j].z
				+ mVF->Planes[vfndx[i]][3] < 0;
		}
		if (outside)
			return false;
	}

	return true;
}
*/

/*
bool FractalTerrain::IsCollided(const Driver *driver, Vector3 &normalOut) const {
    Vector3 separation;
    float myrad = 7.0f*MAP_SIZE/16.0f;
    
    separation = driver->GetPos();
    separation.SetY(0);

    normalOut = -separation.Normalized();

    return separation.DotProduct(separation) > myrad * myrad;
}


bool FractalTerrain::IsCollided(const ProjectileWeapon *projectileWeapon) const {
    Vector3 separation;
    float myrad = 7.0f*MAP_SIZE/16.0f;
    
    separation = projectileWeapon->GetPos();

    return separation.DotProduct(separation) > myrad * myrad;
}
*/

/*
float FractalTerrain::GetHeight(float x, float z) const {
    return mMap[(int)(x+MAP_SIZE/2)][(int)(z+MAP_SIZE/2)];
}
*/

/*
void FractalTerrain::Split(BTTNode *node, int lvl) {
    if (lvl > 0) {
		if (!node->LeftChild) {
			Split(node);
		}
		Split(node->LeftChild, lvl - 1);
		Split(node->RightChild, lvl - 1);
	} 
}
*/

BTTNode *FractalTerrain::AllocateNode() {
	BTTNode *node;

    if (mNextNode >= NUM_NODES) {
        cerr << "Cannot allocate another BTT node!" << endl;
        cin.get();
		exit(1);
    }

    node = &mNodePool[mNextNode];
    mNextNode++;
    node->BottomNeighbor = node->LeftNeighbor = node->RightNeighbor = node->LeftChild = node->RightChild = 0;

	return node;
}

/*
void FractalTerrain::CopyIndex(BTTNode *n1, int ndx1, BTTNode *n2, int ndx2) {
	n1->corners[ndx1] = n2->corners[ndx2];
}

void FractalTerrain::CopyIndex(BTTNode *n, int ndx, Vector3 *v) {
	n->corners[ndx].x = v->x;
	n->corners[ndx].y = v->y;
	n->corners[ndx].z = v->z;
}
*/

void FractalTerrain::Split(BTTNode *node) {
    if (node->LeftChild)
        return;

	if (node->BottomNeighbor) {
		if (node->BottomNeighbor->BottomNeighbor != node) {
			Split(node->BottomNeighbor);
		}
		Split2(node);
		Split2(node->BottomNeighbor);
		node->LeftChild->RightNeighbor = node->BottomNeighbor->RightChild;
		node->RightChild->LeftNeighbor = node->BottomNeighbor->LeftChild;
		node->BottomNeighbor->LeftChild->RightNeighbor = node->RightChild;
		node->BottomNeighbor->RightChild->LeftNeighbor = node->LeftChild;
	}
	else {
		Split2(node);
		node->LeftChild->RightNeighbor = 0;
		node->RightChild->LeftNeighbor = 0;
	}
}

void FractalTerrain::Split2(BTTNode *node) {
	Vector3 vc;
	
	node->LeftChild = AllocateNode();
	node->RightChild = AllocateNode();

	//vc = (node->corners[0] + node->corners[1])/2;
	//vc.y = mGameTerrain[(int)vc.x][(int)vc.z];//mHeightMap->GetHeight(vc.x, vc.z);

	//CopyIndex(node->LeftChild, 0, node, 2);
	//CopyIndex(node->LeftChild, 1, node, 0);
	//CopyIndex(node->LeftChild, 2, &vc);
	//CopyIndex(node->RightChild, 0, node, 1);
	//CopyIndex(node->RightChild, 1, node, 2);
	//CopyIndex(node->RightChild, 2, &vc);

	node->LeftChild->LeftNeighbor = node->RightChild;
	node->RightChild->RightNeighbor = node->LeftChild;

	node->LeftChild->BottomNeighbor = node->LeftNeighbor;
	if (node->LeftNeighbor) {
		if (node->LeftNeighbor->BottomNeighbor == node) {
			node->LeftNeighbor->BottomNeighbor = node->LeftChild;
		}
		else {
			if (node->LeftNeighbor->LeftNeighbor == node) {
				node->LeftNeighbor->LeftNeighbor = node->LeftChild;
			}
			else {
				node->LeftNeighbor->RightNeighbor = node->LeftChild;
			}
		}
	}

	node->RightChild->BottomNeighbor = node->RightNeighbor;
	if (node->RightNeighbor) {
		if (node->RightNeighbor->BottomNeighbor == node) {
			node->RightNeighbor->BottomNeighbor = node->RightChild;
		}
		else {
			if (node->RightNeighbor->RightNeighbor == node) {
				node->RightNeighbor->RightNeighbor = node->RightChild;
			}
			else {
				node->RightNeighbor->LeftNeighbor = node->RightChild;
			}
		}
	}

	node->LeftChild->LeftChild = 0;
	node->LeftChild->RightChild = 0;
	node->RightChild->LeftChild = 0;
	node->RightChild->RightChild = 0;
}

void FractalTerrain::Tessellate(BTTNode *node, const Vector3 &eye, int lX, int lZ, int rX, int rZ, int aX, int aZ, int n) {

    if (n < TREE_NODES && NodeVFIntersect(lX, lZ, rX, rZ, aX, aZ)) {
        float dist;
        int cX = (lX + rX) >> 1;
        int cZ = (lZ + rZ) >> 1;
        //Vector3 vc = (node->corners[0] + node->corners[1])*0.5f;

        dist = Vector3(cX-eye.GetX(), 0, cZ-eye.GetZ()).Length();

        if (mCurVariancePtr[n] * MAP_SIZE * 2 / dist > mRenderThresh)
        {
            Split(node);

            //int x = (node->corners[0].x - node->corners[1].x);
            //int y = (node->corners[0].y - node->corners[1].y);


            if (node->LeftChild &&	
                //(abs(node->corners[0].x - node->corners[1].x) >= 3 || abs(node->corners[0].y - node->corners[1].y) >= 3))
                (abs(lX - rX) >= 3 || abs(lZ - rZ) >= 3))
		    {
			    Tessellate(node->LeftChild, eye, aX, aZ, lX, lZ, cX, cZ, n << 1);
			    Tessellate(node->RightChild, eye, rX, rZ, aX, aZ, cX, cZ, 1+(n << 1));
		    }
        }
        //else {
        //    mRenderList.push_back(node);
        //}
    }
}

void FractalTerrain::Render(BTTNode *node, int lX, int lZ, int rX, int rZ, int aX, int aZ) {
	if (node->LeftChild)					// All non-leaf nodes have both children, so just check for one
	{
		int cX = (lX + rX)>>1;	// Compute X coordinate of center of Hypotenuse
		int cZ = (lZ + rZ)>>1;	// Compute Y coord...

		Render(node->LeftChild,  aX, aZ, lX, lZ, cX, cZ);//apexY, leftX, leftY, centerX, centerY );
		Render(node->RightChild, rX, rZ, aX, aZ, cX, cZ);//rightX, rightY, apexX, apexY, centerX, centerY );
	}
    else {
		numTris++;
        float lY = mGameTerrain[lX][lZ];
        float rY = mGameTerrain[rX][rZ];
        float aY = mGameTerrain[aX][aZ];

        Vector3 norm = Vector3(lX-aX,lY-aY,lZ-aZ).CrossProduct(Vector3(rX-aX,rY-aY,rZ-aZ));

        glNormal3f(norm.GetX(), norm.GetY(), norm.GetZ());
        //glBegin(GL_LINE_LOOP);
        glTexCoord2f(0,0);
	    glVertex3f(lX, lY, lZ);
        glTexCoord2f(.5,1);
        glVertex3f(rX, rY, rZ);
        glTexCoord2f(1,0);
        glVertex3f(aX, aY, aZ);
        //glEnd();

		
    }
}

float FractalTerrain::ComputeVariance(int lX, int lZ, float lY, int rX, int rZ, float rY, int aX, int aZ, float aY, int node) {
    int cX = (lX + rX) >> 1;
	int cZ = (lZ + rZ) >> 1;
	float var = 0, cY;

	cY  = mGameTerrain[cX][cZ];

    var = abs(cY - (((int)lY + (int)rY) >> 1));

	// Since we're after speed and not perfect representations,
	//    only calculate variance down to an 8x8 block
	if ( node < TREE_NODES && ((abs(lX - rX) >= 3) ||
		 (abs(lZ - rZ) >= 3)) )
	{
        var = max(var, ComputeVariance(aX, aZ, aY, lX, lZ, lY, cX, cZ, cY, node<<1));
        var = max(var, ComputeVariance(rX, rZ, rY, aX, aZ, aY, cX, cZ, cY, 1+(node<<1)));
        mCurVariancePtr[node] = 1 + var;
        /*
        if (mCurVariancePtr[node] < 0) {
            cout << "Variance less than zero!" << endl;
        }
        if (mCurVariancePtr[node] == 0) {
            cout << "Variance equals zero!" << endl;
        }
        */
		// Final Variance for this node is the max of it's own variance and that of it's children.
		//myVariance = MAX( myVariance, RecursComputeVariance( apexX,   apexY,  apexZ, leftX, leftY, leftZ, centerX, centerY, centerZ,    node<<1 ) );
		//myVariance = MAX( myVariance, RecursComputeVariance( rightX, rightY, rightZ, apexX, apexY, apexZ, centerX, centerY, centerZ, 1+(node<<1)) );
	}

	// Store the final variance for this node.  Note Variance is never zero.
	//if (node < MAP_SIZE)
	//	mCurVariancePtr[node] = 1 + var;

	return var;
}

/*
unsigned char FractalTerrain::CalcVariance(BTTNode *node) {
	Vector3 vc;
	unsigned char v;
	float val;

	vc = (tree[i].corners[0] + tree[i].corners[1])/2;
	val = mGameTerrain[(int)vc.x][(int)vc.z];//mHeightMap->GetHeight(vc.x, vc.z);
	v = abs(vc.y - val);
    if (node->LeftChild) {
        v += max(CalcVariance(node->LeftChild), CalcVariance(node->RightChild));
		//v = max(v, CalcVariance(node->LeftChild));
		//v = max(v, CalcVariance(node->RightChild));
	}

	node->variance = v;
	return v;
}
*/