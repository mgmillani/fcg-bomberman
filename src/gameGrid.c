#include "gameGrid.h"

/**
  * inicia um grid vazio com largura e altura dada
  * se o grid passado for NULL, aloca um novo, caso contrario, usa o passado
  */
t_gameGrid *gameGridInit(t_gameGrid *grid,unsigned int w, unsigned int h)
{
	unsigned int max = w*h;
	unsigned int i;
	if(grid == NULL)
		grid = malloc(sizeof(*grid));
	grid->w = w;
	grid->h = h;
	grid->grid = malloc(max*sizeof(*grid->grid));
	for(i=0 ; i<max ; i++)
		grid->grid[i] = Empty;

	return grid;
}

/**
  * carrega o gamegrid de um arquivo
  * se o grid passado for NULL, aloca um novo
  */
t_gameGrid *gameGridLoad(t_gameGrid *,const char *fname);
