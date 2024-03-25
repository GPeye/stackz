#ifndef _THREEDEE_H
#define _THREEDEE_H

#define MATRIX_SIZE 4
#define VECTOR_SIZE 4

void matrixMultipy(
    float m1[MATRIX_SIZE][MATRIX_SIZE],
    float m2[MATRIX_SIZE], 
    float* result);

void projectPoint(float fov, float aspect, float near, float far, float matrix[4], float *result);

#endif