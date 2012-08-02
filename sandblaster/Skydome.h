#ifndef SKYDOME_H
#define SKYDOME_H

#include "Shader.h"
#include "Vector3.h"
#include "SBLib.h"

#define DTOR (PI/180.0f)
#define SQR(x) (x*x)

typedef struct {
	float x,y,z;
	unsigned int color;
	float u, v;
} VERTEX;

class Skydome
{
public:
    static void InitializeSkydomeShaders();

	void Initialize(float radius, float dtheta, float dphi, float hTile, float vTile);
	int Render();
	void ReleaseDome();
	void Update(float t);
   Vector3 getSunPos();
private:
        //Profiles
    //static CGprofile vsFireballProfile;
    static CGprofile psSkydomeProfile;
	static CGprofile psSkydomeSunProfile;

    //Programs
    //static CGprogram vsFireballProgram;
    static CGprogram psSkydomeProgram;
	static CGprogram psSkydomeSunProgram;

    static CGparameter texcoord0, dayTime, transition, suntexcoord0;

    //Texture parameters 
    static CGparameter text1, text2, text3;
	static CGparameter suntext1, suntext2;

	VERTEX *Vertices;
	int NumVertices;
	float rotTheta;

    float mDayTime;
    bool mTimeDir;

	int stage;

	Vector3 mSunPos;
	float mSunPhi;
};

#endif