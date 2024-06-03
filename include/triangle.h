#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "misc.h"

typedef struct {
    Vec3d points[3];
    Color col;
} Triangle;

#endif
