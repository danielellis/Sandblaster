#include "Scrappy.h"
#include "TextureLoader.h"
#include "SBLib.h"

//Profiles
CGprofile Scrappy::psScrappyProfile, Scrappy::vsScrappyProfile;

//Programs
CGprogram Scrappy::psScrappyProgram, Scrappy::vsScrappyProgram;

//Parameters
CGparameter Scrappy::texcoord0,  Scrappy::texcoord1, Scrappy::textShift, Scrappy::text1, Scrappy::text2, Scrappy::text3, Scrappy::text4, Scrappy::text5, Scrappy::text6;
CGparameter Scrappy::vsModelViewProj, Scrappy::vsLightViewProj, Scrappy::lightPosition;
CGparameter Scrappy::psNormalMap, Scrappy::vertin, Scrappy::psLightVector, Scrappy::colorIN;
MeshObject Scrappy::mMeshObject("assets/models/scrappy.obj");

void Scrappy::InitializeScrappyShaders() {
	TextureLoader textureLoader;
	glTexture ScrappyColorTexture, ScrappyBodyTexture, ScrappyHullTexture, ScrappyDeckTexture, ScrappySailTexture, ScrappySailColorTexture;
	glTexture ScrappyNormalMap;
	/////////////////////////////
	//Vertex Shader

	vsScrappyProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	cgGLSetOptimalOptions(vsScrappyProfile);
	CheckForCgError("scrappy", "selecting vertex profile");

	vsScrappyProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "assets/vertexshaders/vs_scrappy.cg", vsScrappyProfile, "scrappyVertexShader", 0);
	CheckForCgError("scrappy", "creating vertex program from file");
	cgGLLoadProgram(vsScrappyProgram);
	CheckForCgError("scrappy", "loading vertex program");

	vsModelViewProj	= cgGetNamedParameter(vsScrappyProgram, "modelViewProj");
	vsLightViewProj	= cgGetNamedParameter(vsScrappyProgram, "lightViewProj");
	vertin			= cgGetNamedParameter(vsScrappyProgram, "vertin");
	lightPosition	= cgGetNamedParameter(vsScrappyProgram, "lightPosition");

	/////////////////////////////
	//Pixel Shader

	psScrappyProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(psScrappyProfile);
	CheckForCgError("scrappy", "selecting fragment profile");

	psScrappyProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "assets/pixelshaders/ps_scrappy.cg", psScrappyProfile, "pixelMain", 0);
	CheckForCgError("scrappy", "creating fragment program from file");
	cgGLLoadProgram(psScrappyProgram);
	CheckForCgError("scrappy", "loading fragment program");

	texcoord0       = cgGetNamedParameter(psScrappyProgram, "texcoord0");
	texcoord1       = cgGetNamedParameter(psScrappyProgram, "texcoord1");
	textShift		= cgGetNamedParameter(psScrappyProgram, "textShift");
	psLightVector	= cgGetNamedParameter(psScrappyProgram, "lightVector");
	colorIN	        = cgGetNamedParameter(psScrappyProgram, "color");


	textureLoader.LoadTextureFromDisk("shadermaps/scrappy_colormap.tga", &ScrappyColorTexture);
	text1 = cgGetNamedParameter(psScrappyProgram, "text1");
	cgGLSetTextureParameter(text1, ScrappyColorTexture.TextureID);
	CheckForCgError("scrappy", "setting decal 2D texture");

	textureLoader.LoadTextureFromDisk("shadermaps/scrappy_bodymap.tga", &ScrappyBodyTexture);
	text2 = cgGetNamedParameter(psScrappyProgram, "text2");
	cgGLSetTextureParameter(text2, ScrappyBodyTexture.TextureID);
	CheckForCgError("scrappy", "setting decal 2D texture");

	textureLoader.LoadTextureFromDisk("shadermaps/scrappy_hullmap.tga", &ScrappyHullTexture);
	text3 = cgGetNamedParameter(psScrappyProgram, "text3");
	cgGLSetTextureParameter(text3, ScrappyHullTexture.TextureID);
	CheckForCgError("scrappy", "setting decal 2D texture");

	textureLoader.LoadTextureFromDisk("shadermaps/scrappy_deckmap.tga", &ScrappyDeckTexture);
	text4 = cgGetNamedParameter(psScrappyProgram, "text4");
	cgGLSetTextureParameter(text4, ScrappyDeckTexture.TextureID);
	CheckForCgError("scrappy", "setting decal 2D texture");

	textureLoader.LoadTextureFromDisk("shadermaps/scrappy_sailmap.tga", &ScrappySailTexture);
	text5 = cgGetNamedParameter(psScrappyProgram, "text5");
	cgGLSetTextureParameter(text5, ScrappySailTexture.TextureID);
	CheckForCgError("scrappy", "setting decal 2D texture");

	textureLoader.LoadTextureFromDisk("shadermaps/scrappy_sailcolormap.tga", &ScrappySailColorTexture);
	text6 = cgGetNamedParameter(psScrappyProgram, "text6");
	cgGLSetTextureParameter(text6, ScrappySailColorTexture.TextureID);
	CheckForCgError("scrappy", "setting decal 2D texture");

	textureLoader.LoadTextureFromDisk("shadermaps/scrappy_normalmap.tga", &ScrappyNormalMap);
	psNormalMap = cgGetNamedParameter(psScrappyProgram, "normalmap");
	cgGLSetTextureParameter(psNormalMap, ScrappyNormalMap.TextureID);
	CheckForCgError("scrappy", "setting decal 2D texture");

	//Scale Scrappy Mesh
	mMeshObject.Scale(1.0f);
}

void Scrappy::Initialize(WorldMode *worldMode, int health, int maxHealth, 
						 float phi, float theta,
						 float boundsRad, float mass, //enum projectile_type primaryWeapon, int secondaryWeaponNum,
						 const Vector3 &pos, 
						 const Vector3 &prevPos = Vector3(0,0,0),
						 const Vector3 &vel = Vector3(0,0,0)) 
{
	shift = 0.0f;

	Driver::Initialize(worldMode, health, maxHealth, phi, theta, boundsRad, mass, /*primaryWeapon,
																				  secondaryWeaponNum,*/ pos, prevPos, vel);
}

void Scrappy::StrafeForward() {
	ApplyForce(50 * mDir);
}

void Scrappy::StrafeBackward() {
	ApplyForce(-30 * mDir);
}

void Scrappy::StrafeLeft() {
	ApplyForce(-50 * GetU());
}

void Scrappy::StrafeRight() {
	ApplyForce(50 * GetU());
}

void Scrappy::Render() {

	shift += 0.01f;
	if(shift > 1.0f || shift < 0.0f)
	{
		shift = 0.0f;
	}


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);	
	glEnable(GL_BLEND);

	cgGLEnableProfile(vsScrappyProfile);
	cgGLBindProgram(vsScrappyProgram);

	cgGLEnableProfile(psScrappyProfile);
	cgGLBindProgram(psScrappyProgram);



	cgGLSetParameter4f(lightPosition, 0.0, 20.0, 0.0, 1.0);
	//cgGLSetParameter4f(lightPosition, tmpsun.GetX(), tmpsun.GetY(), tmpsun.GetZ(), 1.0);

	CheckForCgError("scrappy", "render5");

	//Pixel Parameters
	cgGLEnableTextureParameter(text1);
	cgGLEnableTextureParameter(text2);
	cgGLEnableTextureParameter(text3);
	cgGLEnableTextureParameter(text4);
	cgGLEnableTextureParameter(text5);
	cgGLEnableTextureParameter(text6);
	cgGLEnableTextureParameter(psNormalMap);
	CheckForCgError("scrappy", "render4");



	CheckForCgError("scrappy", "render3");

	glPushMatrix();
	cgGLSetStateMatrixParameter(vsLightViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	CheckForCgError("scrappy", "render2");

	glTranslatef(mPos.GetX(), mPos.GetY(), mPos.GetZ());
	glRotatef(-mTheta * 180/PI - 90, 0, 1, 0);
	glRotatef(-90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);

	cgGLSetStateMatrixParameter(vsModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	CheckForCgError("scrappy", "render1");

	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	mDriverParticleSystem.Render();



	glEnable(GL_LIGHTING);

	cgSetParameter1f(textShift, shift);
	if(burned) glColor3f(0, 0, 0);
	else glColor3f(1, 1, 1);
	mMeshObject.GLDraw();
	glColor3f(1, 1, 1);
	glPopMatrix();

	cgGLDisableProfile(vsScrappyProfile);
	cgGLDisableProfile(psScrappyProfile);

	CheckForCgError("scrappy", "render");

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
