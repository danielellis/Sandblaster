#include "Light.h"

Light lights[] =
{GL_LIGHT0, {0.0, 5.0, 0.0, 1.0}, {0.8, 0.8, 0.8, 1.0}, {0.8, 0.8, 0.8, 1.0}, 
 {0.2, 0.2, 0.2, 1.0}};

void SetLight(int i) {
   glLightfv(lights[i].mLightnum, GL_POSITION, lights[i].mPosition);
   glLightfv(lights[i].mLightnum, GL_DIFFUSE, lights[i].mDiffuse);
   glLightfv(lights[i].mLightnum, GL_AMBIENT, lights[i].mAmbient);
   glLightfv(lights[i].mLightnum, GL_SPECULAR, lights[i].mSpecular);
}