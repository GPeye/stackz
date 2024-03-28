#ifndef _GAME_H_
#define _GAME_H_

#include <stdio.h>
#include "pd_api.h"
#include "mini3d.h"
#include "3dmath.h"
#include "shape.h"
#include "scene.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240

extern const struct playdate_graphics *gfx;
extern const struct playdate_sys *sys;
extern const struct playdate_display *display;
extern const struct playdate_file *file;
extern const struct playdate_sound *sound;

typedef enum
{
    State_SplashScreen,
    State_Menu,
    State_InGame,
    State_Scoreboard,
    State_Tutorial
} GameScene;

typedef struct
{
    PlaydateAPI *gPd;
    GameScene gState;

    const char* fontpath;;
    LCDFont* font;

    struct
    {
        Scene3D *scene;
        Shape3D *activebox1;

        Scene3DNode *rootNode;
        Scene3DNode *stackNode;
        Scene3DNode *activeNode;

        Shape3D *stackBoxes[100];
        Shape3D *activeBox;

        Matrix3D activeNodeMatrix;
        Matrix3D stackNodeMatrix;
        Matrix3D *crankMatrix;

        float ellapsed;
        float crankChange;
        float activeOscillator;
        int score;

    } StackzData;

} GameStruct;

extern GameStruct Game;

void InitGame(PlaydateAPI *pd);
int Update(void *userdata);

#endif