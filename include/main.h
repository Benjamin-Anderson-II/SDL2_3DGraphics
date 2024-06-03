#ifndef MAIN_H
#define MAIN_H

#include "app.h"
#include "matrix.h"
#include "vector.h"
#include "mesh.h"
#include <time.h>

int initApp(App *app, float screenWidth, float screenHeight);
void prepareScene(App *app);
void presentScene(App *app);
int doInput();

void cleanUp(App app);

float getElapsedTime(struct timespec *start, struct timespec *stop);

#endif
