#ifndef DRAW_H
#define DRAW_H

#include "app.h"
#include "misc.h"
#include "dynarr.h"

void draw_pixel(App *app, Pixel p, Color c);
void draw_line(App *app, Pixel p1, Pixel p2, Color c);
void draw_triangle(App *app, Triangle t, Color c);
void draw_filledTriangle(App *app, Triangle t, Color c);

#endif
