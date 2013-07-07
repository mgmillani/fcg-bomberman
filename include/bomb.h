#ifndef BOMB_H
#define BOMB_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

typedef struct s_bomb
{
	unsigned int pos[2]; //posicao da bomba no grid
	Uint32 t0;     //instante em que a bomba foi colocada
	Uint32 delay;  //tempo para a explosao
	unsigned int power;
	double radius;
	GLUquadric *body;
	GLUquadric *fuse;
	GLuint bodyTexture;
	GLuint fuseTexture;
}t_bomb;

/**
  * desenha uma bomba
  */
void drawBomb(t_bomb *bomb);

/**
  * cria uma bomba
  * se NULL for passado, aloca uma nova bomba
  */
t_bomb *bombCreate(t_bomb *, GLuint body, GLuint fuse, unsigned int power, unsigned int x, unsigned int y);


#endif // BOMB_H

