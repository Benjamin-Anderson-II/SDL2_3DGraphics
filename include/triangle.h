#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "misc.h"
#include <stdint.h>

typedef struct {
    Vec3d points[3];
    Color col;
} Triangle;

uint8_t Triangle_clipAgainstPlane(Vec3d pointOnPlane, Vec3d planeNormal, Triangle *triToClip, Triangle *outputTri1, Triangle *outputTri2);

#endif
