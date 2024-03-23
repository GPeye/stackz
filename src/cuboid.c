#include <stdio.h>
#include "cuboid.h"

static float largest(float x,float y,float z){
    float max;
    max=x;
    if(y>max){
        max=y;
    }
    if(z>max){
        max=z;
    }
    return max;
}

Cuboid* Cuboid_New(float x, float y, float z, float width, float height, float depth, float rotation_x, float rotation_y, float rotation_z)
{
    Cuboid* c = malloc(sizeof(Cuboid));
    c->x = x;
    c->y = y;
    c->z = z;
    c->width = width;
    c->height = height;
    c->depth = depth;
    c->rotation_x = rotation_x;
    c->rotation_y = rotation_y;
    c->rotation_z = rotation_z;
    UpdateCuboidPoints(c);
    //memcpy(c->points, temp, sizeof(temp));
    return c;
}

void UpdateCuboidPoints(Cuboid *c)
{
    float max = largest(c->width,c->height,c->depth);
    c->scaleFactor = max;
    float normalizedWidth = c->width/max;
    float normalizedHeight = c->height/max;
    float normalizedDepth = c->depth/max;

    float tx = 0.5f*normalizedWidth;
    float ty = 0.5f*normalizedHeight;
    float tz = 0.5f*normalizedDepth;

    float temp[8][3] = {
        {-tx, -ty, -tz}, {tx, -ty, -tz}, {tx, ty, -tz}, {-tx, ty, -tz},
        {-tx, -ty, tz}, {tx, -ty, tz}, {tx, ty, tz}, {-tx, ty, tz}
    };
    memcpy(c->points, temp, sizeof(temp));
}

// normalize width, height and depth into ratios within the range 0-1