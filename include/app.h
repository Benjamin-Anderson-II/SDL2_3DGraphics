#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include "vector.h"
#include "matrix.h"
#include "dynarr.h"

typedef struct {
    int screen_height;
    int screen_width;

    SDL_Window  *window;
    SDL_Surface *surface;

    MESH DynArr *meshes;

    Mat4x4 mWorld;
    Mat4x4 mProj;
    Mat4x4 mCamera;
    Mat4x4 mView;
    float  fElapsedTime;

    Vec3d vCamera;
    Vec3d vLookDir;
    float fYaw;

    float *pixelDepthBuffer;
} App;

#endif
