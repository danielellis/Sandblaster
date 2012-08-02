#include "WeaponPowerUp.h"
#include "TextureLoader.h"

TextureLoader WeaponPowerUp::textureLoader;
glTexture WeaponPowerUp::weapon;

void WeaponPowerUp::Initialize(Vector3 p, float s)
{
	textureLoader.LoadTextureFromDisk("HUD/weapon_generic.tga", &weapon);
	
	standingAngle = 45.0;
	mTheta = 0.0;
	position = p;
	scale = s;
	resetCounter = 0;
	active = true;
}

void WeaponPowerUp::Update(float t)
{
	mTheta += t*5.0f;

	if (!active)
		resetCounter+=t;
	
	if (resetCounter > 25.0)
	{
		if (active)
			resetCounter = 0;
		else
		{
			resetCounter = 0;
			active = true;
		}
	}
}

void WeaponPowerUp::Render(void)
{
	glPushMatrix();

	glTranslatef(position.GetX(), position.GetY(), position.GetZ());
	glRotatef(mTheta*5.0f, 0.0, 1.0, 0.0);
	glTranslatef(-position.GetX(), -position.GetY(), -position.GetZ());

	glPushMatrix();
	glTranslatef(position.GetX(), position.GetY(), position.GetZ());
	glTranslatef(0.0, sin(mTheta), 0.0);
	glRotatef(standingAngle, 0.0, 0.0, 1.0);
	glRotatef(standingAngle, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, weapon.TextureID);
	drawcube();
	glPopMatrix();
	
	glPopMatrix();
}

//original texture mapped cube drawing
void WeaponPowerUp::drawcube(void) {

  glScalef(scale, scale, scale);

  glEnable(GL_TEXTURE_2D);

  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, 0.5, 0.5); //v5
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, 0.5, 0.5); //v6
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, -0.5, 0.5); //v7
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, -0.5, 0.5); //v8
  glEnd();
  
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, -0.5, -0.5); //v1
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, -0.5, -0.5); //v2
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, 0.5, -0.5); //v3
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, 0.5, -0.5); //v4
  glEnd();
  
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, -0.5, -0.5); //v1
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-0.5, 0.5, -0.5); //v4
  glTexCoord2f(1.0, 1.0);
  glVertex3f(-0.5, 0.5, 0.5); //v5
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, -0.5, 0.5); //v8
  glEnd();
  
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, 0.5, -0.5); //v4
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, 0.5, -0.5); //v3
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, 0.5, 0.5); //v6
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, 0.5, 0.5); //v5
  glEnd();
  
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(0.5, 0.5, -0.5); //v3
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, -0.5, -0.5); //v2
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, -0.5, 0.5); //v7
  glTexCoord2f(1.0, 0.0);
  glVertex3f(0.5, 0.5, 0.5); //v6
  glEnd();
  
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, -0.5, -0.5); //v1
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, -0.5, -0.5); //v2
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, -0.5, 0.5); //v7
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, -0.5, 0.5); //v8
  glEnd();
  
  glDisable(GL_TEXTURE_2D);
}

Vector3 WeaponPowerUp::GetPos(void)
{
	return position;
}

float WeaponPowerUp::GetBoundsRadius(void)
{
	float diagonal = scale*scale+scale*scale;
	return sqrt(diagonal*diagonal+diagonal*diagonal)/2.0f;
}

bool WeaponPowerUp::IsCollided(Vector3 pos, float radius) 
{
	Vector3 separation = position - pos;

	if (separation.DotProduct(separation) < (GetBoundsRadius() + radius) * (GetBoundsRadius() + radius))
	{
		active = false;
		return true;
	}
	else
		return false;
}
