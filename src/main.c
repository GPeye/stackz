#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "lcdpatterns.h"

static int update(void* userdata);
const char* fontpath = "num";
LCDFont* font = NULL;
PlaydateAPI *pd;

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

static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;
	DrawBackground(pd);

	

	return 1;
}

