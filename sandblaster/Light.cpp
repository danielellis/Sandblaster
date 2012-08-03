#include "Light.h"

Light lights[] =
{GL_LIGHT0, {0, 5.0f, 0, 1.0f}, {0.8f, 0.8f, 0.8f, 1.0f}, {0.8f, 0.8f, 0.8f, 1.0f}, 
 {0.2f, 0.2f, 0.2f, 1.0f}};

void SetLight(int i) {
   glLightfv(lights[i].mLightnum, GL_POSITION, lights[i].mPosition);
   glLightfv(lights[i].mLightnum, GL_DIFFUSE, lights[i].mDiffuse);
   glLightfv(lights[i].mLightnum, GL_AMBIENT, lights[i].mAmbient);
   glLightfv(lights[i].mLightnum, GL_SPECULAR, lights[i].mSpecular);
}