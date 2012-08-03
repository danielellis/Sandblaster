#include "Intersection.h"
#include <vector>
#include "Vector3.h"
#include "ObjLoader.h"

using namespace std;

int
intersect_triangle(Vector3 orig, Vector3 dir,
                   Vector3 vert0, Vector3 vert1, Vector3 vert2,
                   float *t, float *u, float *v)
{
   Vector3 edge1, edge2, tvec, pvec, qvec;
   float det,inv_det;

   /* find vectors for two edges sharing vert0 */
   edge1 = vert1 - vert0;
   edge2 = vert2 - vert0;
  // SUB(edge1, vert1, vert0);
  // SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   pvec = dir.CrossProduct(edge2);
   //CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = edge1.DotProduct(pvec);
   //det = DOT(edge1, pvec);

#ifdef TEST_CULL           /* define TEST_CULL if culling is desired */
   if (det < EPSILON)
      return 0;

   /* calculate distance from vert0 to ray origin */
   //SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   *u = DOT(tvec, pvec);
   if (*u < 0.0 || *u > det)
      return 0;

   /* prepare to test V parameter */
   //CROSS(qvec, tvec, edge1);

    /* calculate V parameter and test bounds */
   //*v = DOT(dir, qvec);
   if (*v < 0.0 || *u + *v > det)
      return 0;

   /* calculate t, scale parameters, ray intersects triangle */
   //*t = DOT(edge2, qvec);
   inv_det = 1.0 / det;
   *t *= inv_det;
   *u *= inv_det;
   *v *= inv_det;
#else                    /* the non-culling branch */
   if (det > -EPSILON && det < EPSILON)
     return 0;
   inv_det = 1 / det;

   /* calculate distance from vert0 to ray origin */
   tvec = orig - vert0;
   //SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   *u = tvec.DotProduct(pvec) * inv_det;
   //*u = DOT(tvec, pvec) * inv_det;
   if (*u < 0.0 || *u > 1.0)
     return 0;

   /* prepare to test V parameter */
   qvec = tvec.CrossProduct(edge1);
 //  CROSS(qvec, tvec, edge1);

   /* calculate V parameter and test bounds */
   *v = dir.DotProduct(qvec) * inv_det;
   //*v = DOT(dir, qvec) * inv_det;
   if (*v < 0.0 || *u + *v > 1.0)
     return 0;

   /* calculate t, ray intersects triangle */
   *t = edge2.DotProduct(qvec) * inv_det;
   //*t = DOT(edge2, qvec) * inv_det;
#endif
   return 1;
}