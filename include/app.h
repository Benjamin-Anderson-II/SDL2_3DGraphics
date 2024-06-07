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
    Mat4x4 mView;
    float  fElapsedTime;

    // CAMERA
    Mat4x4 mCamera;  //translation matrix
    Vec3d vCamera;   //position
    Vec3d vLookDir;  //position
    Vec3d vVelocity; //velocity
    float fYaw;      //position
    Vec3d vRotation; //velocity

    float *pixelDepthBuffer;
} App;

#endif
