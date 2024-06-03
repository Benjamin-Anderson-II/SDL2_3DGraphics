#include "../include/matrix.h"
#include <math.h>
#include <stdio.h>

Vec3d Matrix_multVector(Vec3d i, Mat4x4 m){
    Vec3d o;

    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
    o.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];

    return o;
}

Mat4x4 Matrix_makeIdentity(){
    Mat4x4 id = {0};

    id.m[0][0] = 1.0f;
    id.m[1][1] = 1.0f;
    id.m[2][2] = 1.0f;
    id.m[3][3] = 1.0f;
    return id;
}

Mat4x4 Matrix_makeRotationX(float angleRad){
    Mat4x4 rot = {0};

    rot.m[0][0] = 1.0f;
    rot.m[1][1] = cosf(angleRad);
    rot.m[1][2] = sinf(angleRad);
    rot.m[2][1] = -sinf(angleRad);
    rot.m[2][2] = cosf(angleRad);
    rot.m[3][3] = 1.0f;
    return rot;
}
Mat4x4 Matrix_makeRotationY(float angleRad){
    Mat4x4 rot = {0};

    rot.m[0][0] = cosf(angleRad);
    rot.m[0][2] = sinf(angleRad);
    rot.m[2][0] = -sinf(angleRad);
    rot.m[1][1] = 1.0f;
    rot.m[2][2] = cosf(angleRad);
    rot.m[3][3] = 1.0f;
    return rot;
}
Mat4x4 Matrix_makeRotationZ(float angleRad){
    Mat4x4 rot = {0};

    rot.m[0][0] = cosf(angleRad);
    rot.m[0][1] = sinf(angleRad);
    rot.m[1][0] = -sinf(angleRad);
    rot.m[1][1] = cosf(angleRad);
    rot.m[2][2] = 1.0f;
    rot.m[3][3] = 1.0f;
    return rot;
}

Mat4x4 Matrix_makeTranslation(float x, float y, float z){
    Mat4x4 t = Matrix_makeIdentity();

    t.m[3][0] = x;
    t.m[3][1] = y;
    t.m[3][2] = z;

    return t;
}

Mat4x4 Matrix_makeProjection(float fovDegrees, float aspectRatio, float near, float far){
    Mat4x4 proj = {0};

    float fovRad = 1.0f / tanf(fovDegrees * 0.5f / 180.0f * 3.1415926f);

    proj.m[0][0] = aspectRatio * fovRad;
    proj.m[1][1] = fovRad;
    proj.m[2][2] = far / (far - near);
    proj.m[3][2] = (-far * near) / (far - near);
    proj.m[2][3] = 1.0f;

    return proj;
}

void _printMatrix(Mat4x4 m){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%f, ", m.m[j][i]);
        }
        putchar('\n');
    }
    putchar('\n');
}

Mat4x4 Matrix_multMatrix(Mat4x4 m1, Mat4x4 m2){
    Mat4x4 result = {0};

    for(int c = 0; c < 4; c++)
        for(int r = 0; r < 4; r++)
            for(int i = 0; i < 4; i++)
                result.m[r][c] += m1.m[r][i] * m2.m[i][c];

    return result;
}

Triangle Matrix_multTriangle(Triangle t, Mat4x4 m){
    Triangle mt = t;

    mt.points[0] = Matrix_multVector(t.points[0], m);
    mt.points[1] = Matrix_multVector(t.points[1], m);
    mt.points[2] = Matrix_multVector(t.points[2], m);

    return mt;
}


Mat4x4 Matrix_pointAt(Vec3d pos, Vec3d target, Vec3d up){
    Vec3d newForward, a, newUp, newRight;
    newForward = a = newUp = newRight = Vector_new();
    // Calculate new forward direction
    newForward = Vector_sub(target, pos);
    newForward = Vector_normalize(newForward);

    // Calculate new Up direction
    a = Vector_mult(newForward, Vector_dotProd(up, newForward));
    newUp = Vector_sub(up, a);
    newUp = Vector_normalize(newUp);

    // Create New Right direction
    newRight = Vector_crossProd(newUp, newForward);

    // Create "point at" matrix from calculated values
    Mat4x4 result;
    result.m[0][0] = newRight.x;
    result.m[0][1] = newRight.y;
    result.m[0][2] = newRight.z;
    result.m[0][3] = 0.0f;

    result.m[1][0] = newUp.x;
    result.m[1][1] = newUp.y;
    result.m[1][2] = newUp.z;
    result.m[1][3] = 0.0f;

    result.m[2][0] = newForward.x;
    result.m[2][1] = newForward.y;
    result.m[2][2] = newForward.z;
    result.m[2][3] = 0.0f;

    result.m[3][0] = pos.x;
    result.m[3][1] = pos.y;
    result.m[3][2] = pos.z;
    result.m[3][3] = 1.0f;

    return result;
}

Mat4x4 Matrix_TransRotInverse(Mat4x4 m){
    Mat4x4 result = {0};

    // mirror upper left 3x3 on the xy-axis
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            result.m[i][j] = m.m[j][i];

    // calculate new translation values
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            result.m[3][i] -= m.m[3][j] * result.m[j][i];

    result.m[3][3] = 1.0f;

    return result;
}
