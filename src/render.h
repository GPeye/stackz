#ifndef _RENDER_H_
#define _RENDER_H_

#include "pd_api.h"
#include "render.h"
#include "camera.h"
#include "cuboid.h"
#include "face.h"
#include "pvector.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH_CENTER SCREEN_WIDTH / 2
#define SCREEN_HEIGHT_CENTER SCREEN_HEIGHT / 2

void DrawBackground(PlaydateAPI* pd);

void undrawCube(PlaydateAPI* pd, int osc, int osc2);
void drawCube(PlaydateAPI* pd, int osc, int osc2);

//void undraw1Cube(PlaydateAPI* pd);
void draw1Cube(PlaydateAPI* pd);

void DrawCuboid(PlaydateAPI* pd, Cuboid* cuboid, Camera* cam);

void DrawCuboidWireframe(PlaydateAPI* pd, Cuboid* cuboid, Camera* cam);

#endif