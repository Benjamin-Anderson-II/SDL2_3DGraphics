#include "../include/vector.h"
#include <math.h>


Vec3d Vector_new(){
    return (Vec3d){0,0,0,1}; // 1 for matrix vector mult
}

Vec3d Vector_add(Vec3d v1, Vec3d v2){
    return (Vec3d){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}
Vec3d Vector_sub(Vec3d v1, Vec3d v2){
    return (Vec3d){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}
Vec3d Vector_mult(Vec3d v, float k){
    return (Vec3d){v.x * k, v.y * k, v.z * k};
}
Vec3d Vector_div(Vec3d v, float k){
    return (Vec3d){v.x / k, v.y / k, v.z / k};
}

float Vector_dotProd(Vec3d v1, Vec3d v2){
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

float Vector_length(Vec3d v){
    return sqrt(Vector_dotProd(v, v));
}

/* Straight from Quake baybee */
float Q_rsqrt(float f){
    long i;
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = f * 0.5f;
    y  = f;
    i  = *(long *)&y;                   // evil floating point bit hack
    i  = 0x5f3759df - (i >> 1);         // what the fuck?
    y  = *(float *)&i;
    y *= (threehalfs - (x2 * y * y));   // 1st iteration
    y *= (threehalfs - (x2 * y * y));   // 2nd iteration, can be removed

    return y;
}

Vec3d Vector_normalize(Vec3d v){
    float rsqrt = Q_rsqrt(Vector_dotProd(v, v));
    Vec3d ret = Vector_mult(v, rsqrt);
    ret.w = 1;
    return ret;
}

Vec3d Vector_crossProd(Vec3d v1, Vec3d v2){
    return (Vec3d){
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
        1
    };
}

Vec3d Vector_intersectPlane(Vec3d plane_point, Vec3d plane_normal, Vec3d lineStart, Vec3d lineEnd){
    plane_normal = Vector_normalize(plane_normal);
    float plane_d = -Vector_dotProd(plane_normal, plane_point);
    float ad = Vector_dotProd(lineStart, plane_normal);
    float bd = Vector_dotProd(lineEnd, plane_normal);
    float t  = (-plane_d - ad) / (bd - ad);
    Vec3d lineStartToEnd = Vector_sub(lineEnd, lineStart);
    Vec3d lineToIntersect = Vector_mult(lineStartToEnd, t);
    return Vector_add(lineStart, lineToIntersect);
}
