#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "render.h"
#include "matrix.h"
#include "threedee.h"

static int update(void* userdata);
const char* fontpath = "num";
LCDFont* font = NULL;
PlaydateAPI *pd;
float ellapsed = 0.0f;
float res = 0.f;
float res2 = 0.f;
Cuboid* mine;

SoundChannel* sound_effects = NULL;
PDSynth* projectile_sound = NULL;

float cpoints[8][4] = {
			{-20, -20, 20, 0},
			{ 20, -20, 20, 0},
			{ 20,  20, 20, 0},
			{-20,  20, 20, 0},

			{-20, -20,  40, 0},
			{ 20, -20,  40, 0},
			{ 20,  20,  40, 0},
			{-20,  20,  40, 0},
		};

float ppoints[8][3] = {0};

static void connectPoints(int i, int j, float projectedPoints[8][3])
{
	float* a = projectedPoints[i];
	float* b = projectedPoints[j];
	pd->graphics->drawLine((int)a[0],(int)a[1],(int)b[0],(int)b[1],3,kColorBlack);
}

static void _translate(float x, float y, float z, float *arr)
{
	arr[0] = arr[0] + x;
	arr[1] = arr[1] + y;
	arr[2] = arr[2] + z;
}

static void _scaled(float *arr)
{
	//float scaleamt = (mine->width / SCREEN_WIDTH);
	arr[0] = arr[0] * 50.f;
	arr[1] = arr[1] * 50.f;
	arr[2] = arr[2] * 50.f;
}
 
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

		//memset(projectedPoints, 0, sizeof projectedPoints);

		mine = Cuboid_New(20,0,0,100.f,100.f,50.f,0.f,0.f,0.f);

		// float cpoints[8][4] = {
		// 	{-100, -100, -100, 0},
		// 	{ 100, -100, -100, 0},
		// 	{ 100,  100, -100, 0},
		// 	{-100,  100, -100, 0},

		// 	{-100, -100,  100, 0},
		// 	{ 100, -100,  100, 0},
		// 	{ 100,  100,  100, 0},
		// 	{-100,  100,  100, 0},
		// };

		// float cpoints[8][4] = {
		// 	{-20, -20, -20, 0},
		// 	{ 20, -20, -20, 0},
		// 	{ 20,  20, -20, 0},
		// 	{-20,  20, -20, 0},

		// 	{-20, -20,  -40, 0},
		// 	{ 20, -20,  -40, 0},
		// 	{ 20,  20,  -40, 0},
		// 	{-20,  20,  -40, 0},
		// };

		

		// float cpoints[8][4] = {
		// 	{-0.5f, -0.5f, -0.5f, 0},
		// 	{ 0.5f, -0.5f, -0.5f, 0},
		// 	{ 0.5f,  0.5f, -0.5f, 0},
		// 	{-0.5f,  0.5f, -0.5f, 0},

		// 	{-0.5f, -0.5f,  0.5f, 0},
		// 	{ 0.5f, -0.5f,  0.5f, 0},
		// 	{ 0.5f,  0.5f,  0.5f, 0},
		// 	{-0.5f,  0.5f,  0.5f, 0},
		// };

		float m1[4][4] = {
			{1,2,1,1},
			{0,1,1,1},
			{1,0,1,1},
			{1,1,1,1}
		};
		float m2[4] = {2,6,5,3};
		float res[4];
		matrixMultipy(m1,m2,&res);
		//float ppoints[8][3] = {0};

		for(int i=0;i<8;i++) {
			float pp[4] = {cpoints[i][0], cpoints[i][1], cpoints[i][2], cpoints[i][3]};
			float result[4];
			projectPoint(1.0, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 100.0, pp, &result);
			//_scale(&result);
			ppoints[i][0] = result[0];
			ppoints[i][1] = result[1];
			ppoints[i][2] = result[2];
			_translate(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0,&ppoints[i]);
		}


		//pd->system->logToConsole("%f, %f, %f, %f", res[0],res[1],res[2],res[3]);
		//pd->system->logToConsole("res: %f, %f, %f, %f", ppoints[1][0],ppoints[1][1],ppoints[1][2],ppoints[1][3]);
		//pd->system->logToConsole("res: %f, %f, %f, %f", ppoints[4][0],ppoints[4][1],ppoints[4][2],ppoints[4][3]);
	}
	
	return 0;
}

Camera cam = {
		.x = 0,
		.y = 0,
		.z = 0,
		.angle = 0,
		.rotation_x = 0,
		.rotation_y = 0,
		.rotation_z = 0
	};


#define TEXT_WIDTH 130
#define TEXT_HEIGHT 16

float cubeCoords[8][4] = {
    // Front
    {-100.0, -100.0, -100.0, 1.0},
    {100.0, -100.0, -100.0, 1.0},
    {100.0, 100.0, -100.0, 1.0},
    {-100.0, 100.0, -100.0, 1.0},
    // Back
    {-100.0, -100.0, 100.0, 1.0},
    {100.0, -100.0, 100.0, 1.0},
    {100.0, 100.0, 100.0, 1.0},
    {-100.0, 100.0, 100.0, 1.0},
};

float cubey[8][4] = {
    // Front
    {-100.0f, -100.0f, -100.0f, 0.0f},
    {100.0f, -100.0f, -100.0f, 0.0f},
    {100.0f, 100.0f, -100.0f, 0.0f},
    {-100.0f, 100.0f, -100.0f, 0.0f},
    // Back
    {-100.0f, -100.0f, 100.0f, 0.0f},
    {100.0f, -100.0f, 100.0f, 0.0f},
    {100.0f, 100.0f, 100.0f, 0.0f},
    {-100.0f, 100.0f, 100.0f, 0.0f},
};

static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;
	DrawBackground(pd);
	// if(ellapsed == 0.0f)
	// {
	// 	//pd->system->resetElapsedTime();
	// 	// pd->graphics->setFont(font);
	// 	// pd->graphics->drawText("69", strlen("69"), kASCIIEncoding, x, y);
	// 	// pd->sound->synth->playNote(projectile_sound, 700.f, 1.f, 0.1f, 0);
	// }else{
	// 	undrawCube(pd,(int)res,(int)res2);
	// }
	// ellapsed = pd->system->getElapsedTime();
	
	// res = sinf(ellapsed)*50;
	// res2 = sinf(ellapsed)*45;
	// //pd->system->logToConsole("%d",(int)res);

	// draw1Cube(pd);
	// drawCube(pd, (int)res, (int)res2);
        
	
	// DrawCuboid(pd, mine, &cam);
	// pd->system->drawFPS(0,0);

	// mine->rotation_x=(pd->system->getCrankAngle() +90) * (3.14159 / 180);
	// mine->rotation_y=pd->system->getCrankAngle() * (3.14159 / 180);
	// mine->rotation_z=pd->system->getCrankAngle() * (3.14159 / 180);
	// mine->rotation_x=(45.f *(3.14159 / 180));
	// mine->rotation_y=(-35.f *(3.14159 / 180));
	// mine->rotation_z=(30.f *(3.14159 / 180));
	//mine->rotation_y=5.0f;
	//mine->rotation_z=45.0f;
	
	// mine->x = pd->system->getCrankAngle() * (3.14159 / 180);
	// mine->x = pd->system->getCrankAngle() * (3.14159 / 180);
	//mine->depth= 100 + 10 * pd->system->getCrankAngle() * (3.14159 / 180);
	
	// float transMatrix[16];
    // float tmp[16];
	

    // identityMatrix(transMatrix);
    // rotateY(transMatrix, pd->system->getCrankAngle() * (3.14159 / 180), tmp);
    // translate(tmp, 200, 120, 0, transMatrix);
    // //rotateX(transMatrix, 0.f, tmp);
    // //translate(tmp, 0, 50, 0, transMatrix);
    // project(transMatrix, 2.0, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 100.0, tmp);

	// float transPoints[8][4];
    // for (int i = 0; i < 8; i++) {
	// 	float tmpP[4];
    //     transformPoint(cubeCoords[i], tmp, transPoints[i]);
    // }

	// for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         if (i == j)
    //             continue;
	// 			pd->graphics->drawLine(
	// 				(int)transPoints[i][0],
	// 				(int)transPoints[i][1],
	// 				(int)transPoints[j][0],
	// 				(int)transPoints[j][1],
	// 				3,
	// 				kColorBlack);
	// 			//connectPoints(i,j, transPoints);
    //         // SDL_RenderDrawLine(renderer, 
    //         //         transPoints[i][0], transPoints[i][1], 
    //         //         transPoints[j][0], transPoints[j][1]);
    //     }
    // }

	int i;
	for (i = 0; i < 4; i++)
	{
		connectPoints(i, (i + 1) % 4, ppoints);
		connectPoints(i + 4, ((i + 1) % 4) + 4, ppoints);
		connectPoints(i, i + 4, ppoints);
	}

	for(int i=0;i<8;i++) {
			float pp[4] = {cpoints[i][0], cpoints[i][1], cpoints[i][2], cpoints[i][3]};
			float result[4];
			projectPoint(2.5, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 100.0, pp, &result);
			_scaled(&result);
			ppoints[i][0] = result[0];
			ppoints[i][1] = result[1];
			ppoints[i][2] = result[2];
			_translate(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0,&ppoints[i]);
	}

	for(int i=0;i<8;i++){
		pd->graphics->drawEllipse(ppoints[i][0], ppoints[i][1], 3,3,3,0,0,kColorBlack);
	}

	return 1;
}

