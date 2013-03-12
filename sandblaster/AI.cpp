#include "AI.h"

#include <cmath>

#include "Vector3.h"
#include "Driver.h"


void AI::MakeDecision(Driver *targetDriver) {


	Vector3 toObj;

	//cant move if frozen
	if(!mAIDriver->IsFrozen()) {
		/*
		float shortestlength = 10000;

		if(TargetCooldown > 20) {
			//determine new closest target
			int i;
			
			Driver* ptr = *otherDrivers;
			for(i=5; i<5; ptr++, i++) {
				float tmp;
				if ((tmp = (ptr->GetPos() - mAIDriver->GetPos()).Length()) < shortestlength) {
					shortestlength = tmp;
					currentTarget = ptr;
				}
			}

			//FOR 1 DRIVER ONLY
			currentTarget = targetDriver;
			
			TargetCooldown = 0;
		}
		*/

		currentTarget = targetDriver;
		//printf("%d\n", AmmoCooldown);
		//adds ammo to the AI periodicly
		if(AmmoCooldown > 200) {
			//printf("in\n");
			mAIDriver->SetFireCount(mAIDriver->GetFireCount() + 8);
			AmmoCooldown = 0;
			
		}

		if(mAIDriver->GetCurrentWeaponCount() == 0) {
			mAIDriver->SetCurrentWeapon(W_FIREBALL);
		}


		toObj = currentTarget->GetPos() - mAIDriver->GetPos();

		TargetCooldown++;
		AmmoCooldown++;

		
		if (toObj.Length() > 15) {
			Chase(currentTarget);
		} else {
			Evade(currentTarget);
		}
	} else {
		mAIDriver->SetVel(0.0);
	}
	
	
}


void AI::Evade(Driver *targetDriver) {


	Vector3 toObj;
	Vector3 carDir;

	//calculate where to head based on velocity and distance
	float velocitymagnitude = (targetDriver->GetVel() - mAIDriver->GetVel()).Length();
	float timetoclose = (targetDriver->GetPos() - mAIDriver->GetPos()).Length();
	timetoclose = (velocitymagnitude != 0) ? (timetoclose/velocitymagnitude) : 0;
	Vector3 targetposition = targetDriver->GetPos() + targetDriver->GetVel() * timetoclose;

	//calculate normal vector from car to projected object
	//toObj = targetDriver->GetPos() - mAIDriver->GetPos();
	toObj = targetposition - mAIDriver->GetPos();
	toObj.SetY(0);
	toObj.Normalize();

	//calculate normal vector for direction of the car
	carDir.SetX(cos(mAIDriver->GetTheta()));
	carDir.SetZ(sin(mAIDriver->GetTheta()));

	//calculate angle between cardirection vector and toobject vector
	float angle = acos( toObj.DotProduct(carDir) );


	if(angle < 2.9) {

		if(carDir.CrossProduct(toObj).GetY() > 0) {
			//turn right
			mAIDriver->SetTheta(mAIDriver->GetTheta() + .15f);
		} else {
			//turn left
			mAIDriver->SetTheta(mAIDriver->GetTheta() - .15f);
		}
	}

	//thrust
	mAIDriver->ApplyForce(50*carDir);
	

}

void AI::Chase(Driver *targetDriver) {

	Vector3 toObj;
	Vector3 carDir;

	//calculate where to head based on velocity and distance
	Vector3 addedvelocity;
	
	addedvelocity.SetX(30*targetDriver->GetTheta());
	addedvelocity.SetZ(30*targetDriver->GetTheta());
	
	
	float velocitymagnitude = ((targetDriver->GetVel() + addedvelocity) - mAIDriver->GetVel()).Length();
	float timetoclose = (targetDriver->GetPos() - mAIDriver->GetPos()).Length();
	timetoclose = (velocitymagnitude != 0) ? (timetoclose/velocitymagnitude) : 0;
	Vector3 targetposition = targetDriver->GetPos() + (targetDriver->GetVel() * timetoclose);

	//calculate normal vector from car to projected object
	//toObj = targetDriver->GetPos() - mAIDriver->GetPos();
	toObj = targetposition - mAIDriver->GetPos();
	toObj.SetY(0);
	float distanceToObject = toObj.Length();
	toObj.Normalize();

	//calculate normal vector for direction of the car
	carDir.SetX(cos(mAIDriver->GetTheta()));
	carDir.SetZ(sin(mAIDriver->GetTheta()));

	//calculate angle between cardirection vector and toobject vector
	float angle = acos( toObj.DotProduct(carDir) );
	

	if(angle < .15) {
		//point at the target
		mAIDriver->SetTheta( atan2(toObj.GetZ(), toObj.GetX()) );
		//thrust
		mAIDriver->ApplyForce(50*carDir);
		//shoot
		mAIDriver->SetDir(mAIDriver->GetPhi(), mAIDriver->GetTheta());
		mAIDriver->FireCurrentWeapon();
	} else if(carDir.CrossProduct(toObj).GetY() > 0) {
		//turn left
		mAIDriver->SetTheta(mAIDriver->GetTheta() - .15f);
	} else {
		//turn right
		mAIDriver->SetTheta(mAIDriver->GetTheta() + .15f);
	}

}
