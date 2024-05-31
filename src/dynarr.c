#include <stdlib.h>
#include <assert.h>
#include "../include/dynarr.h"

void _DynArrSetCapacity(DynArr *v, int newCap);

void
DynArr_init(DynArr *v, int capacity){
    v->data = (dynarr_u *) malloc(sizeof(dynarr_u) * capacity);
    assert(v->data != 0);

    v->size = 0;
    v->capacity = capacity;
}

DynArr *
DynArr_new(int cap) {
    DynArr *r = (DynArr *)malloc(sizeof(DynArr));
    assert(r != 0);
    DynArr_init(r, cap);
    return r;
}

void
DynArr_free(DynArr *v){
    if(v->data != 0){
        free(v->data);
        v->data = 0;
    }
    v->size = 0;
    v->capacity = 0;
}

void
DynArr_delete(DynArr *v){
    DynArr_free(v);
    free(v);
}

void
_DynArrSetCapacity(DynArr *v, int newCap){
    int i;

    dynarr_u *newData = (dynarr_u *)malloc(sizeof(dynarr_u) * newCap);
    assert(newData != 0);

    for(i = 0; i < v->size; i++)
        newData[i] = v->data[i];

    DynArr_free(v);
    v->data = newData;
    v->capacity = newCap;
    v->size = i;
}

void
DynArr_add(DynArr *v, dynarr_u val){
    assert(v!=0);

    if(v->size >= v->capacity)
        _DynArrSetCapacity(v, v->capacity*2);

    v->data[v->size] = val;
    v->size++;
}

dynarr_u
DynArr_get(DynArr *v, int pos){
    assert(pos < v->size);
    assert(pos >= 0);

    return v->data[pos];
}

void
DynArr_put(DynArr *v, int pos, dynarr_u val){
    assert((v != 0) && (v->size > pos) && (pos >= 0));
    v->data[pos] = val;
}

void
DynArr_swap(DynArr *v, int i, int j){
    dynarr_u tmp;
    assert((v != 0));
    assert((i >= 0));
    assert((j >= 0));
    assert((v->size > i));
    assert((v->size > j));

    tmp = v->data[i];
    v->data[i]=v->data[j];
    v->data[j]=tmp;
}

void
DynArr_removeAt(DynArr *v, int pos){
    assert((v != 0) && (v->size > pos) && (pos >= 0));
    for(int i = pos + 1; i < v->size; i++)
        DynArr_put(v, i-1, DynArr_get(v, i));
    v->size--;
}

void
DynArr_merge(DynArr *v1, DynArr *v2){
    for(int i = 0; i < v2->size; i++)
        DynArr_add(v1, DynArr_get(v2, i));
}

/* QUICK SORT */
int _compare(const void *a, const void *b){
    Triangle *t1 = (Triangle *)a;
    Triangle *t2 = (Triangle *)b;
    float z1 = (t1->points[0].z + t1->points[1].z + t1->points[2].z) / 3.0f;
    float z2 = (t2->points[0].z + t2->points[1].z + t2->points[2].z) / 3.0f;
    return z1 < z2;
}

void Triangle_sortInc(DynArr *tris){
    // This is just a wrapper function

    qsort(tris->data, tris->size, sizeof(dynarr_u), _compare);
}
