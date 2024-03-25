#ifndef _MATRIX_H
#define _MATRIX_H

void transformPoint(float *point, float *tMatrix, float *result);
void multiplyTransMatrices(float *A, float *B, float *result);
void rotateY(float* matrix, float angle, float *result);
void rotateX(float *matrix, float angle, float *result);
void translate(float *matrix, float dx, float dy, float dz, float *result);
void project(float *matrix, float fov, float aspect, float near, float far, float* result);
void normalize(float* point, float* result);
void identityMatrix(float* result);
void printMatrix(float* matrix);
void printPoint(float* point);

#endif