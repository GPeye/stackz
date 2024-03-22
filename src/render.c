#include "pd_api.h"
#include "render.h"
#include "lcdpatterns.h"

void DrawBackground(PlaydateAPI* pd)
{
    pd->graphics->clear(LCD_PATTERN_DITHER_GREY35);
	pd->graphics->fillEllipse(-40,-60,480,360,0.0,0.0,LCD_PATTERN_DITHER_GREY30);
	pd->graphics->fillEllipse(-30,-50,460,340,0.0,0.0,LCD_PATTERN_DITHER_GREY25);
	pd->graphics->fillEllipse(-20,-40,440,320,0.0,0.0,LCD_PATTERN_DITHER_GREY20);
	pd->graphics->fillEllipse(-10,-30,420,300,0.0,0.0,LCD_PATTERN_DITHER_GREY15);
	pd->graphics->fillEllipse(0,-20,400,280,0.0,0.0,LCD_PATTERN_DITHER_GREY10);
	pd->graphics->fillEllipse(10,-10,380,260,0.0,0.0,LCD_PATTERN_DITHER_GREY05);
	pd->graphics->fillEllipse(20,0,360,240,0.0,0.0,kColorWhite);
}

void undrawCube(PlaydateAPI* pd, int osc, int osc2)
{	int x = 160+(int)(osc*1.5f);
	int y = 80-(int)(osc2*1.4f);
	int height=20;
	int width=50;
	int scale=5;
	int toppoints[8] = {scale*10+x, scale*0+y, scale*20+x, scale*8+y, scale*10+x, scale*16+y, scale*0+x, scale*8+y};
	int rightpoints[8] = {scale*20+x, scale*8+y,scale*20+x, scale*11+y,scale*10+x, scale*19+y,scale*10+x, scale*16+y};
	int leftpoints[8] = {scale*10+x, scale*16+y, scale*10+x, scale*19+y, scale*0+x, scale*11+y, scale*0+x, scale*8+y};
	pd->graphics->fillPolygon(4,toppoints,kColorWhite,kPolygonFillEvenOdd);
	pd->graphics->fillPolygon(4,rightpoints,kColorWhite,kPolygonFillEvenOdd);
	pd->graphics->fillPolygon(4,leftpoints,kColorWhite,kPolygonFillEvenOdd);
}

void drawCube(PlaydateAPI* pd, int osc, int osc2)
{	int x = 160+(int)(osc*1.5f);
	int y = 80-(int)(osc2*1.4f);
	int height=20;
	int width=50;
	int scale=5;
	int toppoints[8] = {scale*10+x, scale*0+y, scale*20+x, scale*8+y, scale*10+x, scale*16+y, scale*0+x, scale*8+y};
	int rightpoints[8] = {scale*20+x, scale*8+y,scale*20+x, scale*11+y,scale*10+x, scale*19+y,scale*10+x, scale*16+y};
	int leftpoints[8] = {scale*10+x, scale*16+y, scale*10+x, scale*19+y, scale*0+x, scale*11+y, scale*0+x, scale*8+y};
	pd->graphics->fillPolygon(4,toppoints,LCD_PATTERN_DITHER_GREY30,kPolygonFillEvenOdd);
	pd->graphics->fillPolygon(4,rightpoints,LCD_PATTERN_DITHER_GREY75,kPolygonFillEvenOdd);
	pd->graphics->fillPolygon(4,leftpoints,LCD_PATTERN_DITHER_GREY45,kPolygonFillEvenOdd);
}

void draw1Cube(PlaydateAPI* pd)
{	int x = 160;
	int y = 94;
	int height=20;
	int width=50;
	int scale=5;
	int toppoints[8] = {scale*10+x, scale*0+y, scale*20+x, scale*8+y, scale*10+x, scale*16+y, scale*0+x, scale*8+y};
	int rightpoints[8] = {scale*20+x, scale*8+y,scale*20+x, scale*11+y,scale*10+x, scale*19+y,scale*10+x, scale*16+y};
	int leftpoints[8] = {scale*10+x, scale*16+y, scale*10+x, scale*19+y, scale*0+x, scale*11+y, scale*0+x, scale*8+y};
	pd->graphics->fillPolygon(4,toppoints,LCD_PATTERN_DITHER_GREY30,kPolygonFillEvenOdd);
	pd->graphics->fillPolygon(4,rightpoints,LCD_PATTERN_DITHER_GREY75,kPolygonFillEvenOdd);
	pd->graphics->fillPolygon(4,leftpoints,LCD_PATTERN_DITHER_GREY45,kPolygonFillEvenOdd);
}