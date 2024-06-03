#include "../include/triangle.h"

// Get shortest distance from point to plane
float _dist(Vec3d p, Vec3d plane_normal, Vec3d plane_point){
//  Vec3d n = Vector_normalize(p);
    return Vector_dotProd(plane_normal, p) - Vector_dotProd(plane_normal, plane_point);
}

#define IN
#define OUT

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
