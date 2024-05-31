#ifndef MAIN_H
#define MAIN_H

#include "misc.h"
#include "matrix.h"
#include "vector.h"
#include "mesh.h"
#include <time.h>

int initApp(App *app);
void prepareScene(App *app, Mesh meshCube, Mat4x4 matProj, Vec3d vCamera, float elapsedTime);
void presentScene(App *app);
int doInput();

float getElapsedTime(struct timespec *start, struct timespec *stop);

#endif
