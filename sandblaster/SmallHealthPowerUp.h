#ifndef SMALLHEALTHPOWERUP_H
#define SMALLHEALTHPOWERUP_H

#include <math.h>
#include "Vector3.h"
#include "TextureLoader.h"

class SmallHealthPowerUp
{
public:
	void Initialize(Vector3 p, float s);
	void Update(float);
	void Render(void);
	Vector3 GetPos(void);
	float GetBoundsRadius(void);
	void drawcube(void);
	bool IsCollided(Vector3 pos, float radius);
	bool isActive() {return active;}

protected:
	Vector3 position;
	float scale;
	float standingAngle;
	float mTheta;
	float resetCounter;
	bool active;
	static TextureLoader textureLoader;
	static glTexture health;
};

#endif
