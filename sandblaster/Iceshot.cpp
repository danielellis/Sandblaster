#include "Iceshot.h"
#include "SBLib.h"
#include "BMPLoader.h"
#include "Shader.h"
#include <SDL/SDL_OpenGL.h>
#include <iostream>
#include <cstdlib>

using namespace std;

MeshObject Iceshot::mIceMesh("models/iceshot.obj");
//Profiles
CGprofile Iceshot::vsIceshotProfile;
CGprofile Iceshot::psIceshotProfile;

//Programs
CGprogram Iceshot::vsIceshotProgram;
CGprogram Iceshot::psIceshotProgram;

//Parameters
CGparameter	Iceshot::position, Iceshot::texVert, Iceshot::mvMatrix;//, Iceshot::move;	
CGparameter	Iceshot::texcoord0, Iceshot::shift;

//Texture parameters 
CGparameter Iceshot::text1, Iceshot::text2, Iceshot::text3;

void Iceshot::InitializeIceshotShaders() {
    GLuint IceshotTexIDs[3];
    BMPImage *iceTexture1, *iceTexture2,*iceTexture3;

    //Vertext Shader profile
    vsIceshotProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
    //Set profile
    cgGLSetOptimalOptions(vsIceshotProfile);
    CheckForCgError("ice shot", "selecting vertex profile");

    //Load vertex shader
    vsIceshotProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "vertexshaders/vs_iceshot.cg", vsIceshotProfile, "vertMain", 0);

    CheckForCgError("ice shot", "creating vertex program from file");
    cgGLLoadProgram(vsIceshotProgram);
    CheckForCgError("ice shot", "loading vertex program");

    
    texVert			= cgGetNamedParameter(vsIceshotProgram, "texCoordIN");
    position		= cgGetNamedParameter(vsIceshotProgram, "posIN");
    //move			= cgGetNamedParameter(vsIceshotProgram, "move");
    mvMatrix		= cgGetNamedParameter(vsIceshotProgram, "ModelViewProj");
    
    /////////////////////////////
    //Pixel Shader
	  
    psIceshotProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(psIceshotProfile);
    CheckForCgError("ice shot", "selecting fragment profile");

    psIceshotProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "pixelshaders/ps_iceshot.cg", psIceshotProfile, "pixelMain", 0);
    CheckForCgError("ice shot", "creating fragment program from file");
    cgGLLoadProgram(psIceshotProgram);
    CheckForCgError("ice shot", "loading fragment program");
	
    glGenTextures(3, IceshotTexIDs);
 
	shift		   = cgGetNamedParameter(psIceshotProgram, "textShift");
    
    iceTexture1 = BMPLoader::LoadTexture("weapontextures/ice1.bmp", IceshotTexIDs[0]);
    iceTexture2 = BMPLoader::LoadTexture("weapontextures/ice2.bmp", IceshotTexIDs[1]);
	iceTexture3 = BMPLoader::LoadTexture("weapontextures/ice3.bmp", IceshotTexIDs[2]);

    text1 = cgGetNamedParameter(psIceshotProgram, "text1");
    cgGLSetTextureParameter(text1, IceshotTexIDs[0]);
    text2 = cgGetNamedParameter(psIceshotProgram, "text2");
    cgGLSetTextureParameter(text2, IceshotTexIDs[1]);
	text3 = cgGetNamedParameter(psIceshotProgram, "text3");
    cgGLSetTextureParameter(text3, IceshotTexIDs[2]);

    CheckForCgError("ice shot", "setting decal 2D texture");

   delete iceTexture1->data;
   delete iceTexture2->data;
   delete iceTexture3->data;
   delete iceTexture1;
   delete iceTexture2;
   delete iceTexture3;

    //Temporarily Unbind Texture
    //glDisable(GL_TEXTURE_2D);	
    //glDisable(GL_BLEND);						// Disable Blending
    //glBindTexture(GL_TEXTURE_2D,0);	
}

void Iceshot::Update(float dt) {
    mPrevPos = mPos;

    mPos += mVel;

	if(shineDir) {shineVal += 0.05f;}
	else{ shineVal -= 0.05f;}

	if(shineVal >= 1)
	{
		shineVal = 1.0f;
		shineDir = false;
	}
	if(shineVal <=0)
	{
		shineVal = 0.0f;
		shineDir = true;
	}


	if (glitterDir)
	{
		glitterVal += 0.01f;
	}
	else
	{
		glitterVal -= 0.01f;
	}

	if(glitterVal >= 1)
	{
		glitterVal = 1.0;
		glitterDir = false;
	}
	if(glitterVal <=0)
	{
		glitterVal = 0.0;
		glitterDir = true;
	}

	rotate += 5;
	if(rotate == 360)
	{
		rotate = 0;
	}
}

void Iceshot::Render() {

	glEnable(GL_TEXTURE_2D);	
	glEnable(GL_BLEND);

	//Bind and load shader and profile
  cgGLEnableProfile(vsIceshotProfile);
  cgGLBindProgram(vsIceshotProgram);
  
  //NOTE WARNING USE FRAME RATE TO REGULATE SHADER VALUE INCREMENTS
  //or else changes become unnoticeable as they are too fast.
  cgGLEnableProfile(psIceshotProfile);
  cgGLBindProgram(psIceshotProgram);

  //VertexShader Parameter
  //cgSetParameter1f(move, 1);
  
  //Pixel Parameters
  cgGLEnableTextureParameter(text1);
  cgGLEnableTextureParameter(text2);
  cgGLEnableTextureParameter(text3);
	
    glPushMatrix();
		
		glTranslatef(mPos.GetX(), mPos.GetY(), mPos.GetZ());
        glRotatef(-mTheta * 180 / PI - 90, 0, 1, 0);
		glRotatef(90.0f + rotate, 0.7f, 1.0f, 0.5f);
	    glScalef(2,2,2);
	    //Model view transforms must go before passing hte modelview matrix into hte shader
		cgSetParameter2f(shift,shineVal,glitterVal);

	    cgGLSetStateMatrixParameter(mvMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
        mIceMesh.GLDraw();
        /*
        glPushMatrix();

       glBegin(GL_TRIANGLES);
       for (unsigned i = 0; i < IceshotMesh->faces; i++) {
          f = &IceshotMesh->faceL[i];
	         glTexCoord2f(IceshotMesh->pointL[f->points[0]].pos[0]/2,IceshotMesh->pointL[f->points[0]].pos[1]/2);
             glVertex3fv(IceshotMesh->pointL[f->points[0]].pos);

		     glTexCoord2f(IceshotMesh->pointL[f->points[1]].pos[0]/2,IceshotMesh->pointL[f->points[1]].pos[1]/2);
             glVertex3fv(IceshotMesh->pointL[f->points[1]].pos);

		     glTexCoord2f(IceshotMesh->pointL[f->points[2]].pos[0]/2,IceshotMesh->pointL[f->points[2]].pos[1]/2);
             glVertex3fv(IceshotMesh->pointL[f->points[2]].pos);
       }
       glEnd();
       glPopMatrix();
       */
	glPopMatrix();

  cgGLDisableProfile(vsIceshotProfile);
  cgGLDisableProfile(psIceshotProfile);
}