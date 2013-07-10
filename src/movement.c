#include <math.h>

#include <SDL/SDL.h>

#include "definitions.h"
#include "movement.h"
#include "bomb.h"
#include "random.h"

#include "debug.h"

/**
  * move um bando de caras por ai
  */
void moveDudes(t_character *dudes, int numDudes)
{
	int i;
	//double freq = 0.17;
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
  * trata um evento do tipo SDL_KEYDOWN
  */
void treatKeyDownCharacters(t_character *chr,SDL_Event *event)
{
	if(event->key.keysym.sym == SDLK_v)
	{
		chr->firstPerson ^=1;
	}
	else if(event->key.keysym.sym == SDLK_SPACE)
	{
		chr->action = PlaceBomb;
	}
}

/**
  * altera o estado do jogo dependendo do que o jogador tentou fazer
  */
void treatCharacterAction(t_character *chr,t_gameData *data)
{
	t_gameGrid *grid = data->grid;
	t_list *bombs = &data->bombs;
	switch(chr->action)
	{
		case PlaceBomb:
		{
			//determina a posicao do personagem
			int x,y;
			x = (chr->pos[0] + grid->cellSize/2)/grid->cellSize;
			y = (chr->pos[2] + grid->cellSize/2)/grid->cellSize;
			//determina para onde ele esta olhando
			int dx = 0,dy = 0;
			if(chr->dir[0] > 0.5)
				dx = 1;
			else if(chr->dir[0] < -0.5)
				dx = -1;
			if(chr->dir[2] > 0.5)
				dy = 1;
			else if(chr->dir[2] < -0.5)
				dy = -1;

			//personagem nao esta olhando diretamente para nenhuma celula
			if(dx + dy == 0)
				break;

			//verifica se a posicao a frente do personagem esta livre
			x += dx;
			y += dy;
			//verifica se esta dentro dos limites
			if(x >= grid->w || x<0)
				break;
			if(y >= grid->h || y<0)
				break;
			if(grid->grid[x + y*grid->w] == Empty)
			{
				//verifica se o personagem pode colocar bombas
				if(chr->bombs > 0)
					chr->bombs--;
				else
					break;
				//cria a bomba
				grid->grid[x + y*grid->w] = Bomb;
				t_bomb *bomb = bombCreate(NULL,chr,data->bombTexture,data->fuseTexture,chr->power,x,y);
				listAppend(bombs,bomb,NULL);
			}
		}
			break;
		default:
			break;
	}
	chr->action = None;
}

/**
  * determina o movimento do personagem com base nas teclas pressionadas
  */
void treatKeyStateCharacter(t_character *character, Uint8 *keystate,int numKeys)
{
	unsigned int i;
	double speed = character->walkSpeed;
	double moveDir[3] = {};

	//move o personagem para frente
	if(keystate[SDLK_w]==1)
	{
		for(i=0 ; i<3 ; i++)
			moveDir[i] += character->dir[i];
	}
	//tras
	if(keystate[SDLK_s]==1)
	{
		for(i=0 ; i<3 ; i++)
			moveDir[i] -= character->dir[i];
	}
	//esquerda
	if(keystate[SDLK_a]==1)
	{
		moveDir[0] += character->dir[2];
		moveDir[2] -= character->dir[0];
	}
	//direita
	if(keystate[SDLK_d]==1)
	{
		moveDir[0] -= character->dir[2];
		moveDir[2] += character->dir[0];
	}

	//normaliza o vetor da direcao do movimento
	double norm = 0;
	for(i=0 ; i<3 ; i++)
		norm += moveDir[i]*moveDir[i];
	if(norm>0)
	{
		norm = sqrt(norm);
		for(i=0 ; i<3 ; i++)
			character->acc[i] += speed*moveDir[i]/norm;
	}

}






















