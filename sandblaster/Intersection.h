#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <assert.h>
#include "Vector3.h"
#include "ObjLoader.h"

#define EPSILON 0.000001

int intersect_triangle(Vector3 orig, Vector3 dir,
                   Vector3 vert0, Vector3 vert1, Vector3 vert2,
                   float *t, float *u, float *v);