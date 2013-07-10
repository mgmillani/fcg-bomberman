#ifndef POWERUP_H
#define POWERUP_H

#include "definitions.h"

typedef struct s_powerup
{
	t_rect3 frameBox[6];
	t_rect3 imageBox[6];
	GLuint image;
	double angle;
	GLuint border;
}t_powerup;

/**
  * cria um novo powerup com uma caixa de tamanho size
  * se o ponteiro passado for NULL, aloca um novo powerup
  */
t_powerup *powerupInit(t_powerup *pwup,double size,GLuint border,GLuint image);

/**
  * desenha um powerup
  */
void drawPowerup(t_powerup *);

#endif // POWERUP_H

