#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <stdio.h>

#include "../include/app.h"
#include "../include/input.h"
#include "../include/vector.h"

int Input_do(App *app){
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        switch(event.type) {
            case SDL_QUIT:
            return 1;
            default: break;
        }
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        Vec3d vForward = Vector_mult(app->vLookDir, 4.0f * app->fElapsedTime);


        // This affects position. Make it affect velocity... somehow
        if(state[SDL_SCANCODE_UP])
            app->vCamera.y -= 8.0f * app->fElapsedTime;
        if(state[SDL_SCANCODE_DOWN])
            app->vCamera.y += 8.0f * app->fElapsedTime;
        if(state[SDL_SCANCODE_RIGHT])
            app->vCamera.x += 8.0f * app->fElapsedTime;
        if(state[SDL_SCANCODE_LEFT])
            app->vCamera.x -= 8.0f * app->fElapsedTime;

        if(state[SDL_SCANCODE_W])
            app->vCamera = Vector_add(app->vCamera, vForward);
        if(state[SDL_SCANCODE_S])
            app->vCamera = Vector_sub(app->vCamera, vForward);
        if(state[SDL_SCANCODE_A])
            app->fYaw -= 1.0f * app->fElapsedTime;
        if(state[SDL_SCANCODE_D])
            app->fYaw += 1.0f * app->fElapsedTime;
    }

    return 0;
}
