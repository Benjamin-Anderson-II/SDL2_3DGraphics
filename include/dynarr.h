#ifndef DYNARR_H
#define DYNARR_H

#include "matrix.h"
#include "mesh.h"

#define INT
#define FLOAT
#define VEC3D
#define MAT4X4
#define COLOR
#define POINT
#define MESH

typedef union {
    int integer;
    float floating;
    Vec3d vec3d;
    Mat4x4 mat4x4;
    Color color;
    Pixel point;
    Mesh *mesh;
} dynarr_u;

typedef struct {
    dynarr_u *data;
    int size;
    int capacity;
} DynArr;


// Creation
void DynArr_init(DynArr *v, int capacity);
DynArr *DynArr_new(int cap);

// Removal
void DynArr_free(DynArr *v);
void DynArr_delete(DynArr *v);

// Modify
void DynArr_add(DynArr *v, dynarr_u val);
void DynArr_put(DynArr *v, int pos, dynarr_u val);
void DynArr_swap(DynArr *v, int i, int j);
void DynArr_removeAt(DynArr *v, int pos);
void DynArr_merge(DynArr *v1, DynArr *v2);

// Retrieval
dynarr_u DynArr_get(DynArr *v, int pos);

#endif
