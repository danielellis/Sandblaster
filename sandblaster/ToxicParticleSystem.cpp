#include "ToxicParticleSystem.h"
#include "BMPLoader.h"
#include "SBLib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

//Profiles
CGprofile ToxicParticleSystem::vsToxicshotProfile;
CGprofile ToxicParticleSystem::psToxicshotProfile;

//Programs
CGprogram ToxicParticleSystem::vsToxicshotProgram;
CGprogram ToxicParticleSystem::psToxicshotProgram;

//Parameters
CGparameter ToxicParticleSystem::vs_texcoord0, ToxicParticleSystem::position, ToxicParticleSystem::vs_color;
CGparameter ToxicParticleSystem::deform, ToxicParticleSystem::mvMatrix;
CGparameter ToxicParticleSystem::ps_color, ToxicParticleSystem::ps_texcoord0, ToxicParticleSystem::useColor;
CGparameter ToxicParticleSystem::move, ToxicParticleSystem::alpha;

//Texture parameters 
CGparameter ToxicParticleSystem::text1;

MeshObject ToxicParticleSystem::mToxicParticleMesh("assets/models/toxicparticle.obj");

void ToxicParticleSystem::InitializeToxicParticleSystemShader() {
    GLuint ToxicshotTexIDs[1];
    BMPImage *toxicTexture1;

    //Vertext Shader profile
    vsToxicshotProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
    //Set profile
    cgGLSetOptimalOptions(vsToxicshotProfile);
    CheckForCgError("toxic particle system", "selecting vertex profile");

    //Load vertex shader
    vsToxicshotProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "vertexshaders/vs_toxicshot.cg", vsToxicshotProfile, "vertMain", 0);

    CheckForCgError("toxic particle system", "creating vertex program from file");
    cgGLLoadProgram(vsToxicshotProgram);
    CheckForCgError("toxic particle system", "loading vertex program");

    vs_texcoord0			= cgGetNamedParameter(vsToxicshotProgram, "texCoordIN");
    position		        = cgGetNamedParameter(vsToxicshotProgram, "posIN");
    vs_color			    = cgGetNamedParameter(vsToxicshotProgram, "colorIN");
	deform			        = cgGetNamedParameter(vsToxicshotProgram, "deform");
    mvMatrix		        = cgGetNamedParameter(vsToxicshotProgram, "modelViewProj");
    
    /////////////////////////////
    //Pixel Shader
	  
    psToxicshotProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(psToxicshotProfile);
    CheckForCgError("toxic particle system", "selecting fragment profile");

    psToxicshotProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "pixelshaders/ps_toxicshot.cg", psToxicshotProfile, "pixelMain", 0);
    CheckForCgError("toxic particle system", "creating fragment program from file");
    cgGLLoadProgram(psToxicshotProgram);
    CheckForCgError("toxic particle system", "loading fragment program");
	
    glGenTextures(1, ToxicshotTexIDs);

    ps_color				= cgGetNamedParameter(psToxicshotProgram, "colorIN");
	ps_texcoord0			= cgGetNamedParameter(psToxicshotProgram, "texCoord");
	useColor				= cgGetNamedParameter(psToxicshotProgram,  "useColor");
	move				    = cgGetNamedParameter(psToxicshotProgram, "moveTex");
	alpha     				= cgGetNamedParameter(psToxicshotProgram, "alpha");
    
    toxicTexture1 = BMPLoader::LoadTexture("assets/textures/weapon/toxic1.bmp", ToxicshotTexIDs[0]);

    text1 = cgGetNamedParameter(psToxicshotProgram, "text1");
    cgGLSetTextureParameter(text1, ToxicshotTexIDs[0]);

    CheckForCgError("toxic particle system", "setting decal 2D texture");

    mToxicParticleMesh.Scale(0.2f);

   delete toxicTexture1->data;
   delete toxicTexture1;
}

ToxicParticleSystem::ToxicParticleSystem() {
    mParticleCount = 0;
    glow = 0.3f;
	glowDir = true;
	deformVal = 0.0f;
	moveGlowVal = 0.0f;
}

void ToxicParticleSystem::Initialize(const Vector3 &pos, const Vector3 &vel) {
    mParticleCount = 8;
    Vector3 up = Vector3(0,1,0);
    Vector3 axis = vel.CrossProduct(up);
    ToxicParticle tp;

    axis.Normalize();
    srand((unsigned int)time(NULL));

    mParticles.clear();
    for (float i = 0; i < PI; i += PI/mParticleCount) {
        tp.mLife = 0;
        tp.mPosition = pos;
        tp.mVelocity = 2.5f * vel + (float)(rand() % 6) * axis * cos(i)
			+ (float)(rand() % 6) * up * sin(i);

        mParticles.push_back(tp);
    }
}

void ToxicParticleSystem::Update(float dt) {
    list<ToxicParticle>::iterator itr;

    if(glowDir)
	{
		glow += 0.003f;
		if(glow >= 0.6f)
		{
			glowDir = false;
			glow = 0.6f;
		}
	}
	else
	{
		glow -= 0.003f;
		if(glow <= 0.2f)
		{
			glowDir = true;
			glow = 0.2f;
		}
	}

	if(deformVal >= 360.0f)
	{
		deformVal = 0;
	}
	deformVal += 0.1f;

	if(moveGlowVal >= 360.0f)
	{
		moveGlowVal = 0;
	}
	moveGlowVal += .05f;

    for (itr = mParticles.begin(); itr != mParticles.end(); itr++) {
        if ((*itr).IsAlive()) {
            (*itr).Update(dt);
            if ((*itr).mLife > 1.0f)
                mParticleCount--;
        }
    }
}

void ToxicParticleSystem::Render() {
    list<ToxicParticle>::iterator itr;

    
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    /*
    glColor3f(0,0,0);

    for (itr = mParticles.begin(); itr != mParticles.end(); itr++) {
        if ((*itr).IsAlive()) {
            glPushMatrix();
                glTranslatef((*itr).mPosition.GetX(), (*itr).mPosition.GetY(), (*itr).mPosition.GetZ());
                mToxicParticleMesh.GLDraw();
            glPopMatrix();
        }
    }
    */
    

    //glColor4f(1,1,1,1-mLife);
    //glVertex3f(mPosition.GetX(), mPosition.GetY(), mPosition.GetZ());

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);	
	glEnable(GL_BLEND);

	//Bind and load shader and profile
    cgGLEnableProfile(vsToxicshotProfile);
    cgGLBindProgram(vsToxicshotProgram);
  
  //NOTE WARNING USE FRAME RATE TO REGULATE SHADER VALUE INCREMENTS
  //or else changes become unnottoxicable as they are too fast.
  cgGLEnableProfile(psToxicshotProfile);
  cgGLBindProgram(psToxicshotProgram);

  //Pixel Parameters
  cgGLEnableTextureParameter(text1);
	 cgSetParameter1f(alpha, 1);
	  cgSetParameter1f(deform, deformVal);
	  cgSetParameter1f(move, moveGlowVal);
	  cgSetParameter1f(useColor, 0);
    glPushMatrix();
		
		//glTranslatef(mPosition.GetX(), mPosition.GetY(), mPosition.GetZ());
        //glRotatef(-mTheta * 180/PI - 90, 0, 1, 0);
		//glScalef(0.7,0.7,0.7);

        for (itr = mParticles.begin(); itr != mParticles.end(); itr++) {
            if ((*itr).IsAlive()) {
                glPushMatrix();
                    glTranslatef((*itr).mPosition.GetX(), (*itr).mPosition.GetY(), (*itr).mPosition.GetZ());
                    glScalef(0.7f, 0.7f, 0.7f);
                    //Model view transforms must go before passing hte modelview matrix into hte shader
                    cgGLSetStateMatrixParameter(mvMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

                    mToxicParticleMesh.GLDraw();
                glPopMatrix();
            }
        }
	glPopMatrix();

  cgGLDisableProfile(vsToxicshotProfile);
  cgGLDisableProfile(psToxicshotProfile);


  /*
   *	Second Pass
   ***********************/

  glEnable(GL_TEXTURE_2D);	
	glEnable(GL_BLEND);

	//Bind and load shader and profile
  cgGLEnableProfile(vsToxicshotProfile);
  cgGLBindProgram(vsToxicshotProgram); 

  
  //NOTE WARNING USE FRAME RATE TO REGULATE SHADER VALUE INCREMENTS
  //or else changes become unnottoxicable as they are too fast.
  cgGLEnableProfile(psToxicshotProfile);
  cgGLBindProgram(psToxicshotProgram);

  //Pixel Parameters
  cgGLEnableTextureParameter(text1);
	 cgSetParameter1f(alpha, glow);
	  cgSetParameter1f(deform, deformVal);
	  cgSetParameter1f(move, moveGlowVal);
	  cgSetParameter1f(useColor, 1);
    glPushMatrix();
		
        //glRotatef(-mTheta * 180/PI - 90, 0, 1, 0);
		//glScalef(0.9,0.9,0.9);
		glColor3f(0.5f, 1.0f, 0.7f);

	    glPushMatrix();

            for (itr = mParticles.begin(); itr != mParticles.end(); itr++) {
                if ((*itr).IsAlive()) {
                    glPushMatrix();
                        glTranslatef((*itr).mPosition.GetX(), (*itr).mPosition.GetY(), (*itr).mPosition.GetZ());
                        glScalef(0.9f, 0.9f, 0.9f);
                        //Model view transforms must go before passing hte modelview matrix into hte shader
	                    cgGLSetStateMatrixParameter(mvMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
                        mToxicParticleMesh.GLDraw();
                    glPopMatrix();
                }
            }

            
       glPopMatrix();
	glPopMatrix();

  cgGLDisableProfile(vsToxicshotProfile);
  cgGLDisableProfile(psToxicshotProfile);
  
}
