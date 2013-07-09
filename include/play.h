#ifndef PLAY_H
#define PLAY_H

#include <SDL/SDL_opengl.h>

#include "powerup.h"
#include "gameGrid.h"
#include "list.h"

typedef struct s_gameData
{
	t_gameGrid *grid;
	t_gridTextures *textures;
	t_list bombs;
	t_list explosions;
	GLuint bombTexture;
	GLuint fuseTexture;
	GLuint fireTexture;
	GLuint smokeTexture;

	t_powerup bombPowerup;
	t_powerup powerPowerup;
	t_powerup speedPowerup;

}t_gameData;

/**
  * laco principal do jogo
  */
void play(t_gameData *game);


#endif // PLAY_H
