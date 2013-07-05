typedef enum e_gridElements;
typedef struct s_gameGrid t_gameGrid;
typedef struct s_gridTextures t_gridTextures;

#ifndef GAMEGRID_H
#define GAMEGRID_H

#include <SDL/SDL_opengl.h>

typedef enum {Empty,Bomb,Fire,BreakableWall,UnbreakableWall,NumElements} e_gridElements;

typedef struct s_gameGrid
{
	e_gridElements *grid;  //indica que tipo de coisa que existe em cada ponto do grid
	unsigned int w,h; //altura e largura do grid
}t_gameGrid;

typedef struct s_colorMap
{
	unsigned char emptyColor[3];
	unsigned char breakableColor[3];
	unsigned char unbreakableColor[3];
}t_colorMap;

typedef struct s_gridTextures
{
	GLuint weakWall;
	GLuint strongWall;
	GLuint floor;
	GLuint ceiling;
}t_gridTextures;

/**
  * inicia um grid vazio com largura e altura dada
  * se o grid passado for NULL, aloca um novo, caso contrario, usa o passado
  */
t_gameGrid *gameGridInit(t_gameGrid *,unsigned int w, unsigned int h);

/**
  * carrega o gamegrid de um arquivo
  * se o grid passado for NULL, aloca um novo
  */
t_gameGrid *gameGridLoad(t_gameGrid *,const char *fname,t_colorMap *);

/**
  * escreve os dados da grid na tela
  */
void gameGridPrint(t_gameGrid*);


#endif // GAMEGRID_H

