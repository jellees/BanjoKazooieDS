#include <nds.h>
#include <malloc.h>
#include <stdio.h>
#include <filesystem.h>
#include <nds/arm9/video.h>

#include <string.h>
#include "bkdm.h"

struct MODEL *loadModel(FILE * file)
{
	int i;

	struct MODEL * model = malloc(sizeof(struct MODEL));
	if(model == NULL) iprintf("creating space in memory failed...\n\n");

	//Read header
	fread(&model->header.BKDM, 32, 1, file);
	if(model->header.BKDM == 0x4D444B42)
	{
		//Read materials
		fread(&model->materials.mateSIGN, 4, 1, file);
		if(model->materials.mateSIGN == 0x4554414D)
		{
			int materialCounter = model->header.materialCount;
			model->materials.material = malloc(materialCounter * sizeof(struct MATERIAL));
			int i;
			for(i = 0; i < materialCounter; i++)
			{
				fread(&model->materials.material[i].materialID, 1, 1, file);
				fread(&model->materials.material[i].X, 1, 1, file);
				fread(&model->materials.material[i].Y, 1, 1, file);
				fread(&model->materials.material[i].TX, 1, 1, file);
				fread(&model->materials.material[i].TY, 1, 1, file);
				fread(&model->materials.material[i].alpha, 1, 1, file);
				fread(&model->materials.material[i].culling, 1, 1, file);
				fread(&model->materials.material[i].stringSize, 1, 1, file);
				
				model->materials.material[i].textureName = malloc(model->materials.material[i].stringSize);
				char * materialName = malloc(model->materials.material[i].stringSize);;
				fread(materialName, 1, model->materials.material[i].stringSize, file);
				model->materials.material[i].textureName = materialName;
			}
			fread(&model->materials.padding, 2, 1, file);
		} else iprintf(" no material section\n");
		
		//Read vertex section
		fread(&model->vertices.vertSIGN, 4, 1, file);
		if(model->vertices.vertSIGN == 0x54524556)
		{
			model->vertices.vertex = malloc(model->header.vertexCount * sizeof(struct VERTEX));
			int i = 0;
			while(i < model->header.vertexCount)
			{
				float xFloat;
				float yFloat;
				float zFloat;
				fread(&xFloat, 4, 1, file);
				fread(&yFloat, 4, 1, file);
				fread(&zFloat, 4, 1, file);
				model->vertices.vertex[i].x = floattov16(xFloat);
				model->vertices.vertex[i].y = floattov16(yFloat);
				model->vertices.vertex[i].z = floattov16(zFloat);
				i++;
			}
		} else iprintf(" no vertex section\n\n");
		
		//read the coordinate section
		fread(&model->coordinates.tecoSIGN, 4, 1, file);
		if(model->coordinates.tecoSIGN == 0x4F434554)
		{
			model->coordinates.coordinate = malloc(model->header.coordinateCount * sizeof(struct COORDINATE));
			int i = 0;
			while(i < model->header.coordinateCount)
			{
				float u, v;
				fread(&u, 4, 1, file);
				fread(&v, 4, 1, file);
				model->coordinates.coordinate[i].u = floattot16(u);
				model->coordinates.coordinate[i].v = floattot16(v);
				i++;
			}
		} else iprintf(" no coordinate section\n\n");
		
		//read the normal section
		fread(&model->normals.nocoSIGN, 4, 1, file);
		if(model->normals.nocoSIGN == 0x4F434F4E)
		{
			model->normals.normal = malloc(model->header.normalCount * sizeof(struct NORMAL));
			int i = 0;
			while(i < model->header.normalCount)
			{
				float x, y, z;
				fread(&x, 4, 1, file);
				fread(&y, 4, 1, file);
				fread(&z, 4, 1, file);
				model->normals.normal[i].x = floattov16(x);
				model->normals.normal[i].y = floattov16(y);
				model->normals.normal[i].z = floattov16(z);
				i++;
			}
		} else iprintf(" no normal section\n\n");
		
		//read the color section
		fread(&model->colors.vecoSIGN, 4, 1, file);
		if(model->colors.vecoSIGN == 0x4F434556)
		{
			model->colors.color = malloc(model->header.colorCount * sizeof(struct COLOR));
			int i = 0;
			while(i < model->header.colorCount)
			{
				char r, g, b;
				fread(&r, 1, 1, file);
				fread(&g, 1, 1, file);
				fread(&b, 1, 1, file);
				model->colors.color[i].r = r;
				model->colors.color[i].g = g;
				model->colors.color[i].b = b;
				i++;
			}
		} else iprintf(" no color section\n\n");
		
		//read the face section
		fread(&model->faces.faceSIGN, 4, 1, file);
		if(model->faces.faceSIGN == 0x45434146)
		{
			model->faces.object = malloc(model->header.objectCount * sizeof(struct OBJECT));
			int i = 0;
			while(i < model->header.objectCount)
			{
				fread(&model->faces.object[i].materialID, 1, 1, file);
				fread(&model->faces.object[i].triangleCount, 2, 1, file);
				model->faces.object[i].triangles = malloc(model->faces.object[i].triangleCount * sizeof(struct TRIANGLES));
				int a = 0;
				while(a < model->faces.object[i].triangleCount)
				{
					if(model->header.flags == 1)
					{
						int b = 0;
						while(b < 3)
						{
							fread(&model->faces.object[i].triangles[a].triangle[b].v, 1, 1, file);
							fread(&model->faces.object[i].triangles[a].triangle[b].t, 1, 1, file);
							fread(&model->faces.object[i].triangles[a].triangle[b].n, 1, 1, file);
							fread(&model->faces.object[i].triangles[a].triangle[b].c, 1, 1, file);
							b++;
						}
					}
					else
					{
						int b = 0;
						while(b < 3)
						{
							fread(&model->faces.object[i].triangles[a].triangle[b].v, 2, 1, file);
							fread(&model->faces.object[i].triangles[a].triangle[b].t, 2, 1, file);
							fread(&model->faces.object[i].triangles[a].triangle[b].n, 2, 1, file);
							fread(&model->faces.object[i].triangles[a].triangle[b].c, 2, 1, file);
							b++;
						}
					}
					a++;
				}
				i++;
			}
		} else iprintf(" no face section\n\n");
	}
	else iprintf(" Not correct file\n");
	
	//load textures
	FILE * tex;
	model->textures = malloc(sizeof(int) * model->header.materialCount);
	glGenTextures(model->header.materialCount, &model->textures[0]);
	for(i = 0; i < model->header.materialCount; i++)
	{
		char * two = model->materials.material[i].textureName;
		char * thr = ".bin";
		char str[80];
		strcpy(str, "/data/texture/");
		strcat(str, two);
		strcat(str, thr);
		
		tex = fopen(str, "rb");
		if(tex != NULL)
		{
			int kenmerk = 0;
			int texType = 0;
			int sizeX = 0;
			int sizeY = 0;
			
			fread(&kenmerk, 4, 1, tex);
			fread(&sizeX, 4, 1, tex);
			fread(&sizeY, 4, 1, tex);
			
			if(kenmerk == 0x78424752) texType = 7;
			else texType = 8;
			
			int size = (sizeX * sizeY) * 2;
			char *data = malloc(size);
			fread(data, sizeof(char), size, tex);
			
			glBindTexture(0, model->textures[i]);
			int f = glTexImage2D(0, 0, texType, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T, data);
			if(f == 0) printf(" texture load fail \n");
			
			free(data);
		}
		else
		{
			printf(" texture NOT loaded \n");
			model->textures[i] = 0;
		}
		fclose(tex);
	}
	
	printf(" \n");
	
	return model;
}

int renderModel(struct MODEL * model)
{
	int v_one, v_two, v_thr;
	int c_one, c_two, c_thr;
	//int n_one, n_two, n_thr;
	int t_one, t_two, t_thr;
	v16 v_x, v_y, v_z;
	t16 u_m, v_m;
	char r, g, b;
	//v16 n_x, n_y, n_z;
	
	int objectC = 0;
	int triangleC = 0;
	
	while(objectC < model->header.objectCount)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		
		unsigned char mat = model->faces.object[objectC].materialID;
		unsigned char X = model->materials.material[mat].X;
		unsigned char Y = model->materials.material[mat].Y;
		unsigned char TX = model->materials.material[mat].TX;
		unsigned char TY = model->materials.material[mat].TY;
		unsigned char A = model->materials.material[mat].alpha;
		unsigned char C = model->materials.material[mat].culling;
		
		glPolyFmt(POLY_ALPHA(A) | C);
		
		if(TX != 0 || TY != 0)
		{
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glTranslatef(model->materials.material[mat].scrollX, model->materials.material[mat].scrollY, 0);
			model->materials.material[mat].scrollX += TX;
			model->materials.material[mat].scrollY += TY;
		}
		
		glBindTexture(GL_TEXTURE_2D, model->textures[mat]);
		
		while(triangleC < model->faces.object[objectC].triangleCount)
		{
			v_one = model->faces.object[objectC].triangles[triangleC].triangle[0].v;
			v_two = model->faces.object[objectC].triangles[triangleC].triangle[1].v;
			v_thr = model->faces.object[objectC].triangles[triangleC].triangle[2].v;
			c_one = model->faces.object[objectC].triangles[triangleC].triangle[0].c;
			c_two = model->faces.object[objectC].triangles[triangleC].triangle[1].c;
			c_thr = model->faces.object[objectC].triangles[triangleC].triangle[2].c;
			/*n_one = model->faces.object[objectC].triangles[triangleC].triangle[0].n;
			n_two = model->faces.object[objectC].triangles[triangleC].triangle[1].n;
			n_thr = model->faces.object[objectC].triangles[triangleC].triangle[2].n;*/
			t_one = model->faces.object[objectC].triangles[triangleC].triangle[0].t;
			t_two = model->faces.object[objectC].triangles[triangleC].triangle[1].t;
			t_thr = model->faces.object[objectC].triangles[triangleC].triangle[2].t;
			
			glBegin(GL_TRIANGLES);
			glNormal(NORMAL_PACK( 0, 0, 1<<10));
						
			v_x = model->vertices.vertex[v_one].x;
			v_y = model->vertices.vertex[v_one].y;
			v_z = model->vertices.vertex[v_one].z;
			r = model->colors.color[c_one].r;
			g = model->colors.color[c_one].g;
			b = model->colors.color[c_one].b;
			/*n_x = model->normals.normal[n_one].x;
			n_y = model->normals.normal[n_one].y;
			n_z = model->normals.normal[n_one].z;
			glNormal3f(n_x, n_y, n_z);*/
			u_m = model->coordinates.coordinate[t_one].u;
			v_m = model->coordinates.coordinate[t_one].v;
			glColor3b(r,g,b);
			glTexCoord2t16(u_m * X,v_m * Y);
			glVertex3v16(v_x, v_y, v_z);
			
			v_x = model->vertices.vertex[v_two].x;
			v_y = model->vertices.vertex[v_two].y;
			v_z = model->vertices.vertex[v_two].z;
			r = model->colors.color[c_two].r;
			g = model->colors.color[c_two].g;
			b = model->colors.color[c_two].b;
			/*n_x = model->normals.normal[n_two].x;
			n_y = model->normals.normal[n_two].y;
			n_z = model->normals.normal[n_two].z;
			glNormal3f(n_x, n_y, n_z);*/
			u_m = model->coordinates.coordinate[t_two].u;
			v_m = model->coordinates.coordinate[t_two].v;
			glColor3b(r,g,b);
			glTexCoord2t16(u_m * X,v_m * Y);
			glVertex3v16(v_x, v_y, v_z);
			
			v_x = model->vertices.vertex[v_thr].x;
			v_y = model->vertices.vertex[v_thr].y;
			v_z = model->vertices.vertex[v_thr].z;
			r = model->colors.color[c_thr].r;
			g = model->colors.color[c_thr].g;
			b = model->colors.color[c_thr].b;
			/*n_x = model->normals.normal[n_thr].x;
			n_y = model->normals.normal[n_thr].y;
			n_z = model->normals.normal[n_thr].z;
			glNormal3f(n_x, n_y, n_z);*/
			u_m = model->coordinates.coordinate[t_thr].u;
			v_m = model->coordinates.coordinate[t_thr].v;
			glColor3b(r,g,b);
			glTexCoord2t16(u_m * X,v_m * Y);
			glVertex3v16(v_x, v_y, v_z);
			
			glEnd();
			
			triangleC++;
		}
		
		glPopMatrix(1);
		
		triangleC = 0;
		objectC++;
	}
	
	return 1;
}