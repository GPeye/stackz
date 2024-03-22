#ifndef _RENDER_H_
#define _RENDER_H_

#include "pd_api.h"

void DrawBackground(PlaydateAPI* pd);

void undrawCube(PlaydateAPI* pd, int osc, int osc2);
void drawCube(PlaydateAPI* pd, int osc, int osc2);

//void undraw1Cube(PlaydateAPI* pd);
void draw1Cube(PlaydateAPI* pd);

#endif