#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "definitions.h"

/**
  * desenha um robo da classe A
  */
void drawRobotA(t_character *chr);

/*
typedef struct s_player
{
	GLfloat color[3];
	double x,y;
	t_controller control;
	t_action action; //a acao que o jogador quer executar no proximo frame
}t_player;
*/

#endif // PLAYER_H

