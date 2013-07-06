#include <stdlib.h>
#include <math.h>

#include <SDL/SDL_opengl.h>

#include "init.h"
#include "definitions.h"
#include "draw.h"
#include "build.h"
#include "random.h"

#include "debug.h"

#define FWIDTH 20.0
#define FHEIGHT 20.0

/**
  * inicializa o personagem
  * se o ponteiro passado for NULL, aloca um novo personagem
  */
t_character *initCharacter(t_character *chr,GLuint texture,t_gameGrid *grid)
{
	if(chr == NULL)
		chr = malloc(sizeof(*chr));

	unsigned int i;
	for(i=0 ; i<3 ; i++)
	{
		chr->vel[i] = 0;
		chr->acc[i] = 0;
	}
	//escolhe um spawn point aleatorio para colocar o personagem
	int point = randrange(grid->spawnPoints-1);
	unsigned int j,pos;
	for(i=pos=0 ; i< grid->h && point>=0 ; i++)
	{
		for(j=0 ; j<grid->w && point>=0 ; j++,pos++)
		{
			if(grid->grid[pos] == Spawn)
				point--;
			// coloca o jogador naquela posicao
			if(point < 0)
			{
				grid->grid[pos] = Empty;
				chr->pos[0] = j*grid->cellSize;
				chr->pos[1] = 1.0;
				chr->pos[2] = i*grid->cellSize;
				grid->spawnPoints--;

				//ERR("Char pos: %.2lf %.2lf %.2lf\n",chr->pos[0],chr->pos[1],chr->pos[2]);
			}
		}
	}

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
  */
t_scene *initScene(t_scene *scene, t_gameGrid *grid)
{

	if(scene == NULL)
		scene = malloc(sizeof(*scene));

	scene->minPos[0] = - grid->cellSize/2;
	scene->minPos[1] = 0;
	scene->minPos[2] = - grid->cellSize/2;

	scene->maxPos[0] = grid->w*grid->cellSize - grid->cellSize/2;
	scene->maxPos[1] = FHEIGHT/3;
	scene->maxPos[2] = grid->h*grid->cellSize - grid->cellSize/2;

	int i;
	double margin = 0.01;
	for(i=0 ; i<3 ; i++)
	{
		scene->maxPos[i] -= margin;
		scene->minPos[i] += margin;
	}

	return scene;

}
