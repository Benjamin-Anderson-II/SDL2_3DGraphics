#ifndef MAIN_H
#define MAIN_H

#include "app.h"
#include "matrix.h"
#include "vector.h"
#include "mesh.h"
#include <time.h>

int initApp(App *app, int screenWidth, int screenHeight);
void prepareScene(App *app);
void presentScene(App *app, int delay);
int doInput();

void cleanUp(App app);

float getElapsedTime(struct timespec *start, struct timespec *stop);

#endif
