#ifndef BOMB_H
#define BOMB_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <definitions.h>

#define EXPLOSION_PARTICLES 50

typedef struct s_bomb
{
	unsigned int pos[2]; //posicao da bomba no grid
	Uint32 t0;     //instante em que a bomba foi colocada
	Uint32 delay;  //tempo para a explosao
	unsigned int power;
	double radius;
	t_character *owner;
	GLUquadric *body;
	GLUquadric *fuse;
	GLuint bodyTexture;
	GLuint fuseTexture;
}t_bomb;

typedef struct s_explosion
{
	unsigned int pos[2];
	int dir[2]; //direcao da expansao
	unsigned int power;
	unsigned int expanded; //quanto que a explosao ja expandiu
	Uint32 t0;     //instante em que a explosao comecou
	Uint32 delay;  //tempo que permanece visivel
	t_particle fireParticle[EXPLOSION_PARTICLES]; //posicao de cada particula
	t_particle smokeParticle[EXPLOSION_PARTICLES];
	GLuint smoke;
	GLuint fire;
}t_explosion;

/**
  * desenha uma bomba
  */
void drawBomb(t_bomb *bomb);

/**
  * cria uma bomba
  * se NULL for passado, aloca uma nova bomba
  */
t_bomb *bombCreate(t_bomb *, t_character *owner,GLuint body, GLuint fuse, unsigned int power, unsigned int x, unsigned int y);

/**
  * simula o efeito das explosoes
  */
void simulateExplosion(t_gameData *data);

/**
  * desenha as explosoes
  */
void drawExplosions(t_gameData *data);

/**
  * cria uma explosao no ponto especificado
  */
t_explosion *explosionCreate(t_explosion *explosion, GLuint smoke, GLuint fire, unsigned int power,unsigned int x, unsigned int y, int dx, int dy,double cellSize);

/**
  * explode as bombas cujo pavio acabou
  */
void checkBombExplosion(t_gameData *data);

/**
  * gera uma particula numa regiao aleatorio no espaco [0,cellSize]^3
  */
void generateRandomParticle(t_particle *particle,double cellSize,int direction[2]);


#endif // BOMB_H

