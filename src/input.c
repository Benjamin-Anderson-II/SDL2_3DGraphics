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
        Vec3d vForward = Vector_mult(app->vLookDir, 10.0f * app->fElapsedTime);
        // There's probably a more clever way to do this... maybe figure it out when the mouse is implemented
        float rotationAmount = 0.1f;

        // reset velocity to nothing (in case a key was un-pressed)
        app->vVelocity = Vector_new();
        app->vRotation = Vector_new();

        // This affects position. Make it affect velocity... somehow
        if(state[SDL_SCANCODE_UP])
            app->vVelocity.y = -1;
        if(state[SDL_SCANCODE_DOWN])
            app->vVelocity.y = 1;
        if(state[SDL_SCANCODE_RIGHT])
            app->vVelocity.x = 1;
        if(state[SDL_SCANCODE_LEFT])
            app->vVelocity.x = -1;

        if(state[SDL_SCANCODE_W])
            app->vVelocity = Vector_add(app->vVelocity, vForward);
        if(state[SDL_SCANCODE_S])
            app->vVelocity = Vector_sub(app->vVelocity, vForward);
        if(state[SDL_SCANCODE_A])
            app->vRotation.y = -rotationAmount;
        if(state[SDL_SCANCODE_D])
            app->vRotation.y = rotationAmount;

        // normalize movement vector
        Vector_normalize(app->vVelocity);

        // mult it by something reasonable
        app->vVelocity = Vector_mult(app->vVelocity, 0.5);
    }

    return 0;
}
