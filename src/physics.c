#include <math.h>

#include "physics.h"

#include "debug.h"

#define NORMALHEIGHT 0.15
#define CROUCHHEIGHT 0.10
#define FLOOR 0.2

/**
  * simula a fisica dos personagens
  */
void simulatePhysics(t_character *dudes,int numDudes, t_scene *scene,double gravity[3])
{
	int j;
	for(j=0 ; j<numDudes ; j++)
	{
		t_character *chr = dudes+j;
		//ERR("j:%d\n",j);
		//ERR("Pos: %lf %lf %lf\n",chr->pos[0],chr->pos[1],chr->pos[2]);
		int i;
		//faz com que o personagem nao saia da cena
		forceWithinScene(chr,scene);

		//verifica se o personagem esta no ar
		if(fabs(chr->pos[1] - scene->minPos[1]) <= fabs(gravity[1]))
			chr->jumping = 0;

		double friction;
		if(chr->jumping == 1)
			friction = AIRFRICTION;
		else
			friction = FLOORFRICTION;

		//atualiza os vetores
		//aplica a gravidade
		for(i=0 ; i<3 ; i++)
		{
			chr->vel[i] = chr->acc[i] + chr->vel[i]*friction;
			chr->acc[i] = gravity[i];
			chr->pos[i] += chr->vel[i];
		}
	}
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

