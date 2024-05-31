#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x, y, z, w;
} Vec3d;

Vec3d Vector_new();

Vec3d Vector_add(Vec3d v1, Vec3d v2);
Vec3d Vector_sub(Vec3d v1, Vec3d v2);
Vec3d Vector_mul(Vec3d v, float k);
Vec3d Vector_div(Vec3d v, float k);

float Vector_dotProd(Vec3d v1, Vec3d v2);
float Vector_length(Vec3d v);
Vec3d Vector_normalize(Vec3d v);
Vec3d Vector_crossProd(Vec3d v1, Vec3d v2);

#endif
