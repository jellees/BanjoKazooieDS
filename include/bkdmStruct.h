/* STRUCT of the BKDM */
#ifndef BKDM_H
#define BKDM_H


struct HEADER
{
	int BKDM;
	unsigned int fileSize;
	char version;
	char flags;
	unsigned short sectionCount;
	unsigned short materialCount;
	unsigned short vertexCount;
	unsigned short coordinateCount;
	unsigned short normalCount;
	unsigned short colorCount;
	unsigned short objectCount;
	int padding1;
	int padding2;
};

//--------MATERIALS----------
struct MATERIAL
{
	char materialID;
	char X;
	char Y;
	char TX;
	char TY;
	char alpha;
	char culling;
	char stringSize;
	char * textureName;
	
	//for memory
	short scrollX;
	short scrollY;
};

struct MATE
{
	int mateSIGN;
	struct MATERIAL * material;
	short padding;
};

//--------VERTEX----------
struct VERTEX
{
	v16 x;
	v16 y;
	v16 z;
};

struct VERT
{
	int vertSIGN;
	struct VERTEX * vertex;
};

//--------COORDINATES----------
struct COORDINATE
{
	t16 u;
	t16 v;
};

struct TECO
{
	int tecoSIGN;
	struct COORDINATE * coordinate;
};

//--------NORMALS----------
struct NORMAL
{
	v16 x;
	v16 y;
	v16 z;
};

struct NOCO
{
	int nocoSIGN;
	struct NORMAL * normal;
};

//--------COLORS----------
struct COLOR
{
	char r, g, b;
};

struct VECO
{
	int vecoSIGN;
	struct COLOR * color;
};

//--------FACES----------
struct TRIANGLE
{
	unsigned short v, t, n, c;
};

struct TRIANGLES
{
	struct TRIANGLE triangle[3];
};

struct OBJECT
{
	char materialID;
	short triangleCount;
	struct TRIANGLES * triangles;
};

struct FACE
{
	int faceSIGN;
	struct OBJECT * object;
};

struct MODEL
{
	struct HEADER header;
	struct MATE materials;
	struct VERT vertices;
	struct TECO coordinates;
	struct NOCO normals;
	struct VECO colors;
	struct FACE faces;
	
	//for memory
	int * textures;
};

/* END OF STRUCT */
#endif