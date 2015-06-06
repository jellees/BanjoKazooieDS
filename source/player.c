/*	player code	*/

#include "playerStruct.h"
#include "cameraStruct.h"

#include <nds.h>
#include <filesystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void updatePlayer(struct PLAYER * player, struct CAMERA * camera)
{
	/*scanKeys();
	
	int hold = keysHeld();
	
	player->yPos = -500;

	if (hold & KEY_LEFT)
	{
		player->xPos += sinLerp(cameraRotation +  8191) / 100;
		player->zPos += cosLerp(cameraRotation +  8191) / 100;
	}
	if (hold & KEY_RIGHT)
	{
		player->xPos -= sinLerp(cameraRotation +  8191) / 100;
		player->zPos -= cosLerp(cameraRotation +  8191) / 100;
		player->yRot += 100;
	}
	if (hold & KEY_UP)
	{
		player->xPos += sinLerp(cameraRotation) / 100;
		player->zPos += cosLerp(cameraRotation) / 100;
	}
	if (hold & KEY_DOWN)
	{
		player->xPos -= sinLerp(cameraRotation) / 100;
		player->zPos -= cosLerp(cameraRotation) / 100;
	}
	
	if (hold & KEY_LEFT)
	{
		player->xPos += (player->zPos - 0) / 50;
		player->zPos += -(player->xPos - 0) / 50;
	}
	if (hold & KEY_RIGHT)
	{
		player->xPos -= (player->zPos - 0) / 50;
		player->zPos -= -(player->xPos - 0) / 50;
	}
	if (hold & KEY_UP)
	{
		player->xPos += (player->xPos - 0) / 50;
		player->zPos += (player->zPos - 0) / 50;
	}
	if (hold & KEY_DOWN)
	{
		player->xPos -= (player->xPos - 0) / 50;
		player->zPos -= (player->zPos - 0) / 50;
	}*/
	
	scanKeys();
	int hold = keysHeld();

	player->yPos = -500;

	int xCam = camera->xPos;
	int zCam = camera->zPos;
	
	int vec[3];
	vec[0] = (player->xPos - xCam);
	vec[1] = 0;
	vec[2] = (player->zPos - zCam);

	if(!(vec[0] == 0 && vec[1] == 0 && vec[2] == 0))
		normalizef32(&vec[0]);

		printf("%d, %d\n", xCam, zCam);
	printf("%d, %d\n", vec[0], vec[2]);

	if (hold & KEY_LEFT)
	{
		player->xPos += vec[2] * 25 / 4096;
		player->zPos += -vec[0] * 25 / 4096;
		player->yRot = 8191;
	}
	if (hold & KEY_RIGHT)
	{
		player->xPos -= vec[2] * 25 / 4096;
		player->zPos -= -vec[0] * 25 / 4096;
		player->yRot = -8191;
	}
	if (hold & KEY_UP)
	{
		player->xPos += vec[0] * 25 / 4096;
		player->zPos += vec[2] * 25 / 4096;
	}
	if (hold & KEY_DOWN)
	{
		player->xPos -= vec[0] * 25 / 4096;
		player->zPos -= vec[2] * 25 / 4096;
	}
}

void startPositionPlayer(struct PLAYER * player)
{
	player->xPos = 500;
	player->yPos = 0;
	player->zPos = 500;
}

void renderPlayer(struct PLAYER * player)
{
	glPushMatrix();
	//glScalef(0.03f,0.03f,0.03f);	
	
	glTranslatef32(-player->xPos, -player->yPos, -player->zPos);
	glRotateYi(player->yRot);
		
	
	// for Test purposes a box
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.1f,  0.1f);
		glTexCoord2f(0.1f, 0.0f); glVertex3f( 0.1f, -0.1f,  0.1f);
		glTexCoord2f(0.1f, 0.1f); glVertex3f( 0.1f,  0.1f,  0.1f);
		glTexCoord2f(0.0f, 0.1f); glVertex3f(-0.1f,  0.1f,  0.1f);
		// Back Face
		glTexCoord2f(0.1f, 0.0f); glVertex3f(-0.1f, -0.1f, -0.1f);
		glTexCoord2f(0.1f, 0.1f); glVertex3f(-0.1f,  0.1f, -0.1f);
		glTexCoord2f(0.0f, 0.1f); glVertex3f( 0.1f,  0.1f, -0.1f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.1f, -0.1f);
		// Top Face
		glTexCoord2f(0.0f, 0.1f); glVertex3f(-0.1f,  0.1f, -0.1f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f,  0.1f,  0.1f);
		glTexCoord2f(0.1f, 0.0f); glVertex3f( 0.1f,  0.1f,  0.1f);
		glTexCoord2f(0.1f, 0.1f); glVertex3f( 0.1f,  0.1f, -0.1f);
		// Bottom Face
		glTexCoord2f(0.1f, 0.1f); glVertex3f(-0.1f, -0.1f, -0.1f);
		glTexCoord2f(0.0f, 0.1f); glVertex3f( 0.1f, -0.1f, -0.1f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.1f,  0.1f);
		glTexCoord2f(0.1f, 0.0f); glVertex3f(-0.1f, -0.1f,  0.1f);
		// Right face
		glTexCoord2f(0.1f, 0.0f); glVertex3f( 0.1f, -0.1f, -0.1f);
		glTexCoord2f(0.1f, 0.1f); glVertex3f( 0.1f,  0.1f, -0.1f);
		glTexCoord2f(0.0f, 0.1f); glVertex3f( 0.1f,  0.1f,  0.1f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.1f,  0.1f);
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.1f, -0.1f);
		glTexCoord2f(0.1f, 0.0f); glVertex3f(-0.1f, -0.1f,  0.1f);
		glTexCoord2f(0.1f, 0.1f); glVertex3f(-0.1f,  0.1f,  0.1f);
		glTexCoord2f(0.0f, 0.1f); glVertex3f(-0.1f,  0.1f, -0.1f);
	glEnd();
	glPopMatrix(1);
}