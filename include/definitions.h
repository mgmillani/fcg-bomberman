#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <SDL/SDL_opengl.h>

#define PI 3.14159265358979311599796346854419

#define WALKSPEED 0.01
#define JUMPSTR 0.1
#define ZERO 0.00001
#define AIRFRICTION 0.985
#define FLOORFRICTION 0.85

typedef enum {Floor, LeftWall, RightWall, FrontWall, BackWall,Ceiling,NumWalls} e_wall;

typedef struct s_rect
{
	double x,y,z;
	double w,h,d;
}t_rect;

typedef struct s_character
{
	double pos[3];
	double vel[3];
	double acc[3];
	double dir[3];  //cartesian coordinates - direction of movement
	double look[3]; //spherical coordinates (r,theta,phi) - where the character is looking

	char firstPerson;

	double height;
	double walkSpeed;
	double rotation;

	double wheelRot;    //rotacao da roda, em graus
	double wheelNorm[3];//vetor normal ao sentido da rotacao da roda
	double wheelRadius;
	double neckHeight;
	GLUquadric *wheel;
	GLUquadric *body;
	GLUquadric *arm;
	GLUquadric *neck;
	GLUquadric *shoulder;

	GLuint texture;

}t_character;

typedef struct s_rect3
{
	double pos[3];//coordenada de um dos cantos
	double v[3];  //vetores que indicam onde estarao os outros pontos
	double w[3];

}t_rect3;

typedef struct s_scene
{
	double minPos[3];	//limites da cena
	double maxPos[3];
}t_scene;

typedef struct s_block
{
	t_rect3 left;
	t_rect3 right;
	t_rect3 up;
	t_rect3 down;

	GLuint sideTexture;

	double texScaleX;
	double texScaleY;
}t_block;

#endif // DEFINITIONS_H

