#include "Skydome.h"
#include "TextureLoader.h"

//Profiles
//CGprofile Fireball::vsFireballProfile;
CGprofile Skydome::psSkydomeProfile;
CGprofile Skydome::psSkydomeSunProfile;

//Programs
//CGprogram Fireball::vsFireballProgram;
CGprogram Skydome::psSkydomeProgram;
CGprogram Skydome::psSkydomeSunProgram;

CGparameter Skydome::texcoord0, Skydome::dayTime, Skydome::transition, Skydome::suntexcoord0;

//Texture parameters 
CGparameter Skydome::text1, Skydome::text2, Skydome::text3, Skydome::suntext1, Skydome::suntext2;

void Skydome::InitializeSkydomeShaders() {
    TextureLoader textureLoader;
    glTexture daySky, nightSky, redSky, sun, sunMask;

    textureLoader.LoadTextureFromDisk("skytextures/clouds2.tga", &daySky);
    textureLoader.LoadTextureFromDisk("skytextures/night.tga", &nightSky);
	textureLoader.LoadTextureFromDisk("skytextures/red.tga", &redSky);

    /*
    //Vertext Shader profile
    vsSkydomeProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
    //Set profile
    cgGLSetOptimalOptions(vsFireballProfile);
    CheckForCgError("skydome", "selecting vertex profile");

    //Load vertex shader
    vsFireballProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "vertexshaders/vs_fireball.cg", vsFireballProfile, "vertMain", 0);

    CheckForCgError("skydome", "creating vertex program from file");
    cgGLLoadProgram(vsFireballProgram);
    CheckForCgError("skydome", "loading vertex program");

    
    texVert			= cgGetNamedParameter(vsFireballProgram, "texCoordIN");
    position		= cgGetNamedParameter(vsFireballProgram, "posIN");
    move			= cgGetNamedParameter(vsFireballProgram, "move");
    mvMatrix		= cgGetNamedParameter(vsFireballProgram, "ModelViewProj");
    */
    
    /////////////////////////////
    //Pixel Shader
	  
    psSkydomeProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(psSkydomeProfile);
    CheckForCgError("skydome", "selecting Skydome fragment profile");

    psSkydomeProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "pixelshaders/ps_skydome.cg", psSkydomeProfile, "pixelMain", 0);
    CheckForCgError("skydome", "creating fragment program from file");
    cgGLLoadProgram(psSkydomeProgram);
    CheckForCgError("skydome", "loading fragment program");

    texcoord0       = cgGetNamedParameter(psSkydomeProgram, "texcoord0");
    dayTime       = cgGetNamedParameter(psSkydomeProgram, "lightdist");
	transition       = cgGetNamedParameter(psSkydomeProgram, "transition");

    text1 = cgGetNamedParameter(psSkydomeProgram, "text1");
    cgGLSetTextureParameter(text1, daySky.TextureID);
    text2 = cgGetNamedParameter(psSkydomeProgram, "text2");
    cgGLSetTextureParameter(text2, nightSky.TextureID);
	text3 = cgGetNamedParameter(psSkydomeProgram, "text3");
    cgGLSetTextureParameter(text3, redSky.TextureID);
    CheckForCgError("skydome", "setting decal 2D texture");

	textureLoader.LoadTextureFromDisk("skytextures/sun.tga", &sun);
	textureLoader.LoadTextureFromDisk("skytextures/sunmask.tga", &sunMask);

	psSkydomeSunProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(psSkydomeSunProfile);
    CheckForCgError("skydome", "selecting Skydome sun fragment profile");

	psSkydomeSunProgram = cgCreateProgramFromFile(ShaderContext, CG_SOURCE, "pixelshaders/ps_skydomeSun.cg", psSkydomeSunProfile, "pixelMain", 0);
    CheckForCgError("skydome", "creating fragment program from file");
    cgGLLoadProgram(psSkydomeSunProgram);
    CheckForCgError("skydome", "loading fragment program");

	texcoord0       = cgGetNamedParameter(psSkydomeSunProgram, "texcoord0");

	suntext1 = cgGetNamedParameter(psSkydomeSunProgram, "text1");
    cgGLSetTextureParameter(suntext1, sun.TextureID);
    suntext2 = cgGetNamedParameter(psSkydomeSunProgram, "text2");
    cgGLSetTextureParameter(suntext2, sunMask.TextureID);
	CheckForCgError("skydome", "setting decal 2D texture");

    //Temporarily Unbind Texture
    //glDisable(GL_TEXTURE_2D);	
    //glDisable(GL_BLEND);						// Disable Blending
    //glBindTexture(GL_TEXTURE_2D,0);	
}

void Skydome::Initialize(float radius, float dtheta, float dphi, float hTile, float vTile)
{
	rotTheta = 0.0;
    mDayTime = 0.0;
    mTimeDir = true;
	stage = 0;

	mSunPos = Vector3(120.0, 0.0, 0.0); // change values later
	mSunPhi = 0.0;
	
	int theta, phi;
	/*
	// Make sure our vertex array is clear
	if (Vertices) 
	{
		delete Vertices;
		Vertices = NULL;
		NumVertices = 0;
	}
	*/
	// Initialize our Vertex array
	NumVertices = (int)((360/dtheta)*(90/dphi)*4);
	Vertices = new VERTEX[NumVertices];
	ZeroMemory(Vertices, sizeof(VERTEX)*NumVertices);

	// Used to calculate the UV coordinates
	float vx, vy, vz, mag;

	// Generate the dome
	int n = 0;
	for (phi=0; phi <= 90 - dphi; phi += (int)dphi)
	{
		for (theta=0; theta <= 360 - dtheta; theta += (int)dtheta)
		{
			// Calculate the vertex at phi, theta
			Vertices[n].x = radius * sinf(phi*DTOR) * cosf(DTOR*theta);
			Vertices[n].y = radius * sinf(phi*DTOR) * sinf(DTOR*theta);
			Vertices[n].z = radius * cosf(phi*DTOR);

			// Create a vector from the origin to this vertex
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			// Normalize the vector
			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			// Calculate the spherical texture coordinates
			Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
			n++;

			// Calculate the vertex at phi+dphi, theta
			Vertices[n].x = radius * sinf((phi+dphi)*DTOR) * cosf(theta*DTOR);
			Vertices[n].y = radius * sinf((phi+dphi)*DTOR) * sinf(theta*DTOR);
			Vertices[n].z = radius * cosf((phi+dphi)*DTOR);
			
			// Calculate the texture coordinates
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
			n++;

			// Calculate the vertex at phi, theta+dtheta
			Vertices[n].x = radius * sinf(DTOR*phi) * cosf(DTOR*(theta+dtheta));
			Vertices[n].y = radius * sinf(DTOR*phi) * sinf(DTOR*(theta+dtheta));
			Vertices[n].z = radius * cosf(DTOR*phi);
			
			// Calculate the texture coordinates
			vx = Vertices[n].x;
			vy = Vertices[n].y;
			vz = Vertices[n].z;

			mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
			vx /= mag;
			vy /= mag;
			vz /= mag;

			Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
			Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
			n++;

			if (phi > -90 && phi < 90)
			{
				// Calculate the vertex at phi+dphi, theta+dtheta
				Vertices[n].x = radius * sinf((phi+dphi)*DTOR) * cosf(DTOR*(theta+dtheta));
				Vertices[n].y = radius * sinf((phi+dphi)*DTOR) * sinf(DTOR*(theta+dtheta));
				Vertices[n].z = radius * cosf((phi+dphi)*DTOR);
				
				// Calculate the texture coordinates
				vx = Vertices[n].x;
				vy = Vertices[n].y;
				vz = Vertices[n].z;

				mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz));
				vx /= mag;
				vy /= mag;
				vz /= mag;

				Vertices[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
				Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
				n++;
			}
		}
	}

	// Fix the problem at the seam
	for (int i=0; i < NumVertices-3; i++)
	{
		if (Vertices[i].u - Vertices[i+1].u > 0.9f)
			Vertices[i+1].u += 1.0f;

		if (Vertices[i+1].u - Vertices[i].u > 0.9f)
			Vertices[i].u += 1.0f;

		if (Vertices[i].u - Vertices[i+2].u > 0.9f)
			Vertices[i+2].u += 1.0f;

		if (Vertices[i+2].u - Vertices[i].u > 0.9f)
			Vertices[i].u += 1.0f;

		if (Vertices[i+1].u - Vertices[i+2].u > 0.9f)
			Vertices[i+2].u += 1.0f;

		if (Vertices[i+2].u - Vertices[i+1].u > 0.9f)
			Vertices[i+1].u += 1.0f;

		if (Vertices[i].v - Vertices[i+1].v > 0.8f)
			Vertices[i+1].v += 1.0f;

		if (Vertices[i+1].v - Vertices[i].v > 0.8f)
			Vertices[i].v += 1.0f;

		if (Vertices[i].v - Vertices[i+2].v > 0.8f)
			Vertices[i+2].v += 1.0f;

		if (Vertices[i+2].v - Vertices[i].v > 0.8f)
			Vertices[i].v += 1.0f;

		if (Vertices[i+1].v - Vertices[i+2].v > 0.8f)
			Vertices[i+2].v += 1.0f;

		if (Vertices[i+2].v - Vertices[i+1].v > 0.8f)
			Vertices[i+1].v += 1.0f;
	}
}

int Skydome::Render()
{
    cgGLEnableProfile(psSkydomeProfile);
    cgGLBindProgram(psSkydomeProgram);

    cgSetParameter1f(dayTime, mDayTime);
	cgSetParameter1f(transition, stage);

    cgGLEnableTextureParameter(text1);
    cgGLEnableTextureParameter(text2);
	cgGLEnableTextureParameter(text3);

	glPushMatrix();
	//glTranslatef(0.0f, -100.0f, 0.0f);
	glRotatef(rotTheta,0.0f, 1.0f, 0.0f);
	glRotatef(270, 1.0f, 0.0f, 0.0f);

	glEnable(GL_TEXTURE_2D);

	//glBindTexture(GL_TEXTURE_2D, cloudTexture.TextureID);

	glBegin(GL_TRIANGLE_STRIP);

	for (int i=0; i < NumVertices; i++)
	{
		glColor3f(1.0f, 1.0f, 1.0f);

		glTexCoord2f(Vertices[i].u, Vertices[i].v);
		glVertex3f(Vertices[i].x, Vertices[i].y, Vertices[i].z);
	}

	glEnd();

	glPopMatrix();

	cgGLDisableTextureParameter(text1);
	cgGLDisableTextureParameter(text2);
	cgGLDisableTextureParameter(text3);

    cgGLDisableProfile(psSkydomeProfile);

	
	cgGLEnableProfile(psSkydomeSunProfile);
    cgGLBindProgram(psSkydomeSunProgram);

	cgGLEnableTextureParameter(suntext1);
    cgGLEnableTextureParameter(suntext2);

	/*glPushMatrix();
	glRotatef(mSunPhi-90.0, 0.0, 0.0, 1.0);
	//glTranslatef(-mSunPos.GetX(), -mSunPos.GetY(), 0.0);*/

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslatef(mSunPos.GetX(), mSunPos.GetY(), 0.0);
	//glTranslatef(0.0, 1.0, 0.0);
	glRotatef(60*mSunPhi+90.0, 0.0, 0.0, 1.0);
	glScalef(10.0, 10.0, 10.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1.0, 0.0, 1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-1.0, 0.0, -1.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(1.0, 0.0, -1.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(1.0, 0.0, 1.0);
	glEnd();
	glPopMatrix();

	//glPopMatrix();

	cgGLDisableTextureParameter(suntext1);
	cgGLDisableTextureParameter(suntext2);

	cgGLDisableProfile(psSkydomeSunProfile);
	
    return 1;
}

void Skydome::ReleaseDome()
{
	if (Vertices)
	{
		delete Vertices;
		Vertices = NULL;
	}
}

void Skydome::Update(float t)
{
	//rotTheta += t/5.0;
    rotTheta += t;

    if (mTimeDir)
        mDayTime += t*.01;
    else
        mDayTime -= t*.01;

    if ((mDayTime > 1.0f) && (stage == 1)) {
        mDayTime = 1.0f;
        mTimeDir = false;
    }
	else if((mDayTime > 1.0f) && (stage == 0)) {
		stage = 1;
		mDayTime = 0.0f;
	}
    if ((mDayTime < 0.0f) && (stage == 1)){
        mDayTime = 1.0f;
		stage = 0;
    }
	else if((mDayTime < 0.0f) && (stage == 0)) {
		mDayTime = 0.0f;
		mTimeDir = true;
	}

	//mSunPhi += t/5.0;
	mSunPhi += t/50.0;
	mSunPos.SetX(cos(mSunPhi)*120.0);
	mSunPos.SetY(sin(mSunPhi)*120.0);
}

Vector3 Skydome::getSunPos()
{
   return mSunPos;
}