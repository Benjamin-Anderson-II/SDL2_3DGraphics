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
    Mat4x4 id;
    memset(&id, 0, sizeof(Mat4x4));

    id.m[0][0] = 1.0f;
    id.m[1][1] = 1.0f;
    id.m[2][2] = 1.0f;
    id.m[3][3] = 1.0f;
    return id;
}

Mat4x4 Matrix_makeRotationX(float angleRad){
    Mat4x4 rot;
    memset(&rot, 0, sizeof(Mat4x4));

    rot.m[0][0] = 1.0f;
    rot.m[1][1] = cosf(angleRad);
    rot.m[1][2] = sinf(angleRad);
    rot.m[2][1] = -sinf(angleRad);
    rot.m[2][2] = cosf(angleRad);
    rot.m[3][3] = 1.0f;
    return rot;
}
Mat4x4 Matrix_makeRotationY(float angleRad){
    Mat4x4 rot;
    memset(&rot, 0, sizeof(Mat4x4));

    rot.m[0][0] = cosf(angleRad);
    rot.m[0][2] = sinf(angleRad);
    rot.m[2][0] = -sinf(angleRad);
    rot.m[1][1] = 1.0f;
    rot.m[2][2] = cosf(angleRad);
    rot.m[3][3] = 1.0f;
    return rot;
}
Mat4x4 Matrix_makeRotationZ(float angleRad){
    Mat4x4 rot;
    memset(&rot, 0, sizeof(Mat4x4));

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
    float fovRad = 1.0f / tanf(fovDegrees * 0.5f / 180.0f * 3.14159f);
    Mat4x4 proj;
    memset(&proj, 0, sizeof(Mat4x4));

    proj.m[0][0] = aspectRatio * fovRad;
    proj.m[1][1] = fovRad;
    proj.m[2][2] = far / (far - near);
    proj.m[3][2] = (-far * near) / (far - near);
    proj.m[2][3] = 1.0f;
    proj.m[3][3] = 0.0f;

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
//    _printMatrix(m1);
//    _printMatrix(m2);
//    putchar('\n');
    Mat4x4 result;
    memset(&result, 0, sizeof(Mat4x4));

//    m1.m[0][3] = 1.0f;
//    m1.m[1][3] = 1.0f;
//    m1.m[2][3] = 1.0f;

    for(int c = 0; c < 4; c++){
        for(int r = 0; r < 4; r++){
            for(int i = 0; i < 4; i++){
//                printf("%f * %f\n", m1.m[r][i], m2.m[i][c]);
                result.m[r][c] += m1.m[r][i] * m2.m[i][c];
            }
//            printf(" = %f\n", result.m[r][c]);
        }
//        putchar('\n');
    }

    return result;
}

Triangle Matrix_multTriangle(Triangle t, Mat4x4 m){
    Triangle mt = t;

    mt.points[0] = Matrix_multVector(t.points[0], m);
    mt.points[1] = Matrix_multVector(t.points[1], m);
    mt.points[2] = Matrix_multVector(t.points[2], m);

    return mt;
}
