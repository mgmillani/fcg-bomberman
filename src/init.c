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
			}
		}
	}

	chr->dir[0] = 1;
	chr->dir[1] = 0;
	chr->dir[2] = 0;
	chr->look[0] = 1;
	chr->look[1] = PI/2;
	chr->look[2] = 0;

	//cria a iluminacao
	chr->lighting = GL_LIGHT0;
	GLfloat light_ambient[] = { 0,0,0,0 };
	glGetFloatv(GL_COLOR_CLEAR_VALUE,light_ambient);
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(chr->lighting, GL_AMBIENT, light_ambient);
	glLightfv(chr->lighting, GL_DIFFUSE, light_diffuse);
	glLightfv(chr->lighting, GL_SPECULAR, light_specular);

	chr->firstPerson = 0;
	chr->action = None;
	chr->dead = 0;
	chr->bombs = 1;
	chr->maxBombs = 1;

	chr->height = 0.65;
	chr->walkSpeed = WALKSPEED;
	chr->power = 2;
	chr->rotation = PI/30;

	chr->wheelRot = 0;
	chr->wheelNorm[0] = 1;
	chr->wheelNorm[1] = 0;
	chr->wheelNorm[2] = 0;
	chr->wheelRadius = 0.1;
	chr->neckHeight = 0.01;
	chr->wheel = gluNewQuadric();
	chr->body = gluNewQuadric();
	chr->arm = gluNewQuadric();
	chr->shoulder = gluNewQuadric();
	chr->neck = gluNewQuadric();

	chr->texture = texture;

	return chr;

}

/**
  * inicializa os inimigos
  * se o ponteiro passado for NULL, aloca um novo personagem
  */
e_character *initEnemies(e_character *chra,GLuint texture)
{
	if(chra == NULL)
		chra = malloc(sizeof(*chra));

	unsigned int i;
	for(i=0 ; i<3 ; i++)
	{
		chra->vel[i] = 0;
		chra->acc[i] = 0;
	}
	//escolhe um spawn point aleatorio para colocar o personagem
	/*int point = randrange(grid->enemySpawnPoints-1);
	unsigned int j,pos;
	for(i=pos=0 ; i< grid->h && point>=0 ; i++)
	{
		for(j=0 ; j<grid->w && point>=0 ; j++,pos++)
		{
			if(grid->grid[pos] == EnemySpawn)
				point--;
			// coloca o jogador naquela posicao
			if(point < 0)
			{
				grid->grid[pos] = Empty;
				chra->pos[0] = j*grid->cellSize;
				chra->pos[1] = 1.0;
				chra->pos[2] = i*grid->cellSize;
				grid->spawnPoints--;
			}
		}
	}*/

	chra->dir[0] = 1;
	chra->dir[1] = 0;
	chra->dir[2] = 0;
	chra->look[0] = 1;
	chra->look[1] = PI/2;
	chra->look[2] = 0;

	chra->height = 0.65;
	chra->walkSpeed = WALKSPEED;
	chra->rotation = PI/30;
	chra->numMovement = 0;
	chra->dead = 0;

	chra->wheelRot = 0;
	chra->wheelNorm[0] = 1;
	chra->wheelNorm[1] = 0;
	chra->wheelNorm[2] = 0;
	chra->wheelRadius = 0.1;
	chra->neckHeight = 0.01;
	chra->wheel = gluNewQuadric();
	chra->body = gluNewQuadric();
	chra->arm = gluNewQuadric();
	chra->shoulder = gluNewQuadric();
	chra->neck = gluNewQuadric();

	chra->texture = texture;

	return chra;
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
	double margin = 0.01;

	scene->minPos[0] = - grid->cellSize/2;
	scene->minPos[1] = - margin;
	scene->minPos[2] = - grid->cellSize/2;

	scene->maxPos[0] = grid->w*grid->cellSize - grid->cellSize/2;
	scene->maxPos[1] = FHEIGHT/3 + margin;
	scene->maxPos[2] = grid->h*grid->cellSize - grid->cellSize/2;

	int i;

	for(i=0 ; i<3 ; i++)
	{
		scene->maxPos[i] -= margin;
		scene->minPos[i] += margin;
	}

	return scene;

}
