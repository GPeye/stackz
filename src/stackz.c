#include "stackz.h"

#include "lcdpatterns.h"
#include "mini3d.h"
#include "3dmath.h"
#include "shape.h"
#include "scene.h"
#include "3dhelper.h"

#define BORDER

static Scene3D *scene;
static Scene3DNode *rootNode;
static Scene3DNode *activeNode;
static Scene3DNode *stackNode;

static Shape3D *activeBox;
static Shape3D *stackBoxes[100];

static Matrix3D *stackNodeMatrix;


static void initSceneAndCamera() {
    scene = Game.StackzData.scene = scene_new();
    scene_setCameraOrigin(scene, 0.f, 5.f, 6.f);
	scene_setLight(scene, 0.2f, 0.8f, 0.4f);
}

static void initNodes() {
    rootNode = Game.StackzData.rootNode = Scene3D_getRootNode(scene);
    activeNode = Game.StackzData.activeNode = Scene3DNode_newChild(rootNode);
    stackNode = Game.StackzData.stackNode = Scene3DNode_newChild(rootNode);
#ifdef BORDER
    RenderStyle style = Scene3DNode_getRenderStyle(rootNode);
    style |= kRenderWireframe;
    Scene3DNode_setRenderStyle(rootNode,style);
#endif
}

static void initDataValues() {
    mini3d_setRealloc(sys->realloc);
    Game.StackzData.ellapsed=0.f;
    Game.StackzData.activeOscillator=0.f;
    Game.StackzData.score=0;
    Game.StackzData.crankMatrix = matrix_new();
}

static void initActiveBox() {
    activeBox = Game.StackzData.activeBox = shape_new_cuboid(1.f,0.2f,1.f,-0.1f);
	Scene3DNode_addShape(activeNode, activeBox);
}

static void initStack() {
    stackBoxes[0] = Game.StackzData.stackBoxes[0] = shape_new_cuboid(1.f,0.2f,1.f,-0.2f);
	Scene3DNode_addShape(stackNode, stackBoxes[0]);
    Game.StackzData.stackNodeMatrix = matrix_addTranslation(0,-0.38,0);
    stackNodeMatrix = &Game.StackzData.stackNodeMatrix;
    Scene3DNode_addTransform(stackNode, stackNodeMatrix);
}

// Scene Init
void InitStackzSceneData() {
    initDataValues();
    initSceneAndCamera();
    initNodes();
    initActiveBox();
    initStack();
}

static void drawBackground()
{
    gfx->clear(LCD_PATTERN_DITHER_GREY60);
    gfx->fillEllipse(-45, -65, 490, 370, 0.0, 0.0, LCD_PATTERN_DITHER_GREY50_ALT1);
    gfx->fillEllipse(-40, -60, 480, 360, 0.0, 0.0, LCD_PATTERN_DITHER_GREY45);
    gfx->fillEllipse(-35, -55, 470, 350, 0.0, 0.0, LCD_PATTERN_DITHER_GREY40);
    gfx->fillEllipse(-30, -50, 460, 340, 0.0, 0.0, LCD_PATTERN_DITHER_GREY35);
    gfx->fillEllipse(-25, -45, 450, 330, 0.0, 0.0, LCD_PATTERN_DITHER_GREY30);
    gfx->fillEllipse(-20, -40, 440, 320, 0.0, 0.0, LCD_PATTERN_DITHER_GREY25);
    gfx->fillEllipse(-15, -35, 430, 310, 0.0, 0.0, LCD_PATTERN_DITHER_GREY20);
    gfx->fillEllipse(-10, -30, 420, 300, 0.0, 0.0, LCD_PATTERN_DITHER_GREY15);
    gfx->fillEllipse(0, -20, 400, 280, 0.0, 0.0, LCD_PATTERN_DITHER_GREY10);
    gfx->fillEllipse(10, -10, 380, 260, 0.0, 0.0, LCD_PATTERN_DITHER_GREY05);
    gfx->fillEllipse(20, 0, 360, 240, 0.0, 0.0, kColorWhite);
}

static void firstLoop() {
    if(Game.StackzData.ellapsed == 0.0f)
	{
		sys->resetElapsedTime();
		// pd->graphics->setFont(font);
		// pd->graphics->drawText("69", strlen("69"), kASCIIEncoding, x, y);
		//sound->synth->playNote(projectile_sound, 700.f, 1.f, 0.1f, 0);
	}
}

static void setupOscillator() {
    Game.StackzData.ellapsed = sys->getElapsedTime();
    Game.StackzData.activeOscillator = sinf(Game.StackzData.ellapsed)*5.f;
}

static void OscillateActiveNode() {
    Game.StackzData.activeNodeMatrix = matrix_addTranslation(Game.StackzData.activeOscillator,0,0);
    Scene3DNode_setTransform(activeNode, &Game.StackzData.activeNodeMatrix);
}

static void draw() {
    Scene3D_draw(Game.StackzData.scene, gfx->getFrame(), LCD_ROWSIZE);
	gfx->markUpdatedRows(0, LCD_ROWS-1);
}

static void displayScore() {
    gfx->drawText("0", strlen("0"), kASCIIEncoding, SCREEN_WIDTH/2.f-10.f, 15.f);
}

static void rotateRootNodeWithCrank() {
    Game.StackzData.crankChange = sys->getCrankChange();
    matrix_updateRotation(Game.StackzData.crankMatrix, Game.StackzData.crankChange,0.f,0.f,1.f);
	Scene3DNode_addTransform(rootNode, Game.StackzData.crankMatrix);
}


void updateStackz() {
    drawBackground();
    firstLoop();
    
    setupOscillator();
	OscillateActiveNode();

    rotateRootNodeWithCrank();

    displayScore();
    
    draw();
}