#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "play.h"
#include "bomb.h"
#include "physics.h"
#include "draw.h"
#include "random.h"

#include "definitions.h"

#include "debug.h"

/**
  * desenha uma bomba
  */
void drawBomb(t_bomb *bomb)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,bomb->bodyTexture);
	gluQuadricTexture(bomb->fuse,GLU_TRUE);
	gluQuadricTexture(bomb->body,GLU_TRUE);
	//desenha o corpo da bomba
	glPushMatrix();
	glTranslatef(0,bomb->radius,0);
	gluSphere(bomb->body,bomb->radius,18,18);


	//desenha o pavio
	glBindTexture(GL_TEXTURE_2D,bomb->fuseTexture);
	Uint32 t1 = SDL_GetTicks();
	double fuseHeight = bomb->radius*(1 - (double)(t1 - bomb->t0)/bomb->delay);
	double fuseBase = bomb->radius*0.2;
	glTranslatef(0,bomb->radius,0);
	glRotatef(270,1,0,0);
	gluCylinder(bomb->fuse,fuseBase,fuseBase,fuseHeight,12,3);
	glPopMatrix();

}

/**
  * cria uma bomba
  * se NULL for passado, aloca uma nova bomba
  */
t_bomb *bombCreate(t_bomb *bomb, GLuint body, GLuint fuse, unsigned int power, unsigned int x, unsigned int y)
{
	if(bomb == NULL)
		bomb = malloc(sizeof(*bomb));

	bomb->pos[0] = x;
	bomb->pos[1] = y;
	bomb->power = power;
	bomb->bodyTexture = body;
	bomb->fuseTexture = fuse;

	bomb->body = gluNewQuadric();
	bomb->fuse = gluNewQuadric();
	bomb->radius = 0.1;
	bomb->delay = 3000;
	bomb->t0 = SDL_GetTicks();

	return bomb;
}

void bombDestroy(t_bomb *bomb)
{
	gluDeleteQuadric(bomb->body);
	gluDeleteQuadric(bomb->fuse);
}

/**
  * explode as bombas cujo pavio acabou
  */
void checkBombExplosion(t_gameData *data)
{
	t_listNode *node,*aux;
	Uint32 t1 = SDL_GetTicks();

	for(node=data->bombs.first ; node!=NULL ; node=aux)
	{
		aux = node->next;
		//verifica se a bomba explodiu
		t_bomb *bomb = node->key;
		if(t1 - bomb->t0 > bomb->delay)
		{
			//cria uma explosao para cada direcao
			t_explosion *exp = explosionCreate(NULL,data->smokeTexture,data->fireTexture,bomb->power,bomb->pos[0],bomb->pos[1],1,0,data->grid->cellSize);
			listAppend(&data->explosions,exp,NULL);
			exp = explosionCreate(NULL,data->smokeTexture,data->fireTexture,bomb->power,bomb->pos[0],bomb->pos[1],0,1,data->grid->cellSize);
			listAppend(&data->explosions,exp,NULL);
			exp = explosionCreate(NULL,data->smokeTexture,data->fireTexture,bomb->power,bomb->pos[0],bomb->pos[1],-1,0,data->grid->cellSize);
			listAppend(&data->explosions,exp,NULL);
			exp = explosionCreate(NULL,data->smokeTexture,data->fireTexture,bomb->power,bomb->pos[0],bomb->pos[1],0,-1,data->grid->cellSize);
			listAppend(&data->explosions,exp,NULL);

			//destroi a bomba
			bombDestroy(bomb);
			free(bomb);
			listRemoveNode(&(data->bombs),node);
		}
	}

}

/**
  * cria uma explosao no ponto especificado
  */
t_explosion *explosionCreate(t_explosion *explosion, GLuint smoke, GLuint fire, unsigned int power,unsigned int x, unsigned int y, int dx, int dy,double cellSize)
{
	if(explosion == NULL)
		explosion = malloc(sizeof(*explosion));

	explosion->pos[0] = x;
	explosion->pos[1] = y;
	explosion->dir[0] = dx;
	explosion->dir[1] = dy;
	explosion->power = power;
	explosion->expanded = 0;
	explosion->smoke = smoke;
	explosion->fire = fire;

	explosion->t0 = SDL_GetTicks();
	explosion->delay = 1000;

	//cria as particulas para fogo e fumaca
	unsigned int i;
	for(i=0 ; i<EXPLOSION_PARTICLES ; i++)
	{
		generateRandomParticle(explosion->fireParticle+i,0.2*cellSize,explosion->dir);
		generateRandomParticle(explosion->smokeParticle+i,0.2*cellSize,explosion->dir);
	}

	return explosion;
}

/**
  * simula o efeito das explosoes
  */
void simulateExplosion(t_gameData *data)
{
	t_listNode *node,*aux;
	Uint32 t1 = SDL_GetTicks();
	for(node=data->explosions.first ; node!=NULL ; node=aux)
	{
		aux = node->next;
		//move as particulas
		t_explosion *exp = node->key;
		//verifica se alguma parede foi destruida
		int range = 1+exp->power*((double)(t1 - exp->t0)/exp->delay);
		//destroi a explosao
		if(range > exp->power || exp->power==0)
		{
			listRemoveNode(&data->explosions,node);
			continue;
		}
		int x = exp->pos[0];
		int y = exp->pos[1];
		if(x < 0)
			x=0;
		else if(x >= data->grid->w)
			x = data->grid->w-1;

		if(y < 0)
			y=0;
		else if(y >= data->grid->h)
			y = data->grid->h-1;

		unsigned int i;
		int pos;
		//reconstroi o rastro da explosao
		for(i=0 ; i<=range ; i++)
		{
			pos = x + y*data->grid->w;
			//substitui o que for vazio
			if(data->grid->grid[pos] == Empty)
				data->grid->grid[pos] = Fire;
			x+= exp->dir[0];
			y+= exp->dir[1];

			if(x < 0)
				x=0;
			else if(x >= data->grid->w)
				x = data->grid->w-1;

			if(y < 0)
				y=0;
			else if(y >= data->grid->h)
				y = data->grid->h-1;
		}

		x -= exp->dir[0];
		y -= exp->dir[1];
		//se a explosao expandiu
		if(range > exp->expanded)
		{
			exp->expanded = range;
			//se for um muro indestrutivel, para a expansao
			pos = x + y*data->grid->w;
			if(data->grid->grid[pos] == UnbreakableWall)
				exp->power = range-1;
			//se for um muro destrutivel, ha uma chance de aparece um powerup
			else if(data->grid->grid[pos] == BreakableWall)
			{
				int r = randrange(NUM_POWERUPS-1);
				char powerup;
				switch(r)
				{
					case 0:
						powerup = PowerupBomb;
						break;
					case 1:
						powerup = PowerupPower;
						break;
					case 2:
						powerup = PowerupSpeed;
						break;
				}
				data->grid->grid[pos] = powerup;
			}
			//se for qualquer outra coisa, destroi
			else
				data->grid->grid[pos] = Fire;
		}

		for(i=0 ; i<EXPLOSION_PARTICLES ; i++)
		{
			moveParticle(exp->fireParticle+i);
			moveParticle(exp->smokeParticle+i);
			exp->smokeParticle[i].acc[1]+=0.0001;
		}
	}

}

/**
  * desenha as explosoes
  */
void drawExplosions(t_gameData *data)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D,data->fireTexture);
	glPushMatrix();
	double cellSize = data->grid->cellSize;
	t_listNode *node;
	for(node=data->explosions.first ; node!=NULL ; node=node->next)
	{
		//desenha cada particula
		t_explosion *exp = node->key;
		double dx = exp->pos[0]*cellSize;
		double dy = exp->pos[1]*cellSize;
		glTranslated(dx,0,dy);
		unsigned int i;
		for(i=0 ; i<EXPLOSION_PARTICLES ; i++)
			drawRectangle(&exp->fireParticle[i].pos,1.0,1.0);
		glTranslated(-dx,0,-dy);
	}

	glBindTexture(GL_TEXTURE_2D,data->smokeTexture);

	for(node=data->explosions.first ; node!=NULL ; node=node->next)
	{
		//desenha cada particula
		t_explosion *exp = node->key;
		double dx = exp->pos[0]*cellSize;
		double dy = exp->pos[1]*cellSize;
		glTranslated(dx,0,dy);
		unsigned int i;
		for(i=0 ; i<EXPLOSION_PARTICLES ; i++)
			drawRectangle(&exp->smokeParticle[i].pos,1.0,1.0);
		glTranslated(-dx,0,-dy);
	}
	glPopMatrix();
	glEnable(GL_CULL_FACE);
}

/**
  * gera uma particula numa regiao aleatorio no espaco [0,cellSize]^3
  */
void generateRandomParticle(t_particle *particle,double cellSize,int direction[2])
{
	particle->pos.v[0] = 0;
	particle->pos.v[1] = 0.1;
	particle->pos.v[2] = 0;

	double r = (double)rand()/RAND_MAX;
	particle->pos.w[0] = r*0.1;
	particle->pos.w[1] = 0;
	particle->pos.w[2] = 0.1*sqrt(1 - r*r);

	unsigned int i;
	for(i=0 ; i<3 ; i++)
	{
		particle->pos.pos[i] = cellSize*(double)rand()/RAND_MAX;
		particle->acc[i] = 0;
	}
	particle->vel[0] = direction[0]*(0.01*((double)rand()/RAND_MAX)+0.01);
	particle->vel[1] = 0;
	particle->vel[2] = direction[1]*(0.01*((double)rand()/RAND_MAX)+0.01);

}










