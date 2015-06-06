/* main file */

// ---------------------------------------------------
// Includes
// ---------------------------------------------------
#include <nds.h>
#include <filesystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "bkdm.h"
#include "camera.h"
#include "cameraStruct.h"
#include "player.h"
#include "playerStruct.h"

// ---------------------------------------------------
// Variables
// ---------------------------------------------------

// camera related
char cameraMode = 1; // 0 = debug, 1 = game

// struct related
struct MODEL * levelModel;
struct CAMERA * camera;
struct PLAYER * player;

// ---------------------------------------------------
// Functions
// ---------------------------------------------------

// print functions
void debugPrint()
{
	consoleClear();
	printf(" \n");
	printf(" Camera: \n");
	printf(" \n");
	printf(" xPos: %d\n", camera->xPos);
	printf(" yPos: %d\n", camera->yPos);
	printf(" zPos: %d\n", camera->zPos);
	printf(" \n");
	printf(" xRot: %d\n", camera->xRot);
	printf(" yRot: %d\n", camera->yRot);
	printf(" zRot: %d\n", camera->zRot);
	printf(" \n");
	printf(" Player: \n");
	printf(" \n");
	printf(" xPos: %d\n", player->xPos);
	printf(" yPos: %d\n", player->yPos);
	printf(" zPos: %d\n", player->zPos);
	printf(" \n");
	printf(" xRot: %d\n", player->xRot);
	printf(" yRot: %d\n", player->yRot);
	printf(" zRot: %d\n", player->zRot);
}

// hardware setup functions
void setup3D()
{
	videoSetMode(MODE_0_3D);
	vramSetBankA(VRAM_A_TEXTURE);
	
	glInit();
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	
	glClearColor(0,0,0,31);
	glClearPolyID(63);
	glClearDepth(0x7FFF);
	
	glViewport(0,0,255,191);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.01, 100);
	
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(2.0f,2.0f,2.0f);
	
	glMaterialf(GL_AMBIENT, RGB15(16,16,16));
	glMaterialf(GL_DIFFUSE, RGB15(16,16,16));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
	glMaterialf(GL_EMISSION, RGB15(16,16,16));
	
	glMaterialShinyness();
}

void setupNitroFS()
{
	if (nitroFSInit(NULL)) printf(" NitroFS works\n");
}

void mallocStructs()
{
	camera = (struct CAMERA*)malloc(sizeof(struct CAMERA));
	player = (struct PLAYER*)malloc(sizeof(struct PLAYER));
}

// update functions 
void cameraPart()
{
	if (cameraMode == 0)
	{
		debugCamera(camera);
	}
	else if (cameraMode == 1)
	{
		playerCamera(camera, player->xPos, player->yPos, player->zPos, player->yRot);
	}
}

void playerPart()
{
	updatePlayer(player, camera);
}

void updateGame()
{
	playerPart();
	cameraPart();
}

void updateRender()
{
	updateCamera(camera);
	debugPrint();
	
	renderPlayer(player);
	
	renderModel(levelModel);
}

int main()
{
	consoleDemoInit();
	setup3D();
	setupNitroFS();
	mallocStructs();
	
	startPositionPlayer(player);
	
	// For test purposes
	FILE * model = fopen("/data/stage/intro.bkdm", "rb");
	levelModel = loadModel(model);
	fclose(model);
	
	while(1)
	{
		updateGame();
		
		swiWaitForVBlank();
		
		updateRender();
		
		glFlush(0);
	}
	
	return 0;
}