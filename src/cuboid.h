#ifndef _CUBOID_H
#define _CUBOID_H

typedef struct Cuboid {
    float x;
    float y;
    float z;

    float points[8][3];

    float width;
    float height;
    float depth;

    float rotation_x;
    float rotation_y;
    float rotation_z;

    float scaleFactor;
} Cuboid;

static const float CUBOID_BASE[8][3] = {
	{-0.5f,-0.5f,-0.5f},{0.5f,-0.5f,-0.5f},{0.5f,0.5f,-0.5f},{-0.5f,0.5f,-0.5f},
	{-0.5f,-0.5f,0.5f},{0.5f,-0.5f,0.5f},{0.5f,0.5f,0.5f},{-0.5f,0.5f,0.5f}
};

Cuboid* Cuboid_New(float x, float y, float z, float width, float height, float depth, float rotation_x, float rotation_y, float rotation_z);

void UpdateCuboidPoints(Cuboid *c);

#endif