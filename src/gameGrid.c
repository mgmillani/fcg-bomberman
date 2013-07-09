#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "gameGrid.h"
#include "color.h"
#include "play.h"

#include "debug.h"

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
	grid->cellSize = 1.0;
	grid->spawnPoints = 0;
	grid->grid = malloc(max*sizeof(*grid->grid));
	for(i=0 ; i<max ; i++)
		grid->grid[i] = Empty;

	return grid;
}

/**
  * carrega o gamegrid de um arquivo
  * se o grid passado for NULL, aloca um novo
  */
t_gameGrid *gameGridLoad(t_gameGrid *grid,const char *fname,t_colorMap *colorMap)
{
	//carrega a imagem do cenario
	SDL_Surface *img = IMG_Load(fname);
	//inicia o grid com as dimensoes adequadas
	gameGridInit(grid,img->w,img->h);
	unsigned char *pixels = (unsigned char *)img->pixels;
	int c = img->format->BitsPerPixel/8;
	int w = img->w;
	int h = img->h;

	//ERR("pitch: %u\n",(unsigned int)img->pitch);
	unsigned int waste = img->pitch - img->w*c;
	//ERR("waste: %u\n",(unsigned int)waste);

	int x,y,z;
	int pos=0;
	int cell=0;
	unsigned char color[4];
	unsigned char *order[4];
	unsigned int mask = 0xff;
	unsigned char *Rmask = (unsigned char *)(&img->format->Rmask);
	unsigned char *Gmask = (unsigned char *)(&img->format->Gmask);
	unsigned char *Bmask = (unsigned char *)(&img->format->Bmask);
	//determina a ordem das cores
	for(z=0 ; z<c ; z++)
	{
		if(*Rmask != 0)
			order[z] = color+0;
		else if(*Gmask != 0)
			order[z] = color+1;
		else if(*Bmask != 0)
			order[z] = color+2;

		//ERR("%d : %d\n",z,order[z]-color);

		Rmask++;
		Gmask++;
		Bmask++;
	}
	order[3] = color+3;

	for(y=0 ; y<h ; y++)
	{
		for(x=0 ; x<w ; x++,cell++)
		{
			for(z=0 ; z<c ; z++)
			{
				*order[z] = pixels[pos];
				pos++;
			}

			//determina se a cor encontrada corresponde a algum objeto
			if(colorCompare(color,colorMap->breakableColor,3)==0)
				grid->grid[cell] = BreakableWall;
			else if(colorCompare(color,colorMap->unbreakableColor,3)==0)
				grid->grid[cell] = UnbreakableWall;
			else if(colorCompare(color,colorMap->spawnColor,3)==0)
			{
				grid->grid[cell] = Spawn;
				grid->spawnPoints++;
			}
			else if(colorCompare(color,colorMap->enemySpawnColor,3)==0)
			{
				grid->grid[cell] = EnemySpawn;
				grid->enemySpawnPoints++;
			}
			else
				grid->grid[cell] = Empty;
		}
		//pula os bytes de alinhamento
		pos+=waste;
	}

	SDL_FreeSurface(img);

	return grid;
}

/**
  * transforma todas as ocorrencias de Fire por Empty
  */
void cleanFire(t_gameGrid *grid)
{
	unsigned int x,y;
	unsigned int pos;
	for(pos=0,y=0 ; y<grid->h ; y++)
		for(x=0 ; x<grid->w ; x++,pos++)
			if(grid->grid[pos] == Fire)
				grid->grid[pos] = Empty;
}

/**
  * cria as cores para cada elemento que aparece no minimapa
  */
void createMinimapColors(t_gameData *data)
{
	t_minimap *colors = &data->minimap;
	colors->player[0] = 1.0;
	colors->player[1] = 1.0;
	colors->player[2] = 1.0;

	colors->enemy[0] = 1.0;
	colors->enemy[1] = 0;
	colors->enemy[2] = 0;

	colors->breakable[0] = 0;
	colors->breakable[1] = 1.0;
	colors->breakable[2] = 0;

	colors->unbreakable[0] = 0.5;
	colors->unbreakable[1] = 0.1;
	colors->unbreakable[2] = 0.5;

	colors->fire[0] = 0;
	colors->fire[1] = 1.0;
	colors->fire[2] = 1.0;

	colors->floor[0] = 0;
	colors->floor[1] = 0;
	colors->floor[2] = 0.2;
}

/**
  * escreve os dados da grid na tela
  */
void gameGridPrint(t_gameGrid *grid)
{

	unsigned int x,y,pos;
	for(pos=0,y=0 ; y<grid->h; y++)
	{
		for(x=0 ; x<grid->w ; x++,pos++)
		{
			printf("(%02d)",grid->grid[pos]);
		}
		printf("\n");
	}

}








