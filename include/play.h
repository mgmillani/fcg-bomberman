#ifndef PLAY_H
#define PLAY_H

#include "gameGrid.h"

typedef struct s_gameData
{
	t_gameGrid *grid;
	t_gridTextures *textures;
}t_gameData;

/**
  * laco principal do jogo
  */
void play(t_gameData *game);


#endif // PLAY_H
