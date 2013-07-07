#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "bomb.h"

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













