#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "misc.h"
#include <stdint.h>

typedef struct {
    Vec3d points[3];
    Color col;
} Triangle;

Triangle Triangle_addVec(Triangle t, Vec3d v);
Triangle Triangle_addTri(Triangle t1, Triangle t2);
Triangle Triangle_subVec(Triangle t, Vec3d v);
Triangle Triangle_subTri(Triangle t1, Triangle t2);

Triangle Triangle_mult(Triangle t, Vec3d v);
Triangle Triangle_div(Triangle t, Vec3d v);
Triangle Triangle_normalize(Triangle t);

uint8_t Triangle_clipAgainstPlane(Vec3d pointOnPlane, Vec3d planeNormal, Triangle *triToClip, Triangle *outputTri1, Triangle *outputTri2);
Vec3d Triangle_getNormal(Triangle t);

#endif
