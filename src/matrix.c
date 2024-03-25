#include <stdio.h>
#include <math.h>

#include "matrix.h"

void transformPoint(float *point, float *tMatrix, float *result) {
    for (int i = 0; i < 4; i++) {
        result[i] = 0;
        for (int j = 0; j < 4; j++) {
            result[i] += tMatrix[i*4+j] * point[j];
        }
    }
}

void multiplyTransMatrices(float *A, float *B, float *result) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i*4+j] = 0;
            for (int k = 0; k < 4; k++) {
                result[i*4+j] += A[i*4+k] * B[k*4+j];
            }
        }
    }
    if(result[3] != 0.0)
    {
        //printf("%f", result[3]);
        //printf("%f %f %f\r\n", result[0],result[1],result[2]);
        // result[0] = result[0] / result[3];
        // result[1] = result[0] / result[3];
        // result[2] = result[0] / result[3];
    }
}

void rotateY(float *matrix, float angle, float *result) {
    float rotMat[16];
    identityMatrix(rotMat);

    rotMat[0] = cos(angle);
    rotMat[2] = sin(angle);
    rotMat[8] = -sin(angle);
    rotMat[10] = cos(angle);

    multiplyTransMatrices(rotMat, matrix, result);
}

void rotateX(float *matrix, float angle, float *result) {
    float rotMat[16];
    identityMatrix(rotMat);

    rotMat[5] = cos(angle);
    rotMat[6] = -sin(angle);
    rotMat[9] = sin(angle);
    rotMat[10] = cos(angle);

    multiplyTransMatrices(rotMat, matrix, result);
}

void translate(float *matrix, float dx, float dy, float dz, float *result) {
    float transMat[16];
    identityMatrix(transMat);

    transMat[3] = dx;
    transMat[7] = dy;
    transMat[11] = dz;

    multiplyTransMatrices(transMat, matrix, result);
}

void project(float *matrix, float fov, float aspect, float near, float far, float* result) {
    float yScale = 1.0f / tan(fov / 2.0f);
    float xScale = yScale * aspect;

    float m[16] = {0};
    //identityMatrix(m);
    m[0] = xScale;
    m[5] = yScale;
    m[10] = far / (far - near);
    //m[11] = 1.0;
    m[11] = (-far*near) / (far - near);
    m[14] = 1.0f;

    multiplyTransMatrices(m, matrix, result);
}

void normalize(float* point, float* result) {
    for (int i = 0; i < 3; i++) {
        result[i] = point[i] / point[3];
    }
    result[3] = 1.0;
}


void identityMatrix(float *result) {
    for (int i = 0; i < 16; i++) {
        if (i % 5 == 0) {
            result[i] = 1;
        } else {
            result[i] = 0;
        }
    }
}

void printMatrix(float *matrix) {
    for (int i = 0; i < 16; i++) {
        printf("%f ", matrix[i]);
        if ((i+1) % 4 == 0) {
            printf("\n");
        }
    }
}

void printPoint(float *point) {
    for (int i = 0; i < 4; i++) {
        printf("%f ", point[i]);
    }
    printf("\n");
}