#include "Fireball.h"
#include "SBLib.h"
#include "BMPLoader.h"
#include "Shader.h"
#include <SDL/SDL_OpenGL.h>
#include <iostream>
#include <cstdlib>

using namespace std;

MeshObject Fireball::mFireballMesh("models/fireball.obj");

//Profiles
CGprofile Fireball::vsFireballProfile;
CGprofile Fireball::psFireballProfile;

//Programs
CGprogram Fireball::vsFireballProgram;
CGprogram Fireball::psFireballProgram;

//Parameters
CGparameter	Fireball::position, Fireball::texVert, Fireball::mvMatrix, Fireball::move;	
CGparameter	Fireball::texcoord0, Fireball::lightdist;

//Texture parameters 
CGparameter Fireball::text1, Fireball::text2;

void Fireball::InitializeFireBallShaders() {
    GLuint fireballTexIDs[2];
    BMPImage *fireTexture1, *fireTexture2;

    //Vertext Shader profile
    vsFireballProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
    //Set profile
    cgGLSetOptimalOptions(vsFireballProfile);
    CheckForCgError("fireball", "selecting vertex profile");

    //Load vertex shader
    vsFireballProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "vertexshaders/vs_fireball.cg", vsFireballProfile, "vertMain", 0);

    CheckForCgError("fireball", "creating vertex program from file");
    cgGLLoadProgram(vsFireballProgram);
    CheckForCgError("fireball", "loading vertex program");

    
    texVert			= cgGetNamedParameter(vsFireballProgram, "texCoordIN");
    position		= cgGetNamedParameter(vsFireballProgram, "posIN");
    move			= cgGetNamedParameter(vsFireballProgram, "move");
    mvMatrix		= cgGetNamedParameter(vsFireballProgram, "ModelViewProj");
    
    /////////////////////////////
    //Pixel Shader
	  
    psFireballProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(psFireballProfile);
    CheckForCgError("fireball", "selecting fragment profile");

    psFireballProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "pixelshaders/ps_fireball.cg", psFireballProfile, "pixelMain", 0);
    CheckForCgError("fireball", "creating fragment program from file");
    cgGLLoadProgram(psFireballProgram);
    CheckForCgError("fireball", "loading fragment program");

    texcoord0       = cgGetNamedParameter(psFireballProgram, "texcoord0");
    lightdist       = cgGetNamedParameter(psFireballProgram, "lightdist");
	
    glGenTextures(2, fireballTexIDs);
    
    fireTexture1 = BMPLoader::LoadTexture("weapontextures/fire1.bmp", fireballTexIDs[0]);
    fireTexture2 = BMPLoader::LoadTexture("weapontextures/fire2.bmp", fireballTexIDs[1]);

    text1 = cgGetNamedParameter(psFireballProgram, "text1");
    cgGLSetTextureParameter(text1, fireballTexIDs[0]);
    text2 = cgGetNamedParameter(psFireballProgram, "text2");
    cgGLSetTextureParameter(text2, fireballTexIDs[1]);
    CheckForCgError("fireball", "setting decal 2D texture");

   delete fireTexture1->data;
   delete fireTexture2->data;
   delete fireTexture1;
   delete fireTexture2;

    //Temporarily Unbind Texture
    //glDisable(GL_TEXTURE_2D);	
    //glDisable(GL_BLEND);						// Disable Blending
    //glBindTexture(GL_TEXTURE_2D,0);	
}

void Fireball::Update(float dt) {
    mPrevPos = mPos;

    mPos += mVel;

    if (mMoveDir) {
        mMove += dt;
    }
    else {
        mMove -= dt;
    }

    if (mMove > 1.0f) {
        mMoveDir = false;
        mMove = 1.0f;
    }
    else if (mMove < 0.0f) {
        mMoveDir = true;
        mMove = 0.0f;
    }
}

void Fireball::Render() {

	glEnable(GL_TEXTURE_2D);	
	glEnable(GL_BLEND);

	//Bind and load shader and profile
  cgGLEnableProfile(vsFireballProfile);
  cgGLBindProgram(vsFireballProgram);
  
  //NOTE WARNING USE FRAME RATE TO REGULATE SHADER VALUE INCREMENTS
  //or else changes become unnoticeable as they are too fast.
  cgGLEnableProfile(psFireballProfile);
  cgGLBindProgram(psFireballProgram);

  //VertexShader Parameter
  cgSetParameter1f(move, mMove);
  
  //Pixel Parameters
  cgGLEnableTextureParameter(text1);
  cgGLEnableTextureParameter(text2);
  
  cgSetParameter1f(lightdist, mMove);
	
    glPushMatrix();
		glTranslatef(mPos.GetX(), mPos.GetY(), mPos.GetZ());
        glRotatef(-mTheta * 180/PI - 90, 0, 1, 0);
        glRotatef(90, 1, 0, 0);

	    //Model view transforms must go before passing hte modelview matrix into hte shader
	    cgGLSetStateMatrixParameter(mvMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
        mFireballMesh.GLDraw();
        /*
        glPushMatrix();

       glBegin(GL_TRIANGLES);
       for (unsigned i = 0; i < FireballMesh->faces; i++) {
          f = &FireballMesh->faceL[i];
	         glTexCoord2f(FireballMesh->pointL[f->points[0]].pos[0]/2,FireballMesh->pointL[f->points[0]].pos[1]/2);
             glVertex3fv(FireballMesh->pointL[f->points[0]].pos);

		     glTexCoord2f(FireballMesh->pointL[f->points[1]].pos[0]/2,FireballMesh->pointL[f->points[1]].pos[1]/2);
             glVertex3fv(FireballMesh->pointL[f->points[1]].pos);

		     glTexCoord2f(FireballMesh->pointL[f->points[2]].pos[0]/2,FireballMesh->pointL[f->points[2]].pos[1]/2);
             glVertex3fv(FireballMesh->pointL[f->points[2]].pos);
       }
       glEnd();
       glPopMatrix();
       */
	glPopMatrix();

  cgGLDisableProfile(vsFireballProfile);
  cgGLDisableProfile(psFireballProfile);

  glDisable(GL_TEXTURE_2D);	
	glDisable(GL_BLEND);						// Disable Blending
	glBindTexture(GL_TEXTURE_2D,0);
}
