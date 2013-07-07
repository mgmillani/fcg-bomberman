#ifndef PLAY_H
#define PLAY_H

#include <SDL/SDL_opengl.h>

#include "gameGrid.h"
#include "list.h"

typedef struct s_gameData
{
	t_gameGrid *grid;
	t_gridTextures *textures;
	t_list bombs;
	GLuint bombTexture;
	GLuint fuseTexture;
}t_gameData;

/**
  * laco principal do jogo
  */
void play(t_gameData *game);


#endif // PLAY_H
