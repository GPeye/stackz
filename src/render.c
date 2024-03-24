#include "pd_api.h"
#include "render.h"
#include "lcdpatterns.h"

static PlaydateAPI *pd;
static int cuboidFacePoints1[] = {0,1,2,3};
static int cuboidFacePoints2[] = {4,5,6,7};
static int cuboidFacePoints3[] = {0,1,5,4};
static int cuboidFacePoints4[] = {6,7,3,2};
static int cuboidFacePoints5[] = {5,1,2,6};
static int cuboidFacePoints6[] = {0,4,7,3};


static float faceZIndex(float projectedPoints[8][3], int facepoints[4])
{
    return (projectedPoints[facepoints[0]][2]+
        projectedPoints[facepoints[1]][2]+
        projectedPoints[facepoints[2]][2]+
        projectedPoints[facepoints[3]][2]) /4;
}

static floatFacePoints(float projectedPoints[8][3], int facepoints[4], int *points)
{
    int tpoints[12] = {
        projectedPoints[facepoints[0]][0],
        projectedPoints[facepoints[0]][1],
        projectedPoints[facepoints[1]][0],
        projectedPoints[facepoints[1]][1],
        projectedPoints[facepoints[2]][0],
        projectedPoints[facepoints[2]][1],
        projectedPoints[facepoints[3]][0],
        projectedPoints[facepoints[3]][1]
    };
    memcpy(points, tpoints, sizeof(tpoints));
}

static void getFacesFromPoints(struct CuboidFace *face, float projectedPoints[8][3], int cuboidFacePoints[4])
{
    face->zindex = faceZIndex(projectedPoints, cuboidFacePoints);
    floatFacePoints(projectedPoints, cuboidFacePoints, face->points);
}

static int compare(const void *s1, const void *s2)
{
    struct CuboidFace *e1 = (struct CuboidFace *)s1;
    struct CuboidFace *e2 = (struct CuboidFace *)s2;
    if(e1->zindex > e2->zindex)
        return 1;
    else
        return -1;
}


//static void render

static void _translate(float x, float y, float z, float *arr)
{
	arr[0] = arr[0] + x;
	arr[1] = arr[1] + y;
	arr[2] = arr[2] + z;
}

static void _appyRotationZ(float *m2, float rotation)
{
	float one = 0.f;
	float two = 0.f;
	float three = 0.f;

	one += cosf(rotation) * m2[0];
	one += -sinf(rotation) * m2[1];
	one += 0 * m2[2];

	two += sinf(rotation) * m2[0];
	two += cosf(rotation) * m2[1];
	two += 0 * m2[2];

	three += 0 * m2[0];
	three += 0 * m2[1];
	three += 1 * m2[2];

	m2[0] = one;
	m2[1] = two;
	m2[2] = three;
}

static void _appyRotationX(float *m2, float rotation)
{
	float one = 0.f;
	float two = 0.f;
	float three = 0.f;

	one += 1 * m2[0];
	one += 0 * m2[1];
	one += 0 * m2[2];

	two += 0 * m2[0];
	two += cosf(rotation) * m2[1];
	two += -sinf(rotation) * m2[2];

	three += 0 * m2[0];
	three += sinf(rotation) * m2[1];
	three += cosf(rotation) * m2[2];

	m2[0] = one;
	m2[1] = two;
	m2[2] = three;
}

static void _appyRotationY(float *m2, float rotation)
{
	float one = 0.f;
	float two = 0.f;
	float three = 0.f;

	one += cosf(rotation) * m2[0];
	one += 0 * m2[1];
	one += sinf(rotation) * m2[2];

	two += 0 * m2[0];
	two += 1 * m2[1];
	two += 0 * m2[2];

	three += -sinf(rotation) * m2[0];
	three += 0 * m2[1];
	three += cosf(rotation) * m2[2];

	m2[0] = one;
	m2[1] = two;
	m2[2] = three;
}

static void _matmul(float *m2)
{
	float z = 1.f / ( 2-m2[2] );
	float one = z * m2[0];
	float two = z * m2[1];

	m2[0] = one;
	m2[1] = two;
}

static void _scale(float *arr, float *width)
{
	//float scaleamt = (mine->width / SCREEN_WIDTH);
	arr[0] = arr[0] * *width * 2;
	arr[1] = arr[1] * *width * 2;
	arr[2] = arr[2] * *width * 2;
}

static void connectPoints(int i, int j, float projectedPoints[8][3])
{
	float* a = projectedPoints[i];
	float* b = projectedPoints[j];
	pd->graphics->drawLine((int)a[0],(int)a[1],(int)b[0],(int)b[1],3,kColorBlack);
}

void DrawBackground(PlaydateAPI *pd)
{
    pd->graphics->clear(LCD_PATTERN_DITHER_GREY35);
    pd->graphics->fillEllipse(-40, -60, 480, 360, 0.0, 0.0, LCD_PATTERN_DITHER_GREY30);
    pd->graphics->fillEllipse(-30, -50, 460, 340, 0.0, 0.0, LCD_PATTERN_DITHER_GREY25);
    pd->graphics->fillEllipse(-20, -40, 440, 320, 0.0, 0.0, LCD_PATTERN_DITHER_GREY20);
    pd->graphics->fillEllipse(-10, -30, 420, 300, 0.0, 0.0, LCD_PATTERN_DITHER_GREY15);
    pd->graphics->fillEllipse(0, -20, 400, 280, 0.0, 0.0, LCD_PATTERN_DITHER_GREY10);
    pd->graphics->fillEllipse(10, -10, 380, 260, 0.0, 0.0, LCD_PATTERN_DITHER_GREY05);
    pd->graphics->fillEllipse(20, 0, 360, 240, 0.0, 0.0, kColorWhite);
}

void drawCube(PlaydateAPI *pd, int osc, int osc2)
{
    int x = 160 + (int)(osc * 1.5f);
    int y = 80 - (int)(osc2 * 1.4f);
    int height = 20;
    int width = 50;
    int scale = 5;
    int toppoints[8] = {scale * 10 + x, scale * 0 + y, scale * 20 + x, scale * 8 + y, scale * 10 + x, scale * 16 + y, scale * 0 + x, scale * 8 + y};
    int rightpoints[8] = {scale * 20 + x, scale * 8 + y, scale * 20 + x, scale * 11 + y, scale * 10 + x, scale * 19 + y, scale * 10 + x, scale * 16 + y};
    int leftpoints[8] = {scale * 10 + x, scale * 16 + y, scale * 10 + x, scale * 19 + y, scale * 0 + x, scale * 11 + y, scale * 0 + x, scale * 8 + y};
    pd->graphics->fillPolygon(4, toppoints, LCD_PATTERN_DITHER_GREY30, kPolygonFillEvenOdd);
    pd->graphics->fillPolygon(4, rightpoints, LCD_PATTERN_DITHER_GREY75, kPolygonFillEvenOdd);
    pd->graphics->fillPolygon(4, leftpoints, LCD_PATTERN_DITHER_GREY45, kPolygonFillEvenOdd);
}

void draw1Cube(PlaydateAPI *pd)
{
    int x = 160;
    int y = 94;
    int height = 20;
    int width = 50;
    int scale = 5;
    int toppoints[8] = {scale * 10 + x, scale * 0 + y, scale * 20 + x, scale * 8 + y, scale * 10 + x, scale * 16 + y, scale * 0 + x, scale * 8 + y};
    int rightpoints[8] = {scale * 20 + x, scale * 8 + y, scale * 20 + x, scale * 11 + y, scale * 10 + x, scale * 19 + y, scale * 10 + x, scale * 16 + y};
    int leftpoints[8] = {scale * 10 + x, scale * 16 + y, scale * 10 + x, scale * 19 + y, scale * 0 + x, scale * 11 + y, scale * 0 + x, scale * 8 + y};
    pd->graphics->fillPolygon(4, toppoints, LCD_PATTERN_DITHER_GREY30, kPolygonFillEvenOdd);
    pd->graphics->fillPolygon(4, rightpoints, LCD_PATTERN_DITHER_GREY75, kPolygonFillEvenOdd);
    pd->graphics->fillPolygon(4, leftpoints, LCD_PATTERN_DITHER_GREY45, kPolygonFillEvenOdd);
}

void DrawCuboidWireframe(PlaydateAPI *pd, Cuboid *cuboid, Camera *cam)
{
    // float x = cuboid->x;
    // float y = cuboid->y;
    // float z = cuboid->z;

    // float projectedPoints[8][3] = {0};
    // int ps=0;
	// while(ps <= 7)
	// {
	// 	float pp[3]= {cuboid->points[ps][0],cuboid->points[ps][1],cuboid->points[ps][2]};
	// 	_appyRotationX(&pp,cuboid->rotation_x);
	// 	_appyRotationY(&pp,cuboid->rotation_y);
	// 	_appyRotationZ(&pp,cuboid->rotation_z);
	// 	_matmul(&pp);

	// 	//pd->system->logToConsole("pp: %f",pp[0]);
	// 	_scale(&pp, cuboid->width);
	// 	//applySize(&pp);
	// 	_translate(400, 240, 0.f, &pp);
		

	// 	pd->graphics->drawEllipse(pp[0], pp[1], 3,3,3,0,0,kColorBlack);
	// 	projectedPoints[ps][0] = pp[0];
	// 	projectedPoints[ps][1] = pp[1];
	// 	projectedPoints[ps][2] = pp[2];
	// 	ps++;
	// }
}

void DrawCuboid(PlaydateAPI *_pd, Cuboid *cuboid, Camera *cam)
{
    pd = _pd;
    float x = cuboid->x;
    float y = cuboid->y;
    float z = cuboid->z;
    UpdateCuboidPoints(cuboid);

    float projectedPoints[8][3] = {
        {-0.5,-0.5,-0.5},{0.5,-0.5,-0.5},{0.5,0.5,-0.5},{-0.5,0.5,-0.5},
        {-0.5,-0.5,0.5},{0.5,-0.5,0.5},{0.5,0.5,0.5},{-0.5,0.5,0.5}
    };
    int ps=0;
	while(ps <= 7)
	{
		float pp[3]= {cuboid->points[ps][0],cuboid->points[ps][1],cuboid->points[ps][2]};

		_appyRotationX(&pp,cuboid->rotation_x);
		_appyRotationY(&pp,cuboid->rotation_y);
		_appyRotationZ(&pp,cuboid->rotation_z);
		_matmul(&pp);

		_scale(&pp, &cuboid->scaleFactor);
        
		_translate(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 0.f, &pp);
		

		//pd->graphics->drawEllipse(pp[0], pp[1], 3,3,3,0,0,kColorBlack);
		projectedPoints[ps][0] = pp[0];
		projectedPoints[ps][1] = pp[1];
		projectedPoints[ps][2] = pp[2];
		ps++;
	}

    int i;
	for (i = 0; i < 4; i++)
	{
		connectPoints(i, (i + 1) % 4, projectedPoints);
		connectPoints(i + 4, ((i + 1) % 4) + 4, projectedPoints);
		connectPoints(i, i + 4, projectedPoints);
	}

    CuboidFace faces[6];
    faces[0].color = kColorBlack;
    faces[1].color = LCD_PATTERN_DITHER_GREY20;
    faces[2].color = LCD_PATTERN_DITHER_GREY70;
    faces[3].color = LCD_PATTERN_DITHER_GREY45;
    faces[4].color = LCD_PATTERN_DITHER_GREY55;
    faces[5].color = LCD_PATTERN_DITHER_GREY35;
    getFacesFromPoints(&faces[0],projectedPoints,cuboidFacePoints1);
    getFacesFromPoints(&faces[1],projectedPoints,cuboidFacePoints2);
    getFacesFromPoints(&faces[2],projectedPoints,cuboidFacePoints3);
    getFacesFromPoints(&faces[3],projectedPoints,cuboidFacePoints4);
    getFacesFromPoints(&faces[4],projectedPoints,cuboidFacePoints5);
    getFacesFromPoints(&faces[5],projectedPoints,cuboidFacePoints6);

    qsort(faces, 6, sizeof(struct CuboidFace), compare);
    
    float avgz = (faces[0].zindex+faces[1].zindex+faces[2].zindex+faces[3].zindex+faces[4].zindex+faces[5].zindex)/6;
    for(int i=0;i<6;i++)
    {
        if(faces[i].zindex > avgz)
            pd->graphics->fillPolygon(4,faces[i].points,faces[i].color,0);
    }
    
}



// void DrawCuboid(PlaydateAPI *pd, Cuboid *cuboid, Camera *cam)
// {
//     float x = cuboid->x;
//     float y = cuboid->y;
//     float z = cuboid->z;

//     Face f1 = {
//         .x = x,
//         .y = y,
//         .z = z,
//         .width = cuboid->width,
//         .height = cuboid->height,
//         .depth = 0,
//     };
//     Face f2 = {
//         .x = x + cuboid->width,
//         .y = y,
//         .z = z,
//         .width = 0,
//         .height = cuboid->height,
//         .depth = cuboid->depth,
//     };
//     Face f3 = {
//         .x = x + cuboid->width,
//         .y = y,
//         .z = z + cuboid->depth,
//         .width = -cuboid->width,
//         .height = cuboid->height,
//         .depth = 0,
//     };
//     Face f4 = {
//         .x = x,
//         .y = y,
//         .z = z + cuboid->depth,
//         .width = 0,
//         .height = cuboid->height,
//         .depth = -cuboid->depth,
//     };

//     switch (cuboid->type)
//     {
//     case 0:
//         //pd->system->logToConsole("trying to draw faces");
//         DrawFaceBlack(pd, &f1, cam);
//         DrawFaceTest(pd, &f2, cam);
//         DrawFaceBlack(pd, &f3, cam);
//         DrawFaceTest(pd, &f4, cam);
//         break;
//     case 1:
//         DrawFaceTest(pd, &f1, cam);
//         DrawFaceBlack(pd, &f2, cam);
//         DrawFaceTest(pd, &f3, cam);
//         DrawFaceBlack(pd, &f4, cam);
//         break;
//     case 2:
//         DrawFaceTest(pd, &f1, cam);
//         DrawFaceTiled(pd, &f2, cam);
//         DrawFaceTest(pd, &f3, cam);
//         DrawFaceTiled(pd, &f4, cam);
//         break;
//     case 3:
//         DrawFaceTiled(pd, &f1, cam);
//         DrawFaceTiledDark(pd, &f2, cam);
//         DrawFaceTiled(pd, &f3, cam);
//         DrawFaceTiledDark(pd, &f4, cam);
//         break;
//     }
// }

// int GetFaceTransformed(Face* face, Camera* cam, int* x, int* width, int* z) {
// 	float newFaceXMin = face->x - cam->x;
// 	float newFaceZMin = face->z - cam->z;

// 	float newFaceXMax = newFaceXMin + face->width;
// 	float newFaceZMax = newFaceZMin + face->depth;

// 	float mat[2][2] = { { cosf(-cam->angle), -sinf(-cam->angle) },
// 						{ sinf(-cam->angle), cosf(-cam->angle) } };

// 	int finalFaceZMin = mat[0][0] * newFaceZMin + mat[0][1] * newFaceXMin;
// 	if (finalFaceZMin < 0) return 0;
// 	int finalFaceZMax = mat[0][0] * newFaceZMax + mat[0][1] * newFaceXMax;
// 	if (finalFaceZMax < 0) return 0;

// 	int finalFaceXMin = mat[1][0] * newFaceZMin + mat[1][1] * newFaceXMin;
// 	int finalFaceXMax = mat[1][0] * newFaceZMax + mat[1][1] * newFaceXMax;
// 	float newWidth = finalFaceXMax - finalFaceXMin;
// 	if (newWidth <= 0) return 0;

// 	*x = finalFaceXMin;
// 	*width = newWidth;
// 	if (z != NULL) *z = finalFaceZMin;
// 	return 1;
// }

// void DrawFace(PlaydateAPI* pd, Face* face, Camera* cam) {
// 	int x = 0, width = 0;
// 	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
// 	pd->graphics->drawRect(x, face->y - cam->y, width, face->height, kColorBlack);
// }

// void DrawFaceBlack(PlaydateAPI* pd, Face* face, Camera* cam) {
// 	int x = 0, width = 0;
// 	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
// 	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, kColorBlack);
// }

// void DrawFaceCheckered(PlaydateAPI* pd, Face* face, Camera* cam) {
// 	int x = 0, width = 0;
// 	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
// 	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, (void*)LCD_PATTERN_CHECKER_LG);
// }

// void DrawFaceTest(PlaydateAPI* pd, Face* face, Camera* cam) {
// 	int x = 0, width = 0;
// 	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
// 	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, LCD_PATTERN_H_STRIPES_THIN);
// }

// void DrawFaceTiled(PlaydateAPI* pd, Face* face, Camera* cam) {
// 	int x = 0, width = 0;
// 	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
// 	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, (void*)LCD_PATTERN_DIAG1_THICK);
// }

// void DrawFaceTiledDark(PlaydateAPI* pd, Face* face, Camera* cam)
// {
// 	int x = 0, width = 0;
// 	if (!GetFaceTransformed(face, cam, &x, &width, NULL)) return;
// 	pd->graphics->fillRect(x, face->y - cam->y, width, face->height, (void*)LCD_PATTERN_DITHER_GREY80);
// }