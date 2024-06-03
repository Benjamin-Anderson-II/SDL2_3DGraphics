#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include "triangle.h"

typedef struct {
    float m[4][4];
} Mat4x4;

Mat4x4 Matrix_makeIdentity();
Mat4x4 Matrix_makeRotationX(float angleRad);
Mat4x4 Matrix_makeRotationY(float angleRad);
Mat4x4 Matrix_makeRotationZ(float angleRad);
Mat4x4 Matrix_makeTranslation(float x, float y, float z);
Mat4x4 Matrix_makeProjection(float fovDegrees, float aspectRatio, float near, float far);

Mat4x4   Matrix_multMatrix(Mat4x4 m1, Mat4x4 m2);
Vec3d    Matrix_multVector(Vec3d i, Mat4x4 m);
Triangle Matrix_multTriangle(Triangle t, Mat4x4 m);

Mat4x4 Matrix_pointAt(Vec3d pos, Vec3d target, Vec3d up);
Mat4x4 Matrix_TransRotInverse(Mat4x4 m);

#endif
