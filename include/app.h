#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include "vector.h"
#include "matrix.h"
#include "dynarr.h"

typedef struct {
    SDL_Window *window;
    SDL_Surface *surface;

    DynArr *meshes;
    Mat4x4 mProj;
    Mat4x4 mCamera;
    float fElapsedTime;

    Vec3d vCamera;
    Vec3d vLookDir;
    float fYaw;
} App;

#endif
