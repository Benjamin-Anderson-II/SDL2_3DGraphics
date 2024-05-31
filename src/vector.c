#include "../include/vector.h"
#include <math.h>


Vec3d Vector_new(){
    return (Vec3d){0,0,0,1};
}

Vec3d Vector_add(Vec3d v1, Vec3d v2){
    return (Vec3d){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}
Vec3d Vector_sub(Vec3d v1, Vec3d v2){
    return (Vec3d){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}
Vec3d Vector_mul(Vec3d v, float k){
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

Vec3d Vector_normalize(Vec3d v){
    return Vector_div(v, Vector_length(v));
}

Vec3d Vector_crossProd(Vec3d v1, Vec3d v2){
    return (Vec3d){
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}
