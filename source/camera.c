#include "cameraStruct.h"

#include <nds.h>
#include <filesystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

void debugCamera(struct CAMERA * camera)
{
	scanKeys();
	
	int held = keysHeld();

	if (held & KEY_B)
	{
		camera->yRot += 100;
	}
	else if (held & KEY_A)
	{
		camera->yRot -= 100;
	}
	
	if (held & KEY_X)
	{
		camera->xRot += 100;
	}
	else if (held & KEY_Y)
	{
		camera->xRot -= 100;
	}
	
	if (held & KEY_L)
	{
		camera->yPos += 40;
	}
	else if (held & KEY_R)
	{
		camera->yPos -= 40;
	}
	
	if (held & KEY_LEFT)
	{
		camera->xPos += sinLerp(camera->yRot +  8191) / 100;
		camera->zPos += cosLerp(camera->yRot +  8191) / 100;
	}
	if (held & KEY_RIGHT)
	{
		camera->xPos -= sinLerp(camera->yRot +  8191) / 100;
		camera->zPos -= cosLerp(camera->yRot +  8191) / 100;
	}
	if (held & KEY_UP)
	{
		camera->xPos += sinLerp(camera->yRot) / 100;
		camera->zPos += cosLerp(camera->yRot) / 100;
	}
	if (held & KEY_DOWN)
	{
		camera->xPos -= sinLerp(camera->yRot) / 100;
		camera->zPos -= cosLerp(camera->yRot) / 100;
	}
}

void playerCamera(struct CAMERA * camera, int x, int y, int z, int rotation)
{
	if(rotation > camera->yRot) camera->yRot += 50;
	if(rotation < camera->yRot) camera->yRot -= 50;
	
	camera->xPos = 500 + sinLerp(camera->yRot) + -x;
	camera->zPos = 500 + cosLerp(camera->yRot) + -z;
	
	camera->xFoc = -x;
	camera->yFoc = -y;
	camera->zFoc = -z;
}

void updateCamera(struct CAMERA * camera)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*
	// Rotation
	glRotatef32i(camera->xRot, (1<<12), 0, 0);
	glRotatef32i(LUT_SIZE - camera->yRot, 0, (1<<12), 0);
	
	// X Y Z position
	glTranslatef32(camera->xPos, camera->yPos, camera->zPos);*/
	
	gluLookAtf32(camera->xPos, 800, camera->zPos, camera->xFoc, camera->yFoc, camera->zFoc, 0, 65535, 0);
}