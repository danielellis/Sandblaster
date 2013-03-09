#ifndef MATERIALS_H
#define MATERIALS_H

#include <stdlib.h>
#include <SDL/SDL_opengl.h>

struct Material {
   float mDiffuse[4];
   float mAmbient[4];
   float mSpecular[4];
   float mShininess[1];
};

enum {RED_MAT = 0, GREEN_MAT = 1, GROUND_MAT = 2};

static Material mats[] = 
{
// red
{{1.0f, 0.0f, 0.0f, 1.0f}, {0.8f, 0.1f, 0.1f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f}},
// green
{{0.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.8f, 0.1f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f}},
// black
{{0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f}}
};

void SetMaterial(int i);

#endif