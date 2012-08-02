#include "Entity.h"
#include "Vector3.h"
#include "SBLib.h"
#include <cmath>
#include <iostream>

using namespace std;

void Entity::SetDir(float phi, float theta) {
   mDir.SetX(cos(phi) * cos(theta));
   mDir.SetY(sin(phi));
   mDir.SetZ(cos(phi) * (float)cos(PI/2.0f - theta));
}

void Entity::SetCamDir(float phi, float theta) {
   camDir.SetX(cos(phi) * cos(theta));
   camDir.SetY(sin(phi));
   camDir.SetZ(cos(phi) * (float)cos(PI/2.0f - theta));
}