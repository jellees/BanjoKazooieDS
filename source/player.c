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
	scanKeys();
	int hold = keysHeld();

	player->yPos = -500;
	
	int vec[3];
	vec[0] = (2000 * sinLerp(camera->yRot)) / 4096;
	vec[1] = 0;
	vec[2] = (2000 * cosLerp(camera->yRot)) / 4096;

	if(!(vec[0] == 0 && vec[1] == 0 && vec[2] == 0))
		normalizef32(&vec[0]);

	//printf("%d, %d\n", xCam, zCam);
	printf("%d, %d\n", vec[0], vec[2]);

	if (hold & KEY_LEFT)
	{
		player->yRot += degreesToAngle(1);
		player->xPos += vec[0] * 15/ 4096;
		player->zPos += vec[2] * 15/ 4096;
	}
	if (hold & KEY_RIGHT)
	{
		player->yRot -= degreesToAngle(1);
		player->xPos += vec[0] * 15/ 4096;
		player->zPos += vec[2] * 15/ 4096;
	}
	if (hold & KEY_UP)
	{
			player->xPos += vec[0] * 20/ 4096;
			player->zPos += vec[2] * 20/ 4096;
			//not 100% correct, because when you press up and left together, you will not rotate!
			if(abs(player->yRot - camera->yRot) < 150) player->yRot = camera->yRot;
			else
			{
				if(camera->yRot > player->yRot) player->yRot += 150;
				if(camera->yRot < player->yRot) player->yRot -= 150;
			}
			//player->yRot = camera->yRot;
	}
	if (hold & KEY_DOWN)
	{
		player->xPos -= vec[0] * 20/ 4096;
		player->zPos -= vec[2] * 20/ 4096;
		if(abs(player->yRot - camera->yRot) < 150) player->yRot = camera->yRot;
	else
	{
		if(camera->yRot > player->yRot) player->yRot += 150;
		if(camera->yRot < player->yRot) player->yRot -= 150;
	}
		/*if(abs(player->yRot - (degreesToAngle(180) - camera->yRot)) < 300) player->yRot = (degreesToAngle(180) - camera->yRot);
	else
	{
		if(camera->yRot > (degreesToAngle(180) - camera->yRot)) player->yRot += 300;
		if(camera->yRot < (degreesToAngle(180) - camera->yRot)) player->yRot -= 300;
	}*/
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