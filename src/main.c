#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "lcdpatterns.h"
#include "mini3d.h"
#include "3dmath.h"
#include "shape.h"
#include "scene.h"
#include "3dhelper.h"

static int update(void* userdata);
const char* fontpath = "num";
LCDFont* font = NULL;
PlaydateAPI *pd;

SoundChannel* sound_effects = NULL;
PDSynth* projectile_sound = NULL;
Scene3D* scene;
Shape3D* shape;
Shape3D* stackbox1;
Shape3D* activebox1;

Scene3DNode* n;
Scene3DNode* rootNode;
Scene3DNode* stackNode;
Scene3DNode* activeNode;

Point3D* p1;Point3D* p2;Point3D* p3;Point3D* p4;
Scene3DNode* node1;
Scene3DNode* node2;
Matrix3D* rot;
Matrix3D* rotn;

Scene3DNode* cubenodestop[10];
Scene3DNode* cubenodestop2[10];
Scene3DNode* cubenodes[10];
Scene3DNode* cubenodes2[10];
Scene3DNode* cubenodesbottom[10];
Scene3DNode* cubenodesbottom2[10];
float r = 0.f;
float ellapsed=0.f;
float res=0.f;

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		pd = playdate;
		const char* err;
		font = pd->graphics->loadFont(fontpath, &err);
		//pd->display->setRefreshRate(1);
		
		if ( font == NULL )
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);

		sound_effects = pd->sound->channel->newChannel();
		projectile_sound = pd->sound->synth->newSynth();
		pd->sound->channel->addSource(sound_effects, (SoundSource*)projectile_sound);
		pd->sound->channel->setVolume(sound_effects, 1.f);

		pd->sound->synth->setWaveform(projectile_sound, kWaveformSquare);
		pd->sound->synth->setAttackTime(projectile_sound, 0.f);
		pd->sound->synth->setDecayTime(projectile_sound, 0.f);
		pd->sound->synth->setSustainLevel(projectile_sound, 1.f);
		pd->sound->synth->setReleaseTime(projectile_sound, 0.1f);
		pd->sound->synth->setVolume(projectile_sound, 1.f, 1.f);

		mini3d_setRealloc(pd->system->realloc);

		scene = scene_new();
		scene_setCameraOrigin(scene, 0.f, 5.f, 6.f);
		scene_setLight(scene, 0.2f, 0.8f, 0.4f);

		rootNode = Scene3D_getRootNode(scene);
		stackNode = Scene3DNode_newChild(rootNode);
		activeNode = Scene3DNode_newChild(rootNode);

		stackbox1 = shape_new_cuboid(1,0.2,1,-0.8f);

		//Vector3D v = { 0,-0.38,0};
		//Scene3DNode_addShapeWithOffset(stackNode, stackbox1, v);
		Scene3DNode_addShape(stackNode, stackbox1);
		Matrix3D tm = matrix_addTranslation(0,-0.38,0);
		Scene3DNode_addTransform(stackNode, &tm);


		activebox1 = shape_new_cuboid(1,0.2,1,-0.3f);
		Scene3DNode_addShape(activeNode, activebox1);

		//Matrix3D tm2 = matrix_addTranslation(0.f,-1.f,0.f);
		//Scene3DNode_addTransform(rootNode, &tm2);

		//scene_setCameraOrigin(scene, 0.f, 0.f, 6.f);
		
		rot = matrix_newRotation(1.f,0.f,1.f,0.0f);
		



		// scene = scene_new();
		// scene_setCameraOrigin(scene, 0.f, 0.f, 6.f);
		// scene_setLight(scene, 0.2f, 0.8f, 0.4f);

		// n = Scene3D_getRootNode(scene);

		// Matrix3D tm = matrix_addTranslation(0,0,0);
		// rotn = matrix_newRotation(60.f,1.f,1.1f,.5f);
		// Scene3DNode_setTransform(n, rotn);
		

		// shape = shape_new_cuboid(1,0.2,1,-0.3f);
		// node1 = Scene3DNode_newChild(n);
		// node2 = Scene3DNode_newChild(node1);
		// //Matrix3D tm = matrix_addTranslation(-5,0,0);
		// //Scene3DNode_addTransform(node1, &tm);
		// Scene3DNode_addShape(node2, shape);

		// rot = matrix_newRotation(1.5f,1.f,1.f,0.3f);
		
		
	}
	
	return 0;
}

#define TEXT_WIDTH 130
#define TEXT_HEIGHT 16

void DrawBackground(PlaydateAPI *pd)
{
    pd->graphics->clear(LCD_PATTERN_DITHER_GREY60);
    pd->graphics->fillEllipse(-45, -65, 490, 370, 0.0, 0.0, LCD_PATTERN_DITHER_GREY50_ALT1);
    pd->graphics->fillEllipse(-40, -60, 480, 360, 0.0, 0.0, LCD_PATTERN_DITHER_GREY45);
    pd->graphics->fillEllipse(-35, -55, 470, 350, 0.0, 0.0, LCD_PATTERN_DITHER_GREY40);
    pd->graphics->fillEllipse(-30, -50, 460, 340, 0.0, 0.0, LCD_PATTERN_DITHER_GREY35);
    pd->graphics->fillEllipse(-25, -45, 450, 330, 0.0, 0.0, LCD_PATTERN_DITHER_GREY30);
    pd->graphics->fillEllipse(-20, -40, 440, 320, 0.0, 0.0, LCD_PATTERN_DITHER_GREY25);
    pd->graphics->fillEllipse(-15, -35, 430, 310, 0.0, 0.0, LCD_PATTERN_DITHER_GREY20);
    pd->graphics->fillEllipse(-10, -30, 420, 300, 0.0, 0.0, LCD_PATTERN_DITHER_GREY15);
    pd->graphics->fillEllipse(0, -20, 400, 280, 0.0, 0.0, LCD_PATTERN_DITHER_GREY10);
    pd->graphics->fillEllipse(10, -10, 380, 260, 0.0, 0.0, LCD_PATTERN_DITHER_GREY05);
    pd->graphics->fillEllipse(20, 0, 360, 240, 0.0, 0.0, kColorWhite);
}

float angle;
float rotty = 0.f;
Matrix3D tmm;
static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;
	DrawBackground(pd);

	if(ellapsed == 0.0f)
	{
		pd->system->resetElapsedTime();
		// pd->graphics->setFont(font);
		// pd->graphics->drawText("69", strlen("69"), kASCIIEncoding, x, y);
		pd->sound->synth->playNote(projectile_sound, 700.f, 1.f, 0.1f, 0);
	}else{
	}
	ellapsed = pd->system->getElapsedTime();
	
	res = sinf(ellapsed)*5;

	tmm = matrix_addTranslation(res,0,0);
	Scene3DNode_setTransform(activeNode, &tmm);

	//activebox1->points->x = res;

	//activeNode->transform.dz = res;

	angle = pd->system->getCrankChange();

	Scene3DNode_addTransform(rootNode, matrix_newRotation(angle,0.f,0.f,1.f));
	Scene3DNode_addTransform(rootNode, rot);

	//Scene3DNode_addTransform(stackNode, rot);
	
	
	rotty +=0.1f;
	// Matrix3D tm = matrix_addTranslation(res,0,0);
	// Scene3DNode_setTransform(node2, &tm);
	
	// Scene3DNode_addTransform(n, rot);
	
	//pd->system->logToConsole("%f\r",node1->transform.dx);
	

	Scene3D_draw(scene, pd->graphics->getFrame(), LCD_ROWSIZE);
	pd->graphics->markUpdatedRows(0, LCD_ROWS-1);

	pd->system->drawFPS(0,0);

	return 1;
}

