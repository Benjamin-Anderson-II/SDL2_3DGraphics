#include "../include/draw.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>

void _swapPixel(Pixel *a, Pixel *b){
    Pixel tmp = *b;
    *b = *a;
    *a = tmp;
}

void draw_pixel(App *app, Pixel p, Color c){
    // bounds checking
    if(p.x < 0 || p.y < 0 || p.x >= app->surface->w || p.y >= app->surface->h)
        return;

    Uint32 *pixel = (Uint32 *) ((Uint8 *)app->surface->pixels +
                                p.y * app->surface->pitch + 
                                p.x * app->surface->format->BytesPerPixel);
    *pixel = SDL_MapRGBA(app->surface->format, c.r, c.g, c.b, c.a);
}

/*DDA interpolater*/
INT DynArr *_interpolate(int i0, int d0, int i1, int d1){
    INT DynArr *values = DynArr_new(32);
    if(i0 == i1){
        DynArr_add(values, (dynarr_u)d0);
        return values;
    }
    float slope = (float)(d1 - d0) / (i1 - i0);
    float d = (float)d0;
    for(int i = i0; i < i1; i++){
        DynArr_add(values, (dynarr_u)((int)round(d)));
        d += slope;
    }
    return values;
}

/*Bresenham's Line Algorithm*/
void draw_line(App *app, Pixel start, Pixel end, Color c){
    // initial goal: get the points in a shallow, forward facing orientation
    int a0, b0, a1, b1, steep;
    // shallow
    if(abs(end.y - start.y) < abs(end.x - start.x)){
        // facing backwards
        if(start.x > end.x){
            a0 = end.x; b0 = end.y;
            a1 = start.x; b1 = start.y;
            steep = 0;
        } else {
            a0 = start.x; b0 = start.y;
            a1 = end.x; b1 = end.y;
            steep = 0;
        }

    // steep
    } else {
        // facing backwards
        if(start.y > end.y){
            a0 = end.y; b0 = end.x;
            a1 = start.y; b1 = start.x;
            steep = 1;
        } else {
            a0 = start.y; b0 = start.x;
            a1 = end.y; b1 = end.x;
            steep = 1;
        }
    }

    int da = a1 - a0;
    int db = b1 - b0;
    int inc = 1;
    if(db < 0){
        inc *= -1;
        db *= -1;
    }
    int D = 2*db - da;
    int b = b0;

    for(int a = a0; a < a1; a++){
        if(steep)
            draw_pixel(app, (Pixel){b, a}, c);
        else
            draw_pixel(app, (Pixel){a, b}, c);
        if(D > 0){
            b += inc;
            D -= 2 * da;
        }
        D += 2*db;
    }
}

void draw_triangle(App *app, Triangle t, Color c){
    Pixel p0 = (Pixel){(int)t.points[0].x, (int)t.points[0].y, t.points[0].z};
    Pixel p1 = (Pixel){(int)t.points[1].x, (int)t.points[1].y, t.points[1].z};
    Pixel p2 = (Pixel){(int)t.points[2].x, (int)t.points[2].y, t.points[2].z};
    draw_line(app, p0, p1, c);
    draw_line(app, p1, p2, c);
    draw_line(app, p2, p0, c);
}

void draw_filledTriangle(App *app, Triangle t, Color c){
    // Approx of Algo:
    // for each horizontal line y between the triangle's top and bottom
    //     compute x_left & x_right for this y
    //     draw line from x_left to x_right
    Pixel p0 = (Pixel){(int)t.points[0].x, (int)t.points[0].y, t.points[0].z};
    Pixel p1 = (Pixel){(int)t.points[1].x, (int)t.points[1].y, t.points[1].z};
    Pixel p2 = (Pixel){(int)t.points[2].x, (int)t.points[2].y, t.points[2].z};

    // Sort the points so that y0 <= y1 <= y2
    if(p1.y < p0.y) _swapPixel(&p1, &p0);
    if(p2.y < p0.y) _swapPixel(&p2, &p0);
    if(p2.y < p1.y) _swapPixel(&p2, &p1);

    // Interpolate between the two points (inverted) and store the values in dynamic arrays
    INT DynArr *x01 = _interpolate(p0.y, p0.x, p1.y, p1.x);
    INT DynArr *x12 = _interpolate(p1.y, p1.x, p2.y, p2.x);
    INT DynArr *x02 = _interpolate(p0.y, p0.x, p2.y, p2.x);

    // Last Value of x01 == First Value of x12
    if(x01->size + x12->size > x02->size)
        x01->size--;
    DynArr_merge(x01, x12);

    // FIGURE OUT WHICH IS LEFT AND WHICH IS RIGHT
    // get middle horizontal line (triangles are always convex)
    INT DynArr *xLeft;
    INT DynArr *xRight;
    int midLine = x02->size / 2;
    if(DynArr_get(x02, midLine).integer < DynArr_get(x01, midLine).integer) {
        xLeft = x02;
        xRight = x01;
    } else {
        xLeft = x01;
        xRight = x02;
    }

    if(SDL_MUSTLOCK(app->surface))
        SDL_LockSurface(app->surface);
    // Draw the Pixels to the screen
    for(int y = p0.y; y < p2.y; y++){
        for(int x = DynArr_get(xLeft, y - p0.y).integer; x < DynArr_get(xRight, y - p0.y).integer; x++){
            draw_pixel(app, (Pixel){x, y}, c);
        }
    }
    if(SDL_MUSTLOCK(app->surface))
        SDL_UnlockSurface(app->surface);

    // Free Memory
    DynArr_delete(x01);
    DynArr_delete(x02);
}
