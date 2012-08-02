#ifndef LIGHTS_H
#define LIGHTS_H

// Must include stdlib.h before glut.h in VS7 and VS8
#include <stdlib.h>
#include <SDL/SDL_opengl.h>

struct Light {
   GLenum mLightnum;
   float mPosition[4];
   float mDiffuse[4];
   float mAmbient[4];
   float mSpecular[4];
};

Light lights[];

void SetLight(int i);

#endif