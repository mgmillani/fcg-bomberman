#ifndef PLAY_H
#define PLAY_H

#include <SDL/SDL_opengl.h>

struct s_gameData;

#include "powerup.h"
#include "gameGrid.h"
#include "list.h"

typedef struct s_gameData
{
	struct s_gameGrid *grid;
	struct s_gridTextures *textures;
	//struct s_minimap minimap;
	t_minimap minimap;
	t_list bombs;
	t_list explosions;
	GLuint bombTexture;
	GLuint fuseTexture;
	GLuint fireTexture;
	GLuint smokeTexture;

	GLuint enemyTexture;
	e_character *enemies;
	unsigned int numEnemies;

	t_powerup bombPowerup;
	t_powerup powerPowerup;
	t_powerup speedPowerup;

}t_gameData;

/**
  * laco principal do jogo
  */
void play(t_gameData *game);


#endif // PLAY_H
