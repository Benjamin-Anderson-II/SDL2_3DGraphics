/**
 * The only reason this exists is because I need to have
 * a dynamic array of meshes, and I can't recursively put
 * Mesh in the dynarr_u data type
*/


#ifndef MESH_H
#define MESH_H

#include "triangle.h"

typedef struct {
    Triangle *tris;
    int size;
    int capacity;
} Mesh;

// Creation
void Mesh_init(Mesh *v, int capacity);
Mesh *Mesh_new(int cap);

// Removal
void Mesh_free(Mesh *v);
void Mesh_delete(Mesh *v);

// Modify
void Mesh_add(Mesh *v, Triangle val);
void Mesh_put(Mesh *v, int pos, Triangle val);
void Mesh_swap(Mesh *v, int i, int j);
void Mesh_removeAt(Mesh *v, int pos);
void Mesh_merge(Mesh *v1, Mesh *v2);

void Mesh_sortInc(Mesh *mesh);

// Retrieval
Triangle Mesh_get(Mesh *v, int pos);

#endif
