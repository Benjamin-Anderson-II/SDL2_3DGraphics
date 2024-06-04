#include "../include/triangle.h"

#define IN
#define OUT


Triangle Triangle_addVec(Triangle t, Vec3d v){
    Triangle r = t;
    r.points[0] = Vector_add(t.points[0], v);
    r.points[1] = Vector_add(t.points[1], v);
    r.points[2] = Vector_add(t.points[2], v);
    return r;
}
Triangle Triangle_addTri(Triangle t1, Triangle t2){
    Triangle r = t1;
    r.points[0] = Vector_add(t1.points[0], t2.points[0]);
    r.points[1] = Vector_add(t1.points[1], t2.points[1]);
    r.points[2] = Vector_add(t1.points[2], t2.points[2]);
    return r;
}
Triangle Triangle_subVec(Triangle t, Vec3d v){
    Triangle r = t;
    r.points[0] = Vector_sub(t.points[0], v);
    r.points[1] = Vector_sub(t.points[1], v);
    r.points[2] = Vector_sub(t.points[2], v);
    return r;
}
Triangle Triangle_subTri(Triangle t1, Triangle t2){
    Triangle r = t1;
    r.points[0] = Vector_sub(t1.points[0], t2.points[0]);
    r.points[1] = Vector_sub(t1.points[1], t2.points[1]);
    r.points[2] = Vector_sub(t1.points[2], t2.points[2]);
    return r;
}

Triangle Triangle_mult(Triangle t, Vec3d v){
    Triangle r = t;
    r.points[0].x *= v.x;
    r.points[1].x *= v.x;
    r.points[2].x *= v.x;

    r.points[0].y *= v.y;
    r.points[1].y *= v.y;
    r.points[2].y *= v.y;

    r.points[0].z *= v.z;
    r.points[1].z *= v.z;
    r.points[2].z *= v.z;
    return r;
}
Triangle Triangle_div(Triangle t, Vec3d v){
    Triangle r = t;
    r.points[0] = Vector_div(t.points[0], v.x);
    r.points[1] = Vector_div(t.points[1], v.y);
    r.points[2] = Vector_div(t.points[2], v.z);
    return r;
}
Triangle Triangle_normalize(Triangle t){
    Triangle n = t;
    n.points[0] = Vector_div(t.points[0], t.points[0].w);
    n.points[1] = Vector_div(t.points[1], t.points[1].w);
    n.points[2] = Vector_div(t.points[2], t.points[2].w);
    return n;
}


Vec3d Triangle_getNormal(Triangle t){
    Vec3d normal, line1, line2;
    normal = line1 = line2 = Vector_new();

        // Get the lines to either side of the triangle
    line1 = Vector_sub(t.points[1], t.points[0]);
    line2 = Vector_sub(t.points[2], t.points[0]);

        // Get the normal vector (orthogonal to the plane), then normalize it
    normal = Vector_crossProd(line1, line2);
    normal = Vector_normalize(normal);
    return normal;
}

// Get shortest distance from point to plane
float _dist(Vec3d p, Vec3d plane_normal, Vec3d plane_point){
//  Vec3d n = Vector_normalize(p);
    return Vector_dotProd(plane_normal, p) - Vector_dotProd(plane_normal, plane_point);
}

uint8_t Triangle_clipAgainstPlane(
    Vec3d plane_point, Vec3d plane_normal,
    IN  Triangle *triToClip,
    OUT Triangle *outputTri1,
    OUT Triangle *outputTri2)
{

    //make sure plane normal is normalized
    plane_normal = Vector_normalize(plane_normal);

    // points with positive distance will be "inside" the plane
    Vec3d *inside_points[3];  int numInsidePoints = 0;
    Vec3d *outside_points[3]; int numOutsidePoints = 0;

    float dists[3];
    for(int i = 0; i < 3; i++)
        dists[i] = _dist(triToClip->points[i], plane_normal, plane_point);

    for(int i = 0; i < 3; i++){
        if(dists[i] >= 0)
            inside_points[numInsidePoints++]   = &triToClip->points[i];
        else
            outside_points[numOutsidePoints++] = &triToClip->points[i];
    }

    switch(numInsidePoints){
        case 0: // no valid points; clip the whole triangle
        return 0; // no valid triangles

        case 3: // All points lie inside of the plane; do nothing
            *outputTri1 = *triToClip;
        return 1; // only 1 valid triangle

        case 1: // only 1 point is valid
            /* Triangle needs to be clipped */
            // copy appearance
            outputTri1->col = triToClip->col;

            // keep the valid inside point
            outputTri1->points[0] = *inside_points[0];

            // other two points are outside, just find where they intersect the plane
            outputTri1->points[1] = Vector_intersectPlane(plane_point, plane_normal, *inside_points[0], *outside_points[0]);
            outputTri1->points[2] = Vector_intersectPlane(plane_point, plane_normal, *inside_points[0], *outside_points[1]);
        return 1; // only 1 valid triangle

        case 2: // two points are valid
            outputTri1->col = triToClip->col;
            outputTri2->col = triToClip->col;

            // keep the valid inside points
            outputTri1->points[0] = *inside_points[0];
            outputTri1->points[1] = *inside_points[1];
            outputTri1->points[2] = Vector_intersectPlane(plane_point, plane_normal, *inside_points[0], *outside_points[0]);

            // other point is outside, find where it's lines intersect the plane
            outputTri2->points[0] = *inside_points[1];
            outputTri2->points[1] = outputTri1->points[2];
            outputTri2->points[2] = Vector_intersectPlane(plane_point, plane_normal, *inside_points[1], *outside_points[0]);
        return 2; // create 2 valid triangles
        default: return -1;
    }
}
