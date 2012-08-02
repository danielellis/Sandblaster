#include <SDL/SDL_opengl.h>
#include "Frustum.h"

void Frustum::Update() {
	float md[16], proj[16], clip[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, md);
	glGetFloatv(GL_PROJECTION_MATRIX, proj);

	clip[0] = md[0] * proj[0] + md[1] * proj[4] + md[2] * proj[8]  + md[3] * proj[12];
	clip[1] = md[0] * proj[1] + md[1] * proj[5] + md[2] * proj[9]  + md[3] * proj[13];
	clip[2] = md[0] * proj[2] + md[1] * proj[6] + md[2] * proj[10] + md[3] * proj[14];
	clip[3] = md[0] * proj[3] + md[1] * proj[7] + md[2] * proj[11] + md[3] * proj[15];

	clip[4] = md[4] * proj[0] + md[5] * proj[4] + md[6] * proj[8]  + md[7] * proj[12];
	clip[5] = md[4] * proj[1] + md[5] * proj[5] + md[6] * proj[9]  + md[7] * proj[13];
	clip[6] = md[4] * proj[2] + md[5] * proj[6] + md[6] * proj[10] + md[7] * proj[14];
	clip[7] = md[4] * proj[3] + md[5] * proj[7] + md[6] * proj[11] + md[7] * proj[15];

	clip[8]  = md[8] * proj[0] + md[9] * proj[4] + md[10] * proj[8]  + md[11] * proj[12];
	clip[9]  = md[8] * proj[1] + md[9] * proj[5] + md[10] * proj[9]  + md[11] * proj[13];
	clip[10] = md[8] * proj[2] + md[9] * proj[6] + md[10] * proj[10] + md[11] * proj[14];
	clip[11] = md[8] * proj[3] + md[9] * proj[7] + md[10] * proj[11] + md[11] * proj[15];

	clip[12] = md[12] * proj[0] + md[13] * proj[4] + md[14] * proj[8]  + md[15] * proj[12];
	clip[13] = md[12] * proj[1] + md[13] * proj[5] + md[14] * proj[9]  + md[15] * proj[13];
	clip[14] = md[12] * proj[2] + md[13] * proj[6] + md[14] * proj[10] + md[15] * proj[14];
	clip[15] = md[12] * proj[3] + md[13] * proj[7] + md[14] * proj[11] + md[15] * proj[15];

	// Calculate the right side of the Planes.
    Planes[0][0] = clip[3]  - clip[0];
	Planes[0][1] = clip[7]  - clip[4];
	Planes[0][2] = clip[11] - clip[8];
	Planes[0][3] = clip[15] - clip[12];

	// Calculate the left side of the Planes.
	Planes[1][0] = clip[3]  + clip[0];
	Planes[1][1] = clip[7]  + clip[4];
	Planes[1][2] = clip[11] + clip[8];
	Planes[1][3] = clip[15] + clip[12];

	// Calculate the bottom side of the Planes.
	Planes[2][0] = clip[3]  + clip[1];
	Planes[2][1] = clip[7]  + clip[5];
	Planes[2][2] = clip[11] + clip[9];
	Planes[2][3] = clip[15] + clip[13];

	// Calculate the top side of the Planes.
	Planes[3][0] = clip[3]  - clip[1];
	Planes[3][1] = clip[7]  - clip[5];
	Planes[3][2] = clip[11] - clip[9];
	Planes[3][3] = clip[15] - clip[13];

	// Calculate the far side of the Planes.
	Planes[4][0] = clip[3]  - clip[2];
	Planes[4][1] = clip[7]  - clip[6];
	Planes[4][2] = clip[11] - clip[10];
	Planes[4][3] = clip[15] - clip[14];

	// Calculate the near side of the Planes.
	Planes[5][0] = clip[3]  + clip[2];
	Planes[5][1] = clip[7]  + clip[6];
	Planes[5][2] = clip[11] + clip[10];
	Planes[5][3] = clip[15] + clip[14];
}