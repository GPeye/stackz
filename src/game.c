#include "game.h"
#include "stackz.h"

const struct playdate_graphics *gfx = NULL;
const struct playdate_sys *sys = NULL;
const struct playdate_display *display = NULL;
const struct playdate_file *file = NULL;
const struct playdate_sound *sound = NULL;

GameStruct Game;

static void initFont() {
    Game.fontpath = "num";
	const char* err;
	Game.font = gfx->loadFont(Game.fontpath, &err);
    if ( Game.font == NULL )
		sys->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, Game.fontpath, err);
    gfx->setFont(Game.font);
}

void InitGame(PlaydateAPI *pd)
{
    pd->display->setRefreshRate(30);
    Game.gState = State_InGame;
    Game.gPd = pd;
    gfx = Game.gPd->graphics;
    sys = Game.gPd->system;
    display = Game.gPd->display;
    file = Game.gPd->file;
    sound = Game.gPd->sound;

    initFont();

    InitStackzSceneData();
}

int Update(void *userdata)
{
    //gfx->clear(kColorWhite);

    switch (Game.gState)
    {
    case State_InGame:
        updateStackz(&Game);
        break;
    default:
        break;
    }

    sys->drawFPS(0, 0);
    return 1;
}