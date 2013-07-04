#include <math.h>

#include <SDL/SDL.h>

#include "definitions.h"
#include "movement.h"
#include "random.h"

#include "debug.h"

/**
  * move um bando de caras por ai
  */
void moveDudes(t_character *dudes, int numDudes)
{
	int i;
	double freq = 0.17;
	for(i=0 ; i<numDudes ; i++)
	{
		//rotaciona aleatoriamente
		double angle;
		switch(randrange(5))
		{
			case 0:
				angle = (randrange(100)/100.0 + 0.05)*PI/6;
				break;
			case 2:
				angle = -(randrange(100)/100.0 + 0.05)*PI/6;
				break;
			default:
				angle = 0;
		}

		dudes[i].look[2]+=angle;
		updateCharacterWalkDir(dudes+i);

		int j;
		//move para frente
		for(j=0 ; j<3 ; j++)
			dudes[i].acc[j] += dudes[i].dir[j]*dudes[i].walkSpeed;

		//faz o headBob
		dudes[i].headBob += freq;
		if(dudes[i].headBob > PI)
			dudes[i].headBob = 0;

		//o personagem pode pular
		//if(randrange(50)==0)


	}
}

/**
  * baseando-se na direcao para a qual o personagem esta olhando, atualiza para onde esta andando
  */
void updateCharacterWalkDir(t_character *chr)
{
	//tranca o angulo de visao
	if(chr->look[1]>5*PI/6)
		chr->look[1] = 5*PI/6;
	else if(chr->look[1]<PI/12)
		chr->look[1] = PI/12;

	chr->dir[0] = sin(chr->look[2]);
	chr->dir[2] = cos(chr->look[2]);
}

/**
  * atualiza para onde o personagem esta olhando baseando-se no movimento do mouse
  */
void treatMouseMovement(t_character *chr, int deltax, int deltay)
{
	double factor = 10;

	double dx = deltax/factor;
	double dy = deltay/factor;

	chr->look[2]-=dx*chr->rotation;
	chr->look[1]+=dy*chr->rotation;
}

/**
  * determina o movimento do personagem com base nas teclas pressionadas
  */
void treatKeyStateCharacter(t_character *character, Uint8 *keystate,int numKeys)
{
	unsigned int i;
	double speed = character->walkSpeed;
	double moveDir[3] = {};

	if(character->jumping == 0)
	{
		char bob = 0;
		double freq = 0.13;
		if(character->crouching)
			freq = 0.1;

		//move o personagem para frente
		if(keystate[SDLK_UP]==1)
		{
			for(i=0 ; i<3 ; i++)
				moveDir[i] += character->dir[i];
		}
		//tras
		if(keystate[SDLK_DOWN]==1)
		{
			for(i=0 ; i<3 ; i++)
				moveDir[i] -= character->dir[i];
		}
		//esquerda
		if(keystate[SDLK_LEFT]==1)
		{
			moveDir[0] += character->dir[2];
			moveDir[2] -= character->dir[0];
		}
		//direita
		if(keystate[SDLK_RIGHT]==1)
		{
			moveDir[0] -= character->dir[2];
			moveDir[2] += character->dir[0];
		}
		//lanças bombas
		/*if(keystate[SDLK_SPACE]==1)
		{
			character->acc[0] += (1+character->jumpStr*0.5)*character->vel[0];
			character->acc[1] += character->jumpStr;
			character->acc[2] += (1+character->jumpStr*0.5)*character->vel[2];
			character->jumping = 1;
		}*/

		//normaliza o vetor da direcao do movimento
		double norm = 0;
		int i;
		for(i=0 ; i<3 ; i++)
			norm += moveDir[i]*moveDir[i];
		if(norm>0)
		{
			bob =1;
			norm = sqrt(norm);
			for(i=0 ; i<3 ; i++)
				character->acc[i] += speed*moveDir[i]/norm;
		}

		//se deve ser feito o head bob
		if(bob)
		{
			character->headBob += freq;
			if(character->headBob > PI)
				character->headBob = 0;
		}
		else
		{
			character->headBob -= freq;
			if(character->headBob < 0)
				character->headBob = 0;
		}

	}
}





















