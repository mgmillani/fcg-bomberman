#include <stdlib.h>
#include <math.h>

#include <SDL/SDL_opengl.h>

#include "init.h"
#include "definitions.h"
#include "draw.h"
#include "build.h"
#include "random.h"

#define FWIDTH 20.0
#define FHEIGHT 20.0

/**
  * inicializa o personagem
  * se o ponteiro passado for NULL, aloca um novo personagem
  */
t_character *initCharacter(t_character *chr,GLuint texture)
{
	if(chr == NULL)
		chr = malloc(sizeof(*chr));

	int i;
	for(i=0 ; i<3 ; i++)
	{
		chr->vel[i] = 0;
		chr->acc[i] = 0;
	}
	chr->pos[0] = 1.0;
	chr->pos[1] = 0.5;
	chr->pos[2] = 1.5;
	chr->dir[0] = 1;
	chr->dir[1] = 0;
	chr->dir[2] = 0;
	chr->look[0] = 1;
	chr->look[1] = PI/2;
	chr->look[2] = 0;

	chr->jumping = 0;
	chr->crouching = 0;

	chr->headBob = (randrange(100)/100.0)*PI;
	chr->height = 0.5;
	chr->walkSpeed = WALKSPEED;
	chr->jumpStr = JUMPSTR;
	chr->rotation = PI/30;

	chr->texture = texture;

	return chr;

}

/**
  * inicializa a camera
  * se o ponteiro passado for NULL, aloca uma camera nova
  */
t_camera *initCamera(t_camera *camera, t_character *chr)
{
	if(camera==NULL)
		camera = malloc(sizeof(*camera));

	int i;
	for(i=0 ; i<3 ; i++)
	{
		camera->pos[i] = chr->pos[i];
		camera->rot[i] = chr->dir[i];
	}

	return camera;
}

/**
  * inicializa a cena
  * se o ponteiro passado for NULL, aloca uma cena nova
  * ground, wall e ceiling correspondem as texturas que serao usadas
  */
t_scene *initScene(t_scene *scene, GLuint ground, GLuint wall, GLuint ceiling)
{

	if(scene == NULL)
		scene = malloc(sizeof(*scene));

	scene->textures[Floor] = ground;
	scene->textures[LeftWall] = wall;
	scene->textures[RightWall] = wall;
	scene->textures[FrontWall] = wall;
	scene->textures[BackWall] = wall;
	scene->textures[Ceiling] = ceiling;

	scene->minPos[0] = -FWIDTH/2;
	scene->minPos[1] = 0;
	scene->minPos[2] = -FWIDTH/2;

	scene->maxPos[0] = FWIDTH/2;
	scene->maxPos[1] = FHEIGHT/3;
	scene->maxPos[2] = FWIDTH/2;

	int i;
	double margin = 0.95;
	for(i=0 ; i<3 ; i++)
	{
		scene->maxPos[i] *=margin;
		scene->minPos[i] *=margin;
	}

	buildFloor(&(scene->walls[Floor]),FWIDTH,FHEIGHT);
	buildLeftWall(&(scene->walls[LeftWall]),FWIDTH,FHEIGHT/3);
	buildRightWall(&(scene->walls[RightWall]),FWIDTH,FHEIGHT/3);
	buildFrontWall(&(scene->walls[FrontWall]),FWIDTH,FHEIGHT/3);
	buildBackWall(&(scene->walls[BackWall]),FWIDTH,FHEIGHT/3);
	buildCeiling(&(scene->walls[Ceiling]),FWIDTH,FHEIGHT);

	return scene;

}












