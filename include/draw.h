#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include "misc.h"
#include "dynarr.h"

void draw_pixel(SDL_Surface *surface, Point p, Color c);
void draw_line(SDL_Surface *surface, Point p1, Point p2, Color c);
void draw_triangle(App *app, Point p1, Point p2, Point p3, Color c);
void draw_filledTriangle(App *app, Point p1, Point p2, Point p3, Color c);

#endif
