#ifndef AI_H
#define AI_H

#define	_TOL 1e-10

#include "Vector3.h"

class Driver;

class AI {
public:
    AI(Driver *driver) : mAIDriver(driver) {
		TargetCooldown = 21;
		AmmoCooldown = 0;
	}
    void MakeDecision(Driver *);
	void Chase(Driver *);
	void Evade(Driver *);
	

private:
    Driver *mAIDriver;
	Vector3	VRotate2D( float , Vector3);
	int TargetCooldown;
	int AmmoCooldown;
	Driver *currentTarget;
	Driver *otherDrivers[5];
	
};

#endif