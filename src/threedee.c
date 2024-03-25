#include <math.h>
#include "threedee.h"

/* Simplified Matrix Multiplication
   Since we know our m2 will be 1 dimensional
   we can skip on of the for loops
[1 0 0 0]   [x]   [x]
[0 1 0 0]   [Y]   [y]
[0 0 1 0] x [z] = [z]
[0 0 0 1]   [w]   [y]
*/
void matrixMultipy(
    float m1[MATRIX_SIZE][MATRIX_SIZE],
    float m2[MATRIX_SIZE],
    float *result)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        result[i] = 0.f;
        for (int k = 0; k < VECTOR_SIZE; k++)
        {
            result[i] += m1[i][k] * m2[k];
        }
    }
}

/* MatMultipy X Rotation Matrix
[cos(angle),    0,  sin(angle), 0]   [x]   [newx]
[0,             1,  0,          0]   [Y]   [y]
[-sin(angle),   0,  cos(angle), 0] x [z] = [newz]
[0,             0,  0,          0]   [w]   [0w]
*/
void RotateX(float matrix[4], float angle, float result)
{
    float rotationMatrix[4][4] = {0};
    rotationMatrix[0][0] = 1;
    rotationMatrix[1][1] = cosf(angle);
    rotationMatrix[1][2] = -sinf(angle);
    rotationMatrix[2][1] = sinf(angle);
    rotationMatrix[2][2] = cosf(angle);

    matrixMultipy(rotationMatrix, matrix, &result);
}

/* MatMultipy Y Rotation Matrix
[cos(angle),    0,  sin(angle), 0]   [x]   [newx]
[0,             1,  0,          0]   [Y]   [y]
[-sin(angle),   0,  cos(angle), 0] x [z] = [newz]
[0,             0,  0,          0]   [w]   [0w]
*/
void RotateY()
{
}

/* MatMultipy Y Rotation Matrix
[cos(angle),    0,  sin(angle), 0]   [x]   [newx]
[0,             1,  0,          0]   [Y]   [y]
[-sin(angle),   0,  cos(angle), 0] x [z] = [newz]
[0,             0,  0,          0]   [w]   [0w]
*/
void RotateZ()
{
}

/* Projection Matrix
 */
void projectPoint(float fov, float aspect, float near, float far, float matrix[4], float *result)
{
    float yscale = (1 / tanf(fov / 2));
    float persectiveMatrix[4][4] = {0};
    persectiveMatrix[0][0] = aspect * yscale;
    persectiveMatrix[1][1] = yscale;
    persectiveMatrix[2][2] = far / (far - near);
    persectiveMatrix[2][3] = (-far * near) / (far - near);
    persectiveMatrix[3][2] = 1.0f;

    matrixMultipy(persectiveMatrix, matrix, result);

    if(result[3] != 0.0) {
        result[0] = result[0] / result[3];
        result[1] = result[1] / result[3];
        result[2] = result[2] / result[3];
    }
}

// void projectPoint(float matrix[4], float* result)
// {
//     float rotationMatrix[4][4] = {0};
//     float z = 1.f / (2.f - matrix[2]);
//     rotationMatrix[0][0] = z;
//     rotationMatrix[1][1] = z;

//     matrixMultipy(rotationMatrix, matrix, result);
// }