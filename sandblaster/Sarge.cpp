#include "Sarge.h"
#include "WorldMode.h"

#include "SBLib.h"

//Profiles
CGprofile Sarge::psSargeProfile, Sarge::vsSargeProfile;

//Programs
CGprogram Sarge::psSargeProgram, Sarge::vsSargeProgram;

//Parameters
CGparameter Sarge::modelViewProj, Sarge::lightPosition, Sarge::eyevector, Sarge::normalmap, Sarge::decalmap, Sarge::vertin, Sarge::fragin, Sarge::lightViewProj;

//textures
glTexture Sarge::sargeColorTexture, Sarge::sargeNormalTexture;

//WorldMode *Sarge::mWorldMode;

MeshObject Sarge::mMeshObject("assets/models/sarge.obj");

void Sarge::InitializeSargeShaders() {
	TextureLoader textureLoader;

	/////////////////////////////
	//Vertex Shader

	vsSargeProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	cgGLSetOptimalOptions(vsSargeProfile);
	CheckForCgError("sarge", "selecting vertex profile");

	vsSargeProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "assets/vertexshaders/vs_sarge.cg", vsSargeProfile, "sargeVertexShader", 0);
	CheckForCgError("sarge", "creating vertex program from file");
	cgGLLoadProgram(vsSargeProgram);
	CheckForCgError("sarge", "loading vertex program");


	//////////////////////////////
	//Pixel Shader

	psSargeProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(psSargeProfile);
	CheckForCgError("sarge", "selecting pixel profile");

	psSargeProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "assets/pixelshaders/ps_sarge.cg", psSargeProfile, "sargeLightMap", 0);
	CheckForCgError("sarge", "creating fragment program from file");
	cgGLLoadProgram(psSargeProgram);
	CheckForCgError("sarge", "loading fragment program");

	modelViewProj	= cgGetNamedParameter(vsSargeProgram, "modelViewProj");
	lightViewProj	= cgGetNamedParameter(vsSargeProgram, "lightViewProj");
	vertin			= cgGetNamedParameter(vsSargeProgram, "vertin");
	lightPosition	= cgGetNamedParameter(vsSargeProgram, "lightPosition");
	fragin			= cgGetNamedParameter(psSargeProgram, "fragin");
	eyevector		= cgGetNamedParameter(psSargeProgram, "eyevector");
	


	CheckForCgError("sarge", "getting parameters");

	
	textureLoader.LoadTextureFromDisk("assets/textures/shadermaps/sarge_normalmap.tga", &sargeNormalTexture);
	textureLoader.LoadTextureFromDisk("assets/textures/shadermaps/sarge_colormap.tga", &sargeColorTexture);

	//textureLoader.LoadTextureFromDisk("assets/textures/weapon/fire1.tga", &sargeColorTexture);

	normalmap = cgGetNamedParameter(psSargeProgram, "normalmap");
	cgGLSetTextureParameter(normalmap, sargeNormalTexture.TextureID);
    decalmap = cgGetNamedParameter(psSargeProgram, "decalmap");
    cgGLSetTextureParameter(decalmap, sargeColorTexture.TextureID);
    CheckForCgError("sarge", "setting decal 2D texture");


	/*
	decalmap = cgGetNamedParameter(psSargeProgram, "decalmap");
	cgGLSetTextureParameter(text1, sargeColorTexture.TextureID);
	CheckForCgError("sarge", "setting decal 2D texture");
	
	*/

    mMeshObject.Scale(1.6f);
}

void Sarge::Initialize(WorldMode *worldMode, int health, int maxHealth, 
    float phi, float theta,
    float boundsRad, float mass, //enum projectile_type primaryWeapon, int secondaryWeaponNum,
    const Vector3 &pos, 
    const Vector3 &prevPos = Vector3(0,0,0),
    const Vector3 &vel = Vector3(0,0,0)) 
{
	Driver::Initialize(worldMode, health, maxHealth, phi, theta, boundsRad, mass, /*primaryWeapon,
		secondaryWeaponNum,*/ pos, prevPos, vel);

	Sarge::mWorldMode = worldMode;
}

void Sarge::StrafeForward() {
    ApplyForce(50 * mDir);
}

void Sarge::StrafeBackward() {
    ApplyForce(-30 * mDir);
}

void Sarge::StrafeLeft() {
    ApplyForce(-50 * GetU());
}

void Sarge::StrafeRight() {
    ApplyForce(50 * GetU());
}

void Sarge::Render() {
	
	
	
	cgGLBindProgram(vsSargeProgram);
	cgGLEnableProfile(vsSargeProfile);
	CheckForCgError("sarge", "enabling vertex program");
	
	
	cgGLBindProgram(psSargeProgram);
	cgGLEnableProfile(psSargeProfile);
	CheckForCgError("sarge", "enabling pixel program");

//	mSkydome->getSunPos()

	//Vector3 tmpsun = mWorldMode->getSkydome().getSunPos();

	cgGLSetParameter4f(lightPosition, 0.0, 20.0, 0.0, 1.0);
	//cgGLSetParameter4f(lightPosition, tmpsun.GetX(), tmpsun.GetY(), tmpsun.GetZ(), 1.0);
	cgGLSetParameter3f(eyevector, 0.0, 0.0, 5.0);

	cgGLEnableTextureParameter(normalmap);
	cgGLEnableTextureParameter(decalmap);

	/*
	glActiveTextureARB(GL_TEXTURE0_ARB);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, sargeNormalTexture.TextureID);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, sargeColorTexture.TextureID);
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glActiveTextureARB(GL_TEXTURE3_ARB);
	glActiveTextureARB(GL_TEXTURE4_ARB);
	*/

	glDisable(GL_LIGHTING);
	
	


	//Pixel Parameters
	//      cgGLEnableTextureParameter(text1);

	glPushMatrix();

	cgGLSetStateMatrixParameter(lightViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	glTranslatef(mPos.GetX(), mPos.GetY(), mPos.GetZ());
	glRotatef(-mTheta * 180/PI - 90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	//glScalef(0.6,0.6,0.6);

	cgGLSetStateMatrixParameter(modelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	
	
	
	mDriverParticleSystem.Render();
	if(burned) glColor3f(0, 0, 0);
	else glColor3f(1, 1, 1);
	mMeshObject.GLDraw();
	glColor3f(1, 1, 1);
	

	glPopMatrix();
	
	glEnable(GL_LIGHTING);

	
	cgGLDisableProfile(vsSargeProfile);
	cgGLDisableProfile(psSargeProfile);


	
	/*
	//glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE4_ARB);
	glDisable(GL_TEXTURE_2D);
	*/
	
	glActiveTextureARB(GL_TEXTURE0_ARB);

	if(frozen)
	{	FreezeEffectBegin();
			glTranslatef(mPos.GetX(), mPos.GetY(), mPos.GetZ());
			glRotatef(-mTheta * 180/PI - 90, 0, 1, 0);
			glScalef(2.5,2.5,2.5);
			 cgGLSetStateMatrixParameter(vsFreezeEffectMVMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
		FreezeEffectEnd(); 

		if(freezeCounter > (16.0f/3.0f) && freezeCounter <= 8.0f)
		{
			glPushMatrix();
				mFreezeParticleSystem.Render(mPos.GetX(), mPos.GetY(), mPos.GetZ(),-mTheta * 180/PI - 90, 0, 1, 0);
			glPopMatrix();
		}
	}
	
	
}
