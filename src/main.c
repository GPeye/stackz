#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "render.h"

static int update(void* userdata);
const char* fontpath = "num";
LCDFont* font = NULL;
PlaydateAPI *pd;
float ellapsed = 0.0f;
float res = 0.f;
float res2 = 0.f;


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
		//pd->display->setRefreshRate(0);
		
		if ( font == NULL )
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);
	}
	
	return 0;
}


#define TEXT_WIDTH 130
#define TEXT_HEIGHT 16

int x = (400-TEXT_WIDTH)/2;
int y = (50-TEXT_HEIGHT)/2;
int dx = 5;
int dy = 7;
int currentPattern = 0;
int currentPatternMax = 18;

static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;
	
	if(ellapsed == 0.0f)
	{
		DrawBackground(pd);
		pd->system->resetElapsedTime();
		pd->graphics->setFont(font);
		pd->graphics->drawText("69", strlen("69"), kASCIIEncoding, x, y);
	}else{
		undrawCube(pd,(int)res,(int)res2);
	}
	ellapsed = pd->system->getElapsedTime();;
	res = sinf(ellapsed)*50;
	res2 = sinf(ellapsed)*45;
	//pd->system->logToConsole("%d",(int)res);

	draw1Cube(pd);
	drawCube(pd, (int)res, (int)res2);
        
	//pd->system->drawFPS(0,0);

	return 1;
}

