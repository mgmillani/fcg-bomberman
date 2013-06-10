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

	char jumping;
	char crouching;

	double headBob;
	double height;
	double jumpStr;
	double walkSpeed;
	double rotation;

	GLuint texture;

}t_character;

typedef struct s_camera
{
	double pos[3];
	double rot[3]; //spherical coordinates (r,theta,phi)
}t_camera;

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
	t_rect3 walls[NumWalls];	//paredes da cena
	GLuint textures[NumWalls];	//texturas das paredes
}t_scene;

#endif // DEFINITIONS_H

