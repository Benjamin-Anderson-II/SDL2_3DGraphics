#include "../include/draw.h"

void _swapInt(int *a, int *b){
    int tmp = *b;
    *b = *a;
    *a = tmp;
}

void _swapPoint(Point *a, Point *b){
    Point tmp = *b;
    *b = *a;
    *a = tmp;
}

void draw_pixel(SDL_Surface *surface, Point p, Color c){
    if(p.x < 0 || p.y < 0 || p.x >= surface->w || p.y >= surface->h)
        return;
    Uint32 *pixel = (Uint32 *) ((Uint8 *)surface->pixels +
                                p.y * surface->pitch + 
                                p.x * surface->format->BytesPerPixel);
    *pixel = SDL_MapRGBA(surface->format, c.r, c.g, c.b, c.a);
}

DynArr *_interpolate(int i0, int d0, int i1, int d1){
    DynArr *values = DynArr_new(32);
    if(i0 == i1){
        DynArr_add(values, (dynarr_u)d0);
        return values;
    }
    float a = (float)(d1 - d0) / (i1 - i0);
    float d = (float)d0;
    for(int i = i0; i < i1; i++){
        DynArr_add(values, (dynarr_u)((int)d));
        d += a;
    }
    return values;
}

/*Bresenham's Line Algorithm*/
void draw_line(SDL_Surface *surface, Point p0, Point p1, Color c){
    int a0, b0, a1, b1, isHigh;
    // shallow
    if(abs(p1.y - p0.y) < abs(p1.x - p0.x)){
        // facing backwards
        if(p0.x > p1.x){
            a0 = p1.x; b0 = p1.y;
            a1 = p0.x; b1 = p0.y;
            isHigh = 0;
        } else {
            a0 = p0.x; b0 = p0.y;
            a1 = p1.x; b1 = p1.y;
            isHigh = 0;
        }

    // steep
    } else {
        // facing backwards
        if(p0.y > p1.y){
            a0 = p1.y; b0 = p1.x;
            a1 = p0.y; b1 = p0.x;
            isHigh = 1;
        } else {
            a0 = p0.y; b0 = p0.x;
            a1 = p1.y; b1 = p1.x;
            isHigh = 1;
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
        if(isHigh)
            draw_pixel(surface, (Point){b, a}, c);
        else
            draw_pixel(surface, (Point){a, b}, c);
        if(D > 0){
            b += inc;
            D += 2 * (db - da);
        } else {
            D += 2*db;
        }
    }
}

void draw_triangle(App *app, Point p1, Point p2, Point p3, Color c){
    draw_line(app->surface, p1, p2, c);
    draw_line(app->surface, p2, p3, c);
    draw_line(app->surface, p3, p1, c);
}

void draw_filledTriangle(App *app, Point p0, Point p1, Point p2, Color c){
    // Approx of Algo:
    // for each horizontal line y between the triangle's top and bottom
    //     compute x_left & x_right for this y
    //     draw line from x_left to x_right

    // Sort the points so that y0 <= y1 <= y2
    if(p1.y < p0.y) _swapPoint(&p1, &p0);
    if(p2.y < p0.y) _swapPoint(&p2, &p0);
    if(p2.y < p1.y) _swapPoint(&p2, &p1);

    // Interpolate between the two points (inverted) and store the values in dynamic arrays
    DynArr *x01 = _interpolate(p0.y, p0.x, p1.y, p1.x);
    DynArr *x12 = _interpolate(p1.y, p1.x, p2.y, p2.x);
    DynArr *x02 = _interpolate(p0.y, p0.x, p2.y, p2.x);

    // Last Value of x01 == First Value of x12
    if(x01->size + x12->size > x02->size)
        x01->size--;
    DynArr_merge(x01, x12);

    // FIGURE OUT WHICH IS LEFT AND WHICH IS RIGHT
    // get middle horizontal line (triangles are always convex)
    DynArr *xLeft;
    DynArr *xRight;
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
            draw_pixel(app->surface, (Point){x, y}, c);
        }
    }
    if(SDL_MUSTLOCK(app->surface))
        SDL_UnlockSurface(app->surface);

    // Free Memory
    DynArr_delete(x01);
    DynArr_delete(x02);
}
