#include <stdlib.h>
#include <assert.h>
#include "../include/mesh.h"

void _MeshSetCapacity(Mesh *v, int newCap);

void
Mesh_init(Mesh *v, int capacity){
    v->tris = (Triangle *) malloc(sizeof(Triangle) * capacity);
    assert(v->tris != 0);

    v->size = 0;
    v->capacity = capacity;
}

Mesh *
Mesh_new(int cap) {
    Mesh *r = (Mesh *)malloc(sizeof(Mesh));
    assert(r != 0);
    Mesh_init(r, cap);
    return r;
}

void
Mesh_free(Mesh *v){
    if(v->tris != 0){
        free(v->tris);
        v->tris = 0;
    }
    v->size = 0;
    v->capacity = 0;
}

void
Mesh_delete(Mesh *v){
    Mesh_free(v);
    free(v);
}

void
_MeshSetCapacity(Mesh *v, int newCap){
    int i;

    Triangle *newData = (Triangle *)malloc(sizeof(Triangle) * newCap);
    assert(newData != 0);

    for(i = 0; i < v->size; i++)
        newData[i] = v->tris[i];

    Mesh_free(v);
    v->tris = newData;
    v->capacity = newCap;
    v->size = i;
}

void
Mesh_add(Mesh *v, Triangle val){
    assert(v!=0);

    if(v->size >= v->capacity)
        _MeshSetCapacity(v, v->capacity*2);

    v->tris[v->size] = val;
    v->size++;
}

Triangle
Mesh_get(Mesh *v, int pos){
    assert(pos < v->size);
    assert(pos >= 0);

    return v->tris[pos];
}

void
Mesh_put(Mesh *v, int pos, Triangle val){
    assert((v != 0) && (v->size > pos) && (pos >= 0));
    v->tris[pos] = val;
}

void
Mesh_swap(Mesh *v, int i, int j){
    Triangle tmp;
    assert((v != 0));
    assert((i >= 0));
    assert((j >= 0));
    assert((v->size > i));
    assert((v->size > j));

    tmp = v->tris[i];
    v->tris[i]=v->tris[j];
    v->tris[j]=tmp;
}

void
Mesh_removeAt(Mesh *v, int pos){
    assert((v != 0) && (v->size > pos) && (pos >= 0));
    for(int i = pos + 1; i < v->size; i++)
        Mesh_put(v, i-1, Mesh_get(v, i));
    v->size--;
}

void
Mesh_merge(Mesh *v1, Mesh *v2){
    for(int i = 0; i < v2->size; i++)
        Mesh_add(v1, Mesh_get(v2, i));
}

/* QUICK SORT */
int _compare(const void *a, const void *b){
    Triangle *t1 = (Triangle *)a;
    Triangle *t2 = (Triangle *)b;
    float z1 = (t1->points[0].z + t1->points[1].z + t1->points[2].z) / 3.0f;
    float z2 = (t2->points[0].z + t2->points[1].z + t2->points[2].z) / 3.0f;
    return z1 < z2;
}

void Mesh_sortInc(Mesh *mesh){
    // This is just a wrapper function

    qsort(mesh->tris, mesh->size, sizeof(Triangle), _compare);
}
