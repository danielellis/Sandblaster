#include "FreezeParticle.h"
#include "SBLib.h"
#include "SDL/SDL_OpenGl.h"
#include <cstdlib>
#include <cmath>
#include "TextureLoader.h"

CGprogram FreezeParticle::FreezeParticleProgram;
CGprofile FreezeParticle::FreezeParticleProfile;
CGparameter FreezeParticle::FreezeParticleText1, FreezeParticle::FreezeParticleCoord0;

void FreezeParticle::InitializeFreezeParticleShader()
{
	TextureLoader textureLoader;
    glTexture FreezeParticleTexture;

	FreezeParticleProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(FreezeParticleProfile);
    CheckForCgError("FreezeParticle", "selecting fragment profile");

    FreezeParticleProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "pixelshaders/ps_freezeparticle.cg", FreezeParticleProfile, "pixelMain", 0);
    CheckForCgError("FreezeParticle", "creating fragment program from file");
    cgGLLoadProgram(FreezeParticleProgram);
    CheckForCgError("FreezeParticle", "loading fragment program");

	FreezeParticleCoord0 = cgGetNamedParameter(FreezeParticleProgram, "texcoord0");

    textureLoader.LoadTextureFromDisk("weapontextures/freezeparticle.tga", &FreezeParticleTexture);
    FreezeParticleText1 = cgGetNamedParameter(FreezeParticleProgram, "text1");
    cgGLSetTextureParameter(FreezeParticleText1, FreezeParticleTexture.TextureID);
    CheckForCgError("FreezeParticle", "setting decal 2D texture");
}

void FreezeParticle::Initialize(float speed) {
    mLife = 0;
    mSpeed = speed;

    mPos = Vector3(0,0,0);
	mRotate = 360*((float)rand())/RAND_MAX;


    float angle1 = 1-2*((float)rand())/RAND_MAX;
	float angle2 = ((float)rand())/RAND_MAX;
	float angle3 = 1-2*((float)rand())/RAND_MAX;

    mVel = Vector3(angle1,angle2,angle3);

    mVel = (((float)rand())/RAND_MAX)*mSpeed* mVel;
}

void FreezeParticle::Reset()
{
	mLife = 0;
	mPos = Vector3(0,0,0);
	mRotate = 360*((float)rand())/RAND_MAX;

	float angle1 = .5-1*((float)rand())/RAND_MAX;
	float angle2 = ((float)rand())/RAND_MAX;
	float angle3 = .5-1*((float)rand())/RAND_MAX;

    mVel = Vector3(angle1,angle2,angle3);
    mVel = (((float)rand())/RAND_MAX)*mSpeed * mVel;
}

void FreezeParticle::Update(float dt) {
    mPrevPos = mPos;

	mPos+=dt*mVel;
	mPos.SetX(mPos.GetX() + dt*mVel.GetX());	
	mPos.SetY(mPos.GetY() + dt*mVel.GetY() + 0.5*-0.98/100*dt*dt);
	mPos.SetZ(mPos.GetZ() + dt*mVel.GetZ());

     
      // CALCULATE NEW VEL
	 // mVel.SetX( mVel.GetX() + 0.98 * TIMESTEP);
      mVel.SetY( mVel.GetY() + -0.98/100* dt);
	 // mVel.SetZ( mVel.GetZ() + 0.98 * TIMESTEP);
	
	
	
	mLife += dt*(((float)rand())/RAND_MAX);
	mRotate += 20;
	if(mRotate>360)
	{
		mRotate-=360;
	}
}

void FreezeParticle::Render(float x, float y, float z,
							float theta, float thetaX, float thetaY, float thetaZ) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);	
	glEnable(GL_BLEND);

	cgGLEnableProfile(FreezeParticleProfile);
    cgGLBindProgram(FreezeParticleProgram);		
	cgGLEnableTextureParameter(FreezeParticleText1);

	glPushMatrix();
	glTranslatef(x,y,z);
	glTranslatef(mPos.GetX(), mPos.GetY(), mPos.GetZ());
	glRotatef(mRotate,.2,.5,1);
	glRotatef(theta, thetaX, thetaY, thetaZ);
	glScalef((30-mLife)/30,(30-mLife)/30,(30-mLife)/30);

    glBegin(GL_TRIANGLES);
		glTexCoord3f(0,0,0);
        glVertex3f(-3,0,0);
		glTexCoord3f(.5,1,0);
		glVertex3f(0, 3, 0);
		glTexCoord3f(0,0,0);
		glVertex3f(1, 0, 0);
    glEnd();
	glPopMatrix();

	cgGLDisableProfile(FreezeParticleProfile);
}