#include "stackz.h"

#include "lcdpatterns.h"
#include "mini3d.h"
#include "3dmath.h"
#include "shape.h"
#include "scene.h"
#include "3dhelper.h"
#include "float.h"

#define BORDER

static Scene3D *scene;
static Scene3DNode *rootNode;
static Scene3DNode *activeNode;
static Scene3DNode *activeNodeSubnode;
static Scene3DNode *stackNode;

static Shape3D *activeBox;
static Shape3D *stackBoxes[STACKMAX];

static Matrix3D *stackNodeMatrix;
static RenderStyle globalStyle;

static float leftrightrotation = 45.f;

static float activeBoxWidth = 1.f;
static float activeBoxDepth = 1.f;
static float activeBoxX = 0.f;
static float activeBoxZ = 0.f;
static float targetBoxWidth = 1.f;
static float targetBoxDepth = 1.f;
static float targetBoxX = 0.f;
static float targetBoxZ = 0.f;
static float targetBoxXScale = 1.f;
static float targetBoxZScale = 1.f;
static int isFirstLoop = 0;
static int direction = 0;

static float zoom = 5.f;

static void getIntersectingBox() {
    float size;
    float targetsize;
    if(direction == 0) {
        size = activeBoxDepth;
        targetsize = targetBoxDepth;
    }
    else {
        size = activeBoxWidth;
        targetsize = targetBoxWidth;
    }



}

float getColorFromIndex(int index) {
    float adjustedIndex = (index + 10) * 0.4f;
    return (sinf((float)adjustedIndex) * 0.5f) - 0.3f;
    int x = FLT_MAX;
}

Vector3D v = { 0.f,0.f,0.f };
float addv = 0;
static void addBoxToStack(float x, float z, float scalex, float scalez) {
    Game.StackzData.currentNode = Game.StackzData.currentNode->next;
    node_resetTranform(Game.StackzData.currentNode->scene3DNode);
    Game.StackzData.currentNode->scene3DNode->isVisible = 1;
    Game.StackzData.currentNode->scene3DNode->colorBias = getColorFromIndex(Game.StackzData.stackBoxIndex);
    addv += 0.5f;
    matrix_scaleByAndAddTranslation(Game.StackzData.currentNode->scene3DNode, scalex, 1.f, scalez, x, addv, z);
    //Game.StackzData.stackNodeMatrix = matrix_addTranslation(x, addv, z);
    //Scene3DNode_setTransform(Game.StackzData.currentNode->scene3DNode, stackNodeMatrix);
    //node_scaleBy(Game.StackzData.currentNode->scene3DNode, scalex, 1.f, scalez);
    Game.StackzData.stackNodeMatrix = matrix_addTranslation(0.f, -0.5f, 0.f);
    Scene3DNode_addTransform(stackNode, stackNodeMatrix);

    // Game.StackzData.currentBox = Game.StackzData.currentBox->next;
    // Game.StackzData.currentBox->stackBox->colorBias = getColorFromIndex(Game.StackzData.stackBoxIndex);
    // //Game.StackzData.currentBox->stackBox
    // Game.StackzData.currentBox->stackBox->points->y = 0;
    // //Scene3D_updateShapeInstance(scene, Game.StackzData.currentBox->stackBox, identityMatrix, getColorFromIndex(Game.StackzData.stackBoxIndex), globalStyle);
    // //stackBoxes[Game.StackzData.stackBoxIndex] = Game.StackzData.stackBoxes[Game.StackzData.stackBoxIndex] = shape_new_cuboid(1.f,0.25f,1.f,-0.8f);
    // //stackBoxes[Game.StackzData.stackBoxIndex] = Game.StackzData.stackBoxes[Game.StackzData.stackBoxIndex] = shape_new_cuboid(1.f,0.25f,1.f,getColorFromIndex(Game.StackzData.stackBoxIndex));
    // //v.dy += 0.5f;
    // //Scene3DNode_addShapeWithOffset(stackNode, stackBoxes[Game.StackzData.stackBoxIndex], v);
    // //Scene3DNode_addShape(stackNode, stackBoxes[Game.StackzData.stackBoxIndex]);
    // Game.StackzData.stackNodeMatrix = matrix_addTranslation(0,-0.5,0);
    // stackNodeMatrix = &Game.StackzData.stackNodeMatrix;
    // Scene3DNode_addTransform(stackNode, stackNodeMatrix);
    Game.StackzData.stackBoxIndex++;
}

static void updateActiveBlockSize(float x, float z, float scalex, float scalez) {
    node_resetTranform(activeNodeSubnode);
    matrix_scaleByAndAddTranslation(activeNodeSubnode, scalex, 1.f, scalez, x, 0.f, z);
    //node_scaleBy(activeNodeSubnode, scalex, 1.f, scalez);
}

static void initSceneAndCamera() {
    scene = Game.StackzData.scene = scene_new();
    scene_setCameraOrigin(scene, 0.f, zoom*1.f, zoom*1.2);
	scene_setLight(scene, 0.2f, 0.8f, 0.4f);
}

static void initNodes() {
    rootNode = Game.StackzData.rootNode = Scene3D_getRootNode(scene);
    activeNode = Game.StackzData.activeNode = Scene3DNode_newChild(rootNode);
    activeNodeSubnode = Scene3DNode_newChild(activeNode);
    stackNode = Game.StackzData.stackNode = Scene3DNode_newChild(rootNode);
    matrix_updateRotation(Game.StackzData.crankMatrix, leftrightrotation, 0.f, 1.f, 0.f);
    Scene3DNode_addTransform(rootNode, Game.StackzData.crankMatrix);
#ifdef BORDER
    RenderStyle style = Scene3DNode_getRenderStyle(rootNode);
    style |= kRenderWireframe;
    globalStyle = style;
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
    activeBox = Game.StackzData.activeBox = shape_new_cuboid(1.f,0.25f,1.f,-0.1f);
	Scene3DNode_addShape(activeNodeSubnode, activeBox);
}

static void initStack() {
    if(Game.StackzData.lastNode == NULL) {
        sys->logToConsole("List is empty");
        return;
    }
    if(Game.StackzData.lastNode->next != Game.StackzData.firstNode) {
        sys->logToConsole("List is not circular empty");
        return;
    }
    struct Node* ptr = Game.StackzData.firstNode;
    do {
        //create shape
        ptr->scene3DNode = Game.StackzData.stackNode = Scene3DNode_newChild(stackNode);
        Shape3D *shape = shape_new_cuboid(1.f,0.25f,1.f,0.0f);
        Scene3DNode_addShape(ptr->scene3DNode, shape);
        Scene3DNode_setVisible(ptr->scene3DNode, 0);

        ptr = ptr->next;
    } while (ptr != Game.StackzData.lastNode->next);
    Game.StackzData.currentNode = Game.StackzData.firstNode;

    Scene3DNode_setVisible(ptr->scene3DNode, 1);
    // Scene3DNode_addShape(stackNode, Game.StackzData.currentBox->stackBox);
    Game.StackzData.stackNodeMatrix = matrix_addTranslation(0,-0.5,0);
    stackNodeMatrix = &Game.StackzData.stackNodeMatrix;
    Scene3DNode_addTransform(stackNode, stackNodeMatrix);
}

static void resetStack() {
    struct Node* ptr = Game.StackzData.firstNode;
    do {
        Scene3DNode_setVisible(ptr->scene3DNode, 0);
        ptr = ptr->next;
    } while (ptr != Game.StackzData.lastNode->next);
    addBoxToStack(0.f, 0.f, 1.f, 1.f);
    
}


static void resetGame() {
    Game.StackzData.score = 0;
    Game.StackzData.stackBoxIndex = 0;
    sys->resetElapsedTime();
    direction = 0;
    activeBoxDepth = 1.f;
    activeBoxWidth = 1.f;
    targetBoxDepth = 1.f;
    targetBoxWidth = 1.f;
    targetBoxX = 0.f;
    targetBoxZ = 0.f;
    targetBoxXScale = 1.f;
    targetBoxZScale = 1.f;
    node_resetTranform(activeNodeSubnode);
    resetStack();
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
    if(isFirstLoop == 0)
    {
        isFirstLoop = 1;
		sys->resetElapsedTime();
		// pd->graphics->drawText("69", strlen("69"), kASCIIEncoding, x, y);
	    //sound->synth->playNote(projectile_sound, 700.f, 1.f, 0.1f, 0);
	}
}

static void buttonUp() {
    sys->logToConsole("Up pressed");
}

static void buttonDown() {
    sys->logToConsole("Down pressed");
}

static void buttonLeft() {
    sys->logToConsole("Left pressed");
    leftrightrotation = 10.f;
    matrix_updateRotation(Game.StackzData.crankMatrix, leftrightrotation, 0.f, 1.f, 0.f);
    Scene3DNode_addTransform(rootNode, Game.StackzData.crankMatrix);
}

static void buttonRight() {
    sys->logToConsole("Right pressed");
    leftrightrotation = -10.f;
    matrix_updateRotation(Game.StackzData.crankMatrix, leftrightrotation, 0.f, 1.f, 0.f);
    Scene3DNode_addTransform(rootNode, Game.StackzData.crankMatrix);
}

static void buttonA() {
    addBoxToStack(0.f,0.f,1.f,1.f);
    sys->logToConsole("A pressed");
}

static void buttonB() {
    sys->logToConsole("B pressed");
    if (direction == 0) {
        if (Game.StackzData.activeOscillator > (targetBoxDepth * 2.f) + targetBoxX || Game.StackzData.activeOscillator < -(targetBoxDepth*2.f)+targetBoxX) {
            resetGame();
            return;
        }
        sys->logToConsole("hit");
        Game.StackzData.score++;

        float difference = Game.StackzData.activeOscillator - targetBoxX;
        if (difference < 0.f)
            difference = targetBoxX - Game.StackzData.activeOscillator;

        float pdiff = (difference / (targetBoxDepth * 2.f));
        float scale = 1.f - pdiff;
        targetBoxXScale *= scale;
        targetBoxX = (Game.StackzData.activeOscillator / 2.f) + (targetBoxX / 2.f);

        addBoxToStack(targetBoxX, -targetBoxZ, targetBoxXScale, targetBoxZScale);

        targetBoxDepth -= (difference/2.f);
        
        updateActiveBlockSize(targetBoxX, -targetBoxZ, targetBoxXScale, targetBoxZScale);

    }
    else {
        if (Game.StackzData.activeOscillator > (targetBoxWidth * 2.f) + targetBoxZ || Game.StackzData.activeOscillator < -(targetBoxWidth * 2.f) + targetBoxZ) {
            resetGame();
            return;
        }
        sys->logToConsole("hit");
        Game.StackzData.score++;

        float difference = Game.StackzData.activeOscillator - targetBoxZ;
        if (difference < 0.f)
            difference = targetBoxZ - Game.StackzData.activeOscillator;
        float pdiff = (difference / (targetBoxDepth * 2.f));
        float scale = 1.f - pdiff;
        targetBoxZScale *= scale;
        targetBoxZ = (Game.StackzData.activeOscillator / 2.f) + (targetBoxZ / 2.f);

        addBoxToStack(targetBoxX, -targetBoxZ, targetBoxXScale, targetBoxZScale);

        targetBoxWidth -= (difference / 2.f);

        updateActiveBlockSize(targetBoxX, -targetBoxZ, targetBoxXScale, targetBoxZScale);
    }

    if (direction == 0)
        direction = 1;
    else
        direction = 0;
    sys->resetElapsedTime();
    
    return;

    if(Game.StackzData.activeOscillator > 2.f || Game.StackzData.activeOscillator < -2.f){
        sys->logToConsole("missed\r");
        resetGame();
    } else {
        if (Game.StackzData.activeOscillator < 0.1f && Game.StackzData.activeOscillator > -0.1f)
            sys->logToConsole("perfect\r");

        sys->logToConsole("hit\r");
        
        sys->resetElapsedTime();
        Game.StackzData.score++;

        float difference = Game.StackzData.activeOscillator;
        sys->logToConsole("difference: %f\r", Game.StackzData.activeOscillator);

        
        
        if (direction == 0) {
            float scale = (activeBoxDepth - (fabsf(difference)/2.f)) / activeBoxDepth;
            addBoxToStack((Game.StackzData.activeOscillator / 2), 0.f, scale, 1.f);
            //addBoxToStack(0.f, 0.f, scale, 1.f);
        }
        else {
            float scale = (activeBoxWidth - (fabsf(difference) / 2.f)) / activeBoxWidth;
            addBoxToStack(0.f, (-Game.StackzData.activeOscillator / 2), 1.f, scale);
            //addBoxToStack(0.f, 0.f, 1.f, scale);
        }

        
    }

}

static PDButtons pushed;
static void handleButtonPush() {
	sys->getButtonState(NULL, &pushed, NULL);
	
	if ( pushed & kButtonUp )
		buttonUp();
    if ( pushed & kButtonDown )
		buttonDown();
    if ( pushed & kButtonLeft )
		buttonLeft();
    if ( pushed & kButtonRight )
		buttonRight();
    if ( pushed & kButtonA )
		buttonA();
    if ( pushed & kButtonB )
		buttonB();
}

static void setupOscillator() {
    Game.StackzData.ellapsed = (sys->getElapsedTime() +2.f) * 1.f;
    Game.StackzData.activeOscillator = sinf(Game.StackzData.ellapsed)*3.f;
}

static void OscillateActiveNode() {
    if (direction == 0) {
        Game.StackzData.activeNodeMatrix = matrix_addTranslation(Game.StackzData.activeOscillator,0.f,0.f);
        Scene3DNode_setTransform(activeNode, &Game.StackzData.activeNodeMatrix);
    } else {
        Game.StackzData.activeNodeMatrix = matrix_addTranslation(0.f, 0.f, -Game.StackzData.activeOscillator);
        Scene3DNode_setTransform(activeNode, &Game.StackzData.activeNodeMatrix);
    }
}

static void draw() {
    Scene3D_draw(Game.StackzData.scene, gfx->getFrame(), LCD_ROWSIZE);
	gfx->markUpdatedRows(0, LCD_ROWS-1);
}
static char* score;
static int ww = 0;
int scorewidth = 0;
static void displayScore() {
    ww = sys->formatString(&score,"%d",Game.StackzData.score);
     //= sprintf("%d", Game.StackzData.score);
    scorewidth = gfx->getTextWidth(Game.font, score, strlen(score),kASCIIEncoding,0);
    gfx->drawText(score, strlen(score), kASCIIEncoding, SCREEN_WIDTH/2-(scorewidth/2), 15);
    
    sys->realloc(score, 0);
    //gfx->drawText("0", strlen("0"), kASCIIEncoding, SCREEN_WIDTH/2, 15);
}

static void rotateRootNodeWithCrank() {
    Game.StackzData.crankChange = sys->getCrankChange() / 50.f;
    zoom -= Game.StackzData.crankChange;
    if (zoom > 10.f)
        zoom = 10.f;
    if (zoom < 2.f)
        zoom = 2.f;
    scene_setCameraOrigin(scene, 0.f, zoom * 1.f, zoom * 1.2);
    //matrix_updateRotation(Game.StackzData.crankMatrix, Game.StackzData.crankChange,1.f,0.f,0.f);
	//Scene3DNode_addTransform(rootNode, Game.StackzData.crankMatrix);
}


void updateStackz() {
    //firstLoop();
    drawBackground();
    handleButtonPush();
    
    setupOscillator();
	OscillateActiveNode();

    rotateRootNodeWithCrank();

    displayScore();

    //getxyz(Game.StackzData.activeNode);
    //sys->logToConsole("centerx: %f",);
    
    draw();
}