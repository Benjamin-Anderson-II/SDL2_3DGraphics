#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/file_importer.h"
#include "../include/dynarr.h"
#include "../include/mesh.h"

int read_objAsMesh(char *fileName, Mesh *mesh){
    FILE *fp;
    DynArr *verts; //Vec3d
    char line[255];
    if((fp = fopen(fileName, "r")) == NULL){
        fprintf(stderr, "Failed to Open File: %s\n", fileName);
        return -1;
    }
    verts = DynArr_new(64);
    
    while(fgets(line, sizeof(line), fp)){
        //Read in Vertices
        if(line[0] == 'v'){
            Vec3d v;
            char *tmp = strtok(line+2, " "); // skip "v "
            char *endptr;
            v.x = -strtof(tmp, &endptr);
            tmp = strtok(NULL, " ");
            v.y = -strtof(tmp, &endptr);
            tmp = strtok(NULL, " ");
            v.z = strtof(tmp, &endptr);
            v.w = 1.0f;

            // Add vertex to vertices
            DynArr_add(verts, (dynarr_u)v);
        } else if(line[0] == 'f'){
            // read in triangle
            Triangle t;
            char *tmp = strtok(line+2, " "); // skip "f "
            char *endptr;
            t.points[0] = DynArr_get(verts, atoi(tmp) - 1).vec3d;
            tmp = strtok(NULL, " ");
            t.points[1] = DynArr_get(verts, atoi(tmp) - 1).vec3d;
            tmp = strtok(NULL, " ");
            t.points[2] = DynArr_get(verts, atoi(tmp) - 1).vec3d;

            // add triangle into the mesh
            Mesh_add(mesh, t);
        }
    }

    fclose(fp);
    return 0;
}
