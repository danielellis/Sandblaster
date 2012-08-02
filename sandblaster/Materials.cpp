#include "Materials.h"

void SetMaterial(int i) {
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mats[i].mDiffuse);
   glMaterialfv(GL_FRONT, GL_AMBIENT, mats[i].mAmbient);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mats[i].mSpecular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mats[i].mShininess);
}