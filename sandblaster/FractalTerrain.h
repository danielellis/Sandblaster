#ifndef FRACTAL_TERRAIN_H
#define FRACTAL_TERRAIN_H

#include "Vector3.h"
#include "Frustum.h"
#include "TextureLoader.h"
#include "Shader.h"

class Driver;
class ProjectileWeapon;

#define MAP_SIZE 256
#define LOW (-50)
#define RENDER_THRESH (25)
#define TREE_NODES ((MAP_SIZE)*(MAP_SIZE))
#define NUM_NODES (TREE_NODES*2)

struct BTTNode {
    BTTNode() : LeftNeighbor(0), RightNeighbor(0), BottomNeighbor(0), LeftChild(0), RightChild(0) {}

	BTTNode *LeftNeighbor, *RightNeighbor, *BottomNeighbor;
	BTTNode *LeftChild, *RightChild;

	//unsigned char variance;
	//Vector3 corners[3];

    bool IsLeaf;
};

class FractalTerrain {
public:
    FractalTerrain(Frustum *);
    ~FractalTerrain();

    //int GetTerrainMapSize() {return mMapSize;}
    int RenderTerrain(const Vector3);
    void ChangeRenderMode() {mRenderMode = (mRenderMode + 1) % 3;}
    //void Tesselate();

    void ChangeThresh(bool);

    //void Split(int);
    //void CopyIndex(BTTNode *, int , BTTNode *, int );
    //void CopyIndex(BTTNode *, int , Vector3 *);

    bool IsCollided(const Driver *driver, Vector3 &normalOut) const;
    bool IsCollided(const ProjectileWeapon *projectileWeapon) const;
    float GetBiLinIntHeight(float x, float z) const;
	static void InitializeFractalTerrainShader();

protected:
    void GenerateMap();
    static void DiamondSquare(float **Map, int arg);
    //void Split(BTTNode *, int lvl);
    //void Render(BTTNode *, const Vector3 &);
    //bool NodeVFIntersect(BTTNode *);
    bool NodeVFIntersect(int, int, int, int, int, int);
    //unsigned char CalcVariance(BTTNode *);

    void Reset();
    BTTNode *AllocateNode();
    void Split(BTTNode *);
    void Split2(BTTNode *);
    void Tessellate(BTTNode *, const Vector3 &, int, int, int, int, int, int, int);
    float ComputeVariance(int, int, float, int, int, float, int, int, float, int);
    void Render(BTTNode *, int, int, int, int, int, int);

    //float **mTerrainMap;
    //float **mMap1, **mMap2;
    float **mPlains;
    float **mMountains;
	float **mGameTerrain;

    BTTNode *mLeftRoot, *mRightRoot;

    //std::list<BTTNode *> mRenderList;

    float mLeftVariance[TREE_NODES], mRightVariance[TREE_NODES];
    float *mCurVariancePtr;

    BTTNode mNodePool[NUM_NODES];
    int mNextNode;

    Frustum *mVF;

    int mRenderThresh;

    int mRenderMode;
	glTexture mTexture;
	float mMaxHeight;

	static CGprofile psFractalProfile, vsFractalProfile;
	static CGprogram psFractalProgram, vsFractalProgram;
	static CGparameter FractalModelView, psHeight, terrainTexture, vsPosIN, vsHeightMax,vsTextCoord	;


    //int mMapSize;
};

/*
class FractalTerrain {
public:
    FractalTerrain();
    ~FractalTerrain();

    int GetTerrainMapSize() {return mMapSize;}
    void RenderTerrain();

    bool IsCollided(const Driver *driver, Vector3 &normalOut) const;
    bool IsCollided(const ProjectileWeapon *projectileWeapon) const;
    float GetHeight(float x, float z) const;

protected:
    void GenerateMap();
    static void DiamondSquare(float **Map, int arg);

    //float **mTerrainMap;
    //float **mMap1, **mMap2;
    float **mMap;

    int mMapSize;
    int MapID;
    //int mDisplayListID;
};
*/

#endif
