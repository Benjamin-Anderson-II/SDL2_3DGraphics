#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/file_importer.h"
#include "../include/draw.h"
#include "../include/main.h"
#include "../include/input.h"

#define SCREEN_WIDTH    480
#define SCREEN_HEIGHT   480
#define DELAY           16

// TODO: remove this later
float fTheta = 0.0f;

int
main(int argc, const char *argv[]){
    App app;
    struct timespec start, stop;

    if(initApp(&app) < 0)
        return EXIT_FAILURE;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    while(1){
        app.fElapsedTime = getElapsedTime(&start, &stop);

        if(Input_do(&app)) goto exit;
        prepareScene(&app);
        presentScene(&app);
    }

exit:;
    cleanUp(app);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

int initApp(App *app){
    memset(app, 0, sizeof(App));

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "Video Initialization Error: %s\n", SDL_GetError());
        return -1;
    }
    app->window = SDL_CreateWindow("THE CUBE!!!", 
                                   SDL_WINDOWPOS_CENTERED, 
                                   SDL_WINDOWPOS_CENTERED, 
                                   SCREEN_WIDTH, 
                                   SCREEN_HEIGHT, 
                                   SDL_WINDOW_SHOWN);
    if(!app->window){
        fprintf(stderr, "Window Creation Error %s\n", SDL_GetError());
        return -1;
    }

    app->surface = SDL_GetWindowSurface(app->window);
    if(!app->surface){
        fprintf(stderr, "Failed to Create Surface: %s\n", SDL_GetError());
        return -1;
    }

    // create dynarr of meshes to raster
    app->meshes = DynArr_new(8);

    // add mesh to meshes
    Mesh *mesh = Mesh_new(64);
    if(read_objAsMesh("objects/axis.obj", mesh))
        return -1;
    DynArr_add(app->meshes, (dynarr_u)mesh);

    // create projection matrix
    app->mProj = Matrix_makeProjection(90.0f, 
                                    (float) SCREEN_HEIGHT / (float) SCREEN_WIDTH,
                                    0.1f,
                                    1000.0f);

    // Set up the camera
    app->vCamera = Vector_new();

    Vec3d up = { 0,1,0,1 };
    Vec3d target = { 0,0,1,1 };
    Mat4x4 matCameraRot = Matrix_makeRotationY(app->fYaw);
    app->vLookDir = Matrix_multVector(target, matCameraRot);
    app->mCamera = Matrix_pointAt(app->vCamera, target, up);

    fprintf(stderr, "APP INITIALIZED\n");
    return 0;
}

void prepareScene(App *app){
    // clear the screen
    SDL_FillRect(app->surface, NULL, 0x555555);

//  fTheta += 1.0f * app->fElapsedTime;

    // rotation matrices
//  Mat4x4 matRotZ = Matrix_makeRotationZ(fTheta * 0.5f); 
//  Mat4x4 matRotX = Matrix_makeRotationX(fTheta);

    Mat4x4 matTrans = Matrix_makeTranslation(0.0f, 0.0f, 5.0f);

    Mat4x4 matWorld;
//  matWorld = Matrix_multMatrix(matRotZ, matRotX);
//  matWorld = Matrix_multMatrix(matWorld, matTrans);
    matWorld = matTrans;

    Vec3d up = { 0,1,0,1 };
    Vec3d target = { 0,0,1,1 };
    Mat4x4 matCameraRot = Matrix_makeRotationY(-app->fYaw);
    app->vLookDir = Matrix_multVector(target, matCameraRot);
    target = Vector_add(app->vCamera, app->vLookDir);


    app->mCamera = Matrix_pointAt(app->vCamera, target, up);

    Mat4x4 matView = Matrix_TransRotInverse(app->mCamera);

    Mesh *vecTrianglesToRaster = Mesh_new(64);

    // Draw Triangles
    for(int k = 0; k < app->meshes->size; k++){
        Mesh *currMesh = DynArr_get(app->meshes, k).mesh;

        for(int i = 0; i < currMesh->size; i++){
            Triangle tri = Mesh_get(currMesh, i);
            Triangle triProjected, triTransformed, triViewed;

            // Transform Triangle
            triTransformed = Matrix_multTriangle(tri, matWorld);

            // Calculate the Triangle Normal
            Vec3d normal, line1, line2;
            normal = line1 = line2 = Vector_new();

                // Get the lines to either side of the triangle
            line1 = Vector_sub(triTransformed.points[1], triTransformed.points[0]);
            line2 = Vector_sub(triTransformed.points[2], triTransformed.points[0]);

                // Get the normal vector (orthogonal to the plane), then normalize it
            normal = Vector_crossProd(line1, line2);
            normal = Vector_normalize(normal);

            // Get Ray from Triangle to Camera
            Vec3d vCameraRay = Vector_new();
            vCameraRay = Vector_sub(triTransformed.points[0], app->vCamera);

            // Skip triangle if normal faces the wrong way
            if(Vector_dotProd(normal, vCameraRay) >= 0.0f)
                continue;

            // Illumination
            Vec3d lightDirection = {0.0f, 0.0f, -1.0f};
            lightDirection = Vector_normalize(lightDirection);

            float dp = Vector_dotProd(normal, lightDirection);
            int col = 255 * ((dp + 1) / 2);

            triViewed = Matrix_multTriangle(triTransformed, matView);

            // Project Triangle from 3D --> 2D
            triProjected = Matrix_multTriangle(triViewed, app->mProj);

            // normalize the coordinates
            triProjected.points[0] = Vector_div(triProjected.points[0], triProjected.points[0].w);
            triProjected.points[1] = Vector_div(triProjected.points[1], triProjected.points[1].w);
            triProjected.points[2] = Vector_div(triProjected.points[2], triProjected.points[2].w);
            triProjected.col = (Color){col, col, col, 255};

            // Scale into view
            Vec3d offsetView = {1,1,1,0};
            triProjected.points[0] = Vector_add(triProjected.points[0], offsetView);
            triProjected.points[1] = Vector_add(triProjected.points[1], offsetView);
            triProjected.points[2] = Vector_add(triProjected.points[2], offsetView);
            triProjected.points[0].x *= 0.5f * (float)SCREEN_WIDTH;
            triProjected.points[0].y *= 0.5f * (float)SCREEN_HEIGHT;
            triProjected.points[1].x *= 0.5f * (float)SCREEN_WIDTH;
            triProjected.points[1].y *= 0.5f * (float)SCREEN_HEIGHT;
            triProjected.points[2].x *= 0.5f * (float)SCREEN_WIDTH;
            triProjected.points[2].y *= 0.5f * (float)SCREEN_HEIGHT;

            Mesh_add(vecTrianglesToRaster, triProjected);
        }
    }

    Mesh_sortInc(vecTrianglesToRaster);

    for(int i = 0; i < vecTrianglesToRaster->size; i++){
        Triangle t = Mesh_get(vecTrianglesToRaster, i);
        draw_filledTriangle(app, t, t.col);
        draw_triangle(app, t, (Color){0,0,0,255});
    }
}

void presentScene(App *app){
    SDL_UpdateWindowSurface(app->window);
    SDL_Delay(DELAY);
}

float getElapsedTime(struct timespec *start, struct timespec *stop){
    clock_gettime(CLOCK_MONOTONIC_RAW, stop);

    uint64_t delta_us = (stop->tv_sec - start->tv_sec) * 1000000 + (stop->tv_nsec - start->tv_nsec) / 1000;
    *start = *stop;
    return (float)delta_us / 1000000;
}

void cleanUp(App app){
    // free all meshes
    for(int i = 0; i < app.meshes->size; i++){
        Mesh *m = DynArr_get(app.meshes, i).mesh;
        if(m->tris)
            Mesh_delete(m);
    }

    // free array of meshes
    if(app.meshes)
        DynArr_delete(app.meshes);
}
