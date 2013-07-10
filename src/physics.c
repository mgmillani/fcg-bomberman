#include <math.h>

#include "physics.h"

#include "debug.h"

#define NORMALHEIGHT 0.15
#define CROUCHHEIGHT 0.10
#define FLOOR 0.2

/**
  * simula a fisica dos personagens
  */
void simulatePhysics(t_gameData *data, t_scene *scene,double gravity[3])
{
	t_gameGrid *grid = data->grid;
	int j;
	//move os inimigos
	for(j=0 ; j<data->numEnemies ; j++)
	{
		e_character *chr = data->enemies+j;
		int i;

		double friction = FLOORFRICTION;

		//atualiza os vetores
		//aplica a gravidade
		int x0 = chr->pos[0]/grid->cellSize + 0.5;
		int y0 = chr->pos[2]/grid->cellSize + 0.5;
		for(i=0 ; i<3 ; i++)
		{
			chr->vel[i] = chr->acc[i] + chr->vel[i]*friction;
			chr->acc[i] = gravity[i];
			chr->pos[i] += chr->vel[i];
		}
		int x1 = chr->pos[0]/grid->cellSize + 0.5;
		int y1 = chr->pos[2]/grid->cellSize + 0.5;
		//se a celula mudou, decresce o numero de movimentos
		if(x1 != x0)
			chr->numMovement--;
		else if(y1 != y0)
			chr->numMovement--;

		//trata as colisoes
		enemyCollision(chr,data);

		double sum = sqrt(chr->vel[0]*chr->vel[0] + chr->vel[2]*chr->vel[2]);
		if(sum > ZERO)
		{
			chr->wheelNorm[0] = chr->vel[2]/(sum);
			chr->wheelNorm[2] = -chr->vel[0]/(sum);
			chr->wheelRot += (sum/chr->wheelRadius)*(180/PI);
			if(chr->wheelRot > 360)
				chr->wheelRot -= 360;
			else if(chr->wheelRot < 0)
				chr->wheelRot += 360;
		}
	}
	//move o jogador
	t_character *chr = data->player;
	int i;
	//faz com que o personagem nao saia da cena
	//forceWithinScene(chr,scene);
	playerCollision(chr,data);

	double friction = FLOORFRICTION;

	//atualiza os vetores
	//aplica a gravidade
	for(i=0 ; i<3 ; i++)
	{
		chr->vel[i] = chr->acc[i] + chr->vel[i]*friction;
		chr->acc[i] = gravity[i];
		chr->pos[i] += chr->vel[i];
	}
	double sum = sqrt(chr->vel[0]*chr->vel[0] + chr->vel[2]*chr->vel[2]);
	if(sum > ZERO)
	{
		chr->wheelNorm[0] = chr->vel[2]/(sum);
		chr->wheelNorm[2] = -chr->vel[0]/(sum);
		chr->wheelRot += (sum/chr->wheelRadius)*(180/PI);
		if(chr->wheelRot > 360)
			chr->wheelRot -= 360;
		else if(chr->wheelRot < 0)
			chr->wheelRot += 360;
	}
	/*int cx = (chr->pos[0] + grid->cellSize/2) / grid->cellSize;
	int cy = (chr->pos[2] + grid->cellSize/2) / grid->cellSize;*/
	/*ERR("P: %.2lf\t%.2lf\n",chr->pos[0],chr->pos[2]);
	ERR("P: %d\t%d\n",cx,cy);*/
}

void playerCollision(t_character *chr,t_gameData *data)
{
	t_gameGrid *grid = data->grid;
	int ex = (chr->pos[0] + grid->cellSize/2) / grid->cellSize;
	int ey = (chr->pos[2] + grid->cellSize/2) / grid->cellSize;

	double min = -grid->cellSize/2;
	double maxx = grid->w*grid->cellSize - grid->cellSize/2;
	double maxy = grid->h*grid->cellSize - grid->cellSize/2;


	if(chr->pos[1] < 0)
		chr->pos[1] = 0;
	//se esta dentro das bordas
	char collided = 0;
	if(chr->pos[0]<min)
	{
		//chr->pos[0] = -grid->cellSize;
		collided = 1;
	}
	else if(chr->pos[0] >= maxx)
	{
		//chr->pos[0] -= grid->cellSize;
		collided = 1;
	}

	if(chr->pos[2]<min)
	{
		//chr->pos[2] = -grid->cellSize;
		collided = 1;
	}
	else if(chr->pos[2] > maxy)
	{
		//chr->pos[2] -= grid->cellSize;
		collided = 1;
	}
	//se bateu num muro
	int pos = ex + ey*grid->w;
	switch(grid->grid[pos])
	{
		case BreakableWall:
		case UnbreakableWall:
		case Bomb:
			collided = 1;
			break;
		case Fire:
			chr->dead = 1;
		default:
			break;
	}

	if(collided)
	{
		int i;
		for(i=0 ; i<3 ; i++)
		{
			chr->vel[i] = -chr->vel[i];
			chr->pos[i] += chr->vel[i];
			//chr->acc[i] = chr->vel[i];
		}
	}
}

void enemyCollision(e_character *chr,t_gameData *data)
{
	t_gameGrid *grid = data->grid;
	int ex = (chr->pos[0] + grid->cellSize/2) / grid->cellSize;
	int ey = (chr->pos[2] + grid->cellSize/2) / grid->cellSize;

	double min = -grid->cellSize/2;
	double maxx = grid->w*grid->cellSize - grid->cellSize/2;
	double maxy = grid->h*grid->cellSize - grid->cellSize/2;


	if(chr->pos[1] < 0)
		chr->pos[1] = 0;
	//se esta dentro das bordas
	char collided = 0;
	if(chr->pos[0]<min)
	{
		//chr->pos[0] = -grid->cellSize;
		collided = 1;
	}
	else if(chr->pos[0] >= maxx)
	{
		//chr->pos[0] -= grid->cellSize;
		collided = 1;
	}

	if(chr->pos[2]<0)
	{
		//chr->pos[2] = -grid->cellSize;
		collided = 1;
	}
	else if(chr->pos[2] > maxy)
	{
		//chr->pos[2] -= grid->cellSize;
		collided = 1;
	}
	//se bateu num muro
	int pos = ex + ey*grid->w;
	switch(grid->grid[pos])
	{
		case BreakableWall:
		case UnbreakableWall:
		case Bomb:
			collided = 1;
			break;
		case Fire:
			chr->dead = 1;
		default:
			break;
	}

	if(collided)
	{
		chr->numMovement=0;
		int i;
		for(i=0 ; i<3 ; i++)
		{
			chr->vel[i] = -chr->vel[i];
			chr->pos[i] += 2*chr->vel[i];
			//chr->acc[i] = chr->vel[i];
		}
	}

	/*ex = (chr->pos[0] + grid->cellSize/2) / grid->cellSize;
	ey = chr->pos[2] / grid->cellSize;
	ERR("E: %d\t%d\n",ex,ey);
	ERR("E: %.2lf\t%.2lf\n",chr->pos[0],chr->pos[2]);
	ERR("G: %d\t%d\n",grid->w,grid->h);
	ERR("G: %.2lf\t%.2lf\n",grid->w*grid->cellSize,grid->h*grid->cellSize);*/
}

/**
  * faz com que o personagem nao escape da cena
  */
void forceWithinScene(t_character *chr, t_scene *scene)
{
	int i;
	for(i=0 ; i<3 ; i++)
	{
		if(chr->pos[i] < scene->minPos[i])
		{
			chr->vel[i] =0;
			chr->pos[i] += 0.7*((scene->minPos[i]) - chr->pos[i]);
		}
		else if(chr->pos[i] > scene->maxPos[i])
		{
			chr->vel[i] =0;
			chr->pos[i] -= 0.7*(chr->pos[i] - (scene->maxPos[i]));
		}
	}
}

/**
  * atualiza os vetores da particula
  */
void moveParticle(t_particle *part)
{
	unsigned int i;
	for(i=0 ; i<3 ; i++)
	{
		part->pos.pos[i] += part->vel[i];
		part->vel[i] += part->acc[i];
		part->acc[i] = 0;
	}

}
