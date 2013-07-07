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
		int i;
		//faz com que o personagem nao saia da cena
		forceWithinScene(chr,scene);

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

