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
Cuboid* mine;

SoundChannel* sound_effects = NULL;
PDSynth* projectile_sound = NULL;
 
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

		mine = Cuboid_New(0,0,0,200.f,120.f,40.f,0.f,0.f,0.f);
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
        
	
	DrawCuboid(pd, mine, &cam);
	pd->system->drawFPS(0,0);

	mine->rotation_x=pd->system->getCrankAngle() * (3.14159 / 180);
	mine->rotation_y=pd->system->getCrankAngle() * (3.14159 / 180);
	mine->rotation_z=pd->system->getCrankAngle() * (3.14159 / 180);
	//mine->width= 100 + 10 * pd->system->getCrankAngle() * (3.14159 / 180);
	
	return 1;
}

