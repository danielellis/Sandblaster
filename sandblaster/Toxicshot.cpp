#include "Toxicshot.h"
#include "SBLib.h"
#include "BMPLoader.h"
#include "Shader.h"
#include <SDL/SDL_OpenGL.h>
#include <iostream>
#include <cstdlib>

using namespace std;

MeshObject Toxicshot::mToxicShotMesh("assets/models/toxicshot.obj");

//Profiles
CGprofile Toxicshot::vsToxicshotProfile;
CGprofile Toxicshot::psToxicshotProfile;

//Programs
CGprogram Toxicshot::vsToxicshotProgram;
CGprogram Toxicshot::psToxicshotProgram;

//Parameters
CGparameter Toxicshot::vs_texcoord0, Toxicshot::position, Toxicshot::vs_color;
CGparameter Toxicshot::deform, Toxicshot::mvMatrix;
CGparameter Toxicshot::ps_color, Toxicshot::ps_texcoord0, Toxicshot::useColor;
CGparameter Toxicshot::move, Toxicshot::alpha;

//Texture parameters 
CGparameter Toxicshot::text1;

void Toxicshot::InitializeToxicshotShaders() {
    GLuint ToxicshotTexIDs[1];
    BMPImage *toxicTexture1;

    //Vertext Shader profile
    vsToxicshotProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
    //Set profile
    cgGLSetOptimalOptions(vsToxicshotProfile);
    CheckForCgError("toxic shot", "selecting vertex profile");

    //Load vertex shader
    vsToxicshotProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "assets/vertexshaders/vs_toxicshot.cg", vsToxicshotProfile, "vertMain", 0);

    CheckForCgError("toxic shot", "creating vertex program from file");
    cgGLLoadProgram(vsToxicshotProgram);
    CheckForCgError("toxic shot", "loading vertex program");

    vs_texcoord0			= cgGetNamedParameter(vsToxicshotProgram, "texCoordIN");
    position		        = cgGetNamedParameter(vsToxicshotProgram, "posIN");
    vs_color			    = cgGetNamedParameter(vsToxicshotProgram, "colorIN");
	deform			        = cgGetNamedParameter(vsToxicshotProgram, "deform");
    mvMatrix		        = cgGetNamedParameter(vsToxicshotProgram, "modelViewProj");
    
    /////////////////////////////
    //Pixel Shader
	  
    psToxicshotProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(psToxicshotProfile);
    CheckForCgError("toxic shot", "selecting fragment profile");

    psToxicshotProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "assets/pixelshaders/ps_toxicshot.cg", psToxicshotProfile, "pixelMain", 0);
    CheckForCgError("toxic shot", "creating fragment program from file");
    cgGLLoadProgram(psToxicshotProgram);
    CheckForCgError("toxic shot", "loading fragment program");
	
    glGenTextures(1, ToxicshotTexIDs);

    ps_color				= cgGetNamedParameter(psToxicshotProgram, "colorIN");
	ps_texcoord0			= cgGetNamedParameter(psToxicshotProgram, "texCoord");
	useColor				= cgGetNamedParameter(psToxicshotProgram,  "useColor");
	move				    = cgGetNamedParameter(psToxicshotProgram, "moveTex");
	alpha     				= cgGetNamedParameter(psToxicshotProgram, "alpha");
    
    toxicTexture1 = BMPLoader::LoadTexture("assets/textures/weapon/toxic1.bmp", ToxicshotTexIDs[0]);

    text1 = cgGetNamedParameter(psToxicshotProgram, "text1");
    cgGLSetTextureParameter(text1, ToxicshotTexIDs[0]);

    CheckForCgError("toxic shot", "setting decal 2D texture");

   delete toxicTexture1->data;
   delete toxicTexture1;
}

void Toxicshot::Update(float dt) {
    mPrevPos = mPos;
    mPos += mVel/2.5;

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
		if(glow<= .2)
		{
			glowDir = true;
			glow = 0.2f;
		}
	}

	if(deformVal >= 360)
	{
		deformVal = 0;
	}
	deformVal += 0.1f;

	if(moveGlowVal >= 360)
	{
		moveGlowVal = 0;
	}
	moveGlowVal += .05f;

}

void Toxicshot::Render() {

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
		
		glTranslatef(mPos.GetX(), mPos.GetY(), mPos.GetZ());
        glRotatef(-mTheta * 180 / PI - 90, 0, 1.0f, 0);
		glScalef(0.7f, 0.7f, 0.7f);

	    //Model view transforms must go before passing hte modelview matrix into hte shader
	    cgGLSetStateMatrixParameter(mvMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	    glPushMatrix();

        mToxicShotMesh.GLDraw();

        /*
       glBegin(GL_TRIANGLES);
       for (unsigned i = 0; i < ToxicshotMesh->faces; i++) {
          f = &ToxicshotMesh->faceL[i];
	         glTexCoord2f(ToxicshotMesh->pointL[f->points[0]].pos[0]/2,ToxicshotMesh->pointL[f->points[0]].pos[1]/2);
             glVertex3fv(ToxicshotMesh->pointL[f->points[0]].pos);

		     glTexCoord2f(ToxicshotMesh->pointL[f->points[1]].pos[0]/2,ToxicshotMesh->pointL[f->points[1]].pos[1]/2);
             glVertex3fv(ToxicshotMesh->pointL[f->points[1]].pos);

		     glTexCoord2f(ToxicshotMesh->pointL[f->points[2]].pos[0]/2,ToxicshotMesh->pointL[f->points[2]].pos[1]/2);
             glVertex3fv(ToxicshotMesh->pointL[f->points[2]].pos);
       }
       
       glEnd();
       */
       glPopMatrix();
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
		glTranslatef(mPos.GetX(), mPos.GetY(), mPos.GetZ());
        glRotatef(-mTheta * 180 / PI - 90, 0, 1.0, 0);
		glScalef(0.9f, 0.9f, 0.9f);
		glColor3f(0.5f, 1, 0.7f);

	    //Model view transforms must go before passing hte modelview matrix into hte shader
	    cgGLSetStateMatrixParameter(mvMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	    glPushMatrix();

        mToxicShotMesh.GLDraw();
        /*
       glBegin(GL_TRIANGLES);
       for (unsigned i = 0; i < ToxicshotMesh->faces; i++) {
          f = &ToxicshotMesh->faceL[i];
	         glTexCoord2f(ToxicshotMesh->pointL[f->points[0]].pos[0]/2,ToxicshotMesh->pointL[f->points[0]].pos[1]/2);
             glVertex3fv(ToxicshotMesh->pointL[f->points[0]].pos);

		     glTexCoord2f(ToxicshotMesh->pointL[f->points[1]].pos[0]/2,ToxicshotMesh->pointL[f->points[1]].pos[1]/2);
             glVertex3fv(ToxicshotMesh->pointL[f->points[1]].pos);

		     glTexCoord2f(ToxicshotMesh->pointL[f->points[2]].pos[0]/2,ToxicshotMesh->pointL[f->points[2]].pos[1]/2);
             glVertex3fv(ToxicshotMesh->pointL[f->points[2]].pos);
       }
       glEnd();
       */
       glPopMatrix();
	glPopMatrix();

  cgGLDisableProfile(vsToxicshotProfile);
  cgGLDisableProfile(psToxicshotProfile);

}