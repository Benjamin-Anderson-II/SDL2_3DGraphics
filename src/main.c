/**
 * TODO:
 *   
 *   *Actually* fix the mirrored XY issue (dread) [currently negating xy's in the file_importer and input and fYaw]
 *   Implement z-buffering
 *
 *
 *    <- near the end ->
 *   Implement parallelism GPU (OpenCL) and CPU (OpenMP)
 *   Refactor the code such that that it can become an engine, rather than a program
*/


#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/file_importer.h"
#include "../include/draw.h"
#include "../include/main.h"
#include "../include/input.h"
#include "../include/triangle.h"

#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720
#define DELAY           16

int main(int argc, const char *argv[]){
    App app;
    struct timespec start, stop;

    if(initApp(&app, SCREEN_WIDTH, SCREEN_HEIGHT) < 0)
        return EXIT_FAILURE;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    while(1){
        app.fElapsedTime = getElapsedTime(&start, &stop);

        if(Input_do(&app)) goto exit;
        prepareScene(&app);
        presentScene(&app, DELAY);
    }

exit:;
    cleanUp(app);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

int initApp(App *app, int screenWidth, int screenHeight){
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
    MESH app->meshes = DynArr_new(8);

    // add mesh to meshes
    Mesh *mesh = Mesh_new(64);
    if(read_objAsMesh("objects/axis.obj", mesh))
        return -1;
    DynArr_add(app->meshes, (dynarr_u)mesh);

    // create projection matrix
    app->mProj = Matrix_makeProjection(90.0f, 
                                    (float)screenHeight / (float)screenWidth,
                                    0.1f,
                                    1000.0f);

    // Set up the camera
    app->vCamera = Vector_new();

    Vec3d up = { 0,1,0,1 };
    Vec3d target = { 0,0,1,1 };
    Mat4x4 matCameraRot = Matrix_makeRotationY(app->fYaw);
    app->vLookDir = Matrix_multVector(target, matCameraRot);
    app->mCamera = Matrix_pointAt(app->vCamera, target, up);

    app->pixelDepthBuffer = malloc(screenWidth*screenHeight * sizeof(float));
    app->screen_height = screenHeight;
    app->screen_width  = screenWidth;

    fprintf(stderr, "APP INITIALIZED\n");
    return 0;
}

void resetScene(App *app){
    // clear the screen
    SDL_FillRect(app->surface, NULL, 0xa0d9ef);
    // empty the depth buffer
    for(int i = 0; i < app->screen_width*app->screen_height; i++)
        app->pixelDepthBuffer[i] = 0.0f;
}

Mat4x4 createViewMatrix(App *app){
    Vec3d up = { 0,1,0,1 };
    Vec3d target = { 0,0,1,1 };
    app->fYaw += app->vRotation.y;
    Mat4x4 matCameraRot = Matrix_makeRotationY(-app->fYaw);
    app->vLookDir = Matrix_multVector(target, matCameraRot);
    app->vCamera = Vector_add(app->vCamera, app->vVelocity);
    target = Vector_add(app->vCamera, app->vLookDir);

    app->mCamera = Matrix_pointAt(app->vCamera, target, up);

    return Matrix_TransRotInverse(app->mCamera);
}

Color getColor(Vec3d normal){
    Vec3d lightDirection = {0.0f, 0.0f, -1.0f};
    float dp;
    int col;

    lightDirection = Vector_normalize(lightDirection);
    dp = Vector_dotProd(normal, lightDirection);
    col = 255 * ((dp + 1) / 2);
    return (Color){ col, col, col, 255 };
}

Mesh *transformMesh(App *app, Mesh *m){
    Mesh *transformed = Mesh_new(64);
    for(int i = 0; i < m->size; i++){
        Triangle tri = Mesh_get(m, i);
        Triangle triProjected, triTransformed, triViewed;

        // Transform Triangle
        triTransformed = Matrix_multTriangle(tri, app->mWorld);

        // Calculate the Triangle Normal
        Vec3d normal = Triangle_getNormal(triTransformed);

        // Get Ray from Triangle to Camera
        Vec3d vCameraRay = Vector_new(); // for w=1
        vCameraRay = Vector_sub(triTransformed.points[0], app->vCamera);

        // Skip triangle if normal faces the wrong way
        if(Vector_dotProd(normal, vCameraRay) >= 0.0f) continue;

        // Adjust triangle locations for viewing
        triViewed = Matrix_multTriangle(triTransformed, app->mView);
        triViewed.col = getColor(normal);

        int numClippedTris = 0;
        Triangle clipped[2];
        numClippedTris = Triangle_clipAgainstPlane((Vec3d){ 0.0f,0.0f,0.1f,1.0f }, (Vec3d){ 0.0f,0.0f,1.0f,1.0f }, &triViewed, &clipped[0], &clipped[1]);

        for(int c = 0; c < numClippedTris; c++){
            // Project Triangle from 3D --> 2D
            triProjected = Matrix_multTriangle(clipped[c], app->mProj);

            // normalize the coordinates
            triProjected = Triangle_normalize(triProjected);

            // Projection matrix gives results between -1 and 1
            Vec3d offsetView = {1,1,1};
            triProjected = Triangle_addVec(triProjected, offsetView); // now btwn 0 and 2

            //scale it to the appropriate size for that axis
            Vec3d adjustScreen = { (float)app->screen_width / 2, (float)app->screen_height / 2, 0.5f };
            triProjected = Triangle_mult(triProjected, adjustScreen);

            Mesh_add(transformed, triProjected);
        }
    }
    return transformed;
}

void rasterMesh(App *app, Mesh *m){
    // painter's algorithm (temporary)
    Mesh_sortInc(m);

    for(int i = 0; i < m->size; i++){
        Triangle t = Mesh_get(m, i);
        Triangle clipped[2];
        Mesh *listTriangles = Mesh_new(32);
        Mesh_add(listTriangles, t);
        int numNewTris = 1;

        // create points and normals for clipping against the screen
        //                     x                           y                            z
        Vec3d topPoint    = { 0.0f,                       0.0f,                        0.0f };
        Vec3d topNormal   = { 0.0f,                       1.0f,                        0.0f };
        Vec3d botPoint    = { 0.0f,                       (float)app->screen_height-1, 0.0f };
        Vec3d botNormal   = { 0.0f,                       -1.0f,                       0.0f };
        Vec3d leftPoint   = { 0.0f,                       0.0f,                        0.0f };
        Vec3d leftNormal  = { 1.0f,                       0.0f,                        0.0f };
        Vec3d rightPoint  = { (float)app->screen_width-1, 0.0f,                        0.0f };
        Vec3d rightNormal = { -1.0f,                      0.0f,                        0.0f };


        // clip against all 4 sides of the screen
        for(int p = 0; p < 4; p++){
            int numTrisToAdd = 0;
            // perform this for every triangle
            while(numNewTris > 0){
                Triangle test = Mesh_get(listTriangles, 0);
                Mesh_removeAt(listTriangles, 0);
                numNewTris--;

                switch(p){
                    case 0: 
                        numTrisToAdd = Triangle_clipAgainstPlane(topPoint, topNormal, &test, &clipped[0], &clipped[1]);
                    break;
                    case 1: 
                        numTrisToAdd = Triangle_clipAgainstPlane(botPoint, botNormal, &test, &clipped[0], &clipped[1]);
                    break;
                    case 2: 
                        numTrisToAdd = Triangle_clipAgainstPlane(leftPoint, leftNormal, &test, &clipped[0], &clipped[1]);
                    break;
                    case 3: 
                        numTrisToAdd = Triangle_clipAgainstPlane(rightPoint, rightNormal, &test, &clipped[0], &clipped[1]);
                    break;
                    default:break;
                }

                for(int w = 0; w < numTrisToAdd; w++)
                    Mesh_add(listTriangles, clipped[w]);
            }
            numNewTris = listTriangles->size;
        }

        for(int a = 0; a < listTriangles->size; a++){
            Triangle t = Mesh_get(listTriangles, a);
            draw_filledTriangle(app, t, t.col);
            draw_triangle(app, t, (Color){0,0,0,255});
        }
    }
}

void prepareScene(App *app){
    resetScene(app);

    app->mWorld = Matrix_makeTranslation(0.0f, 0.0f, 5.0f); // might be different in the future
    app->mView = createViewMatrix(app);

    Mesh *trisToRaster;

    for(int k = 0; k < app->meshes->size; k++){
        Mesh *m = DynArr_get(app->meshes, k).mesh;
        trisToRaster = transformMesh(app, m);
        rasterMesh(app, trisToRaster);
    }
}

void presentScene(App *app, int delay){
    SDL_UpdateWindowSurface(app->window);
    SDL_Delay(delay);
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

    // free pixelDepthBuffer
    free(app.pixelDepthBuffer);
    fprintf(stderr, "CLEANED UP\n");
}
