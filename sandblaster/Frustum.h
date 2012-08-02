#ifndef FRUSTUM_H
#define FRUSTUM_H

struct Frustum {
	float Planes[6][4];

	void Update();
};

#endif