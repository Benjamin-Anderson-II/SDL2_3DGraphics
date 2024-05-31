#ifndef MISC_H
#define MISC_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Window *window;
    SDL_Surface *surface;
} App;

typedef struct { int r, g, b, a; } Color;
typedef struct { int x, y; } Point;

#endif
