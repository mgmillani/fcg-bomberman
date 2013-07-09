/*typedef enum e_gridElements;
typedef struct s_gameGrid t_gameGrid;
typedef struct s_gridTextures t_gridTextures;
*/
#ifndef GAMEGRID_H
#define GAMEGRID_H

typedef struct s_minimap
{
	float player[3];
	float enemy[3];
	float breakable[3];
	float unbreakable[3];
	float floor[3];
	float fire[3];
}t_minimap;

#include <SDL/SDL_opengl.h>

#include "play.h"

#define NUM_POWERUPS 3

typedef enum {Empty,Bomb,Fire,EnemySpawn,Spawn,PowerupBomb,PowerupPower,PowerupSpeed,BreakableWall,UnbreakableWall,NumElements} e_gridElements;

typedef struct s_gameGrid
{
	e_gridElements *grid;     //indica que tipo de coisa que existe em cada ponto do grid
	unsigned int w,h;         //altura e largura do grid
	double cellSize;          //tamanho das celulas do grid ao serem desenhadas
	unsigned int spawnPoints; //numero de spawnpoints no mapa
	unsigned int enemySpawnPoints; //numero de spawnpoints de inimigos no mapa
}t_gameGrid;

typedef struct s_colorMap
{
	unsigned char emptyColor[3];
	unsigned char spawnColor[3];
	unsigned char enemySpawnColor[3];
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
  * transforma todas as ocorrencias de Fire por Empty
  */
void cleanFire(t_gameGrid *grid);

/**
  * cria as cores para cada elemento que aparece no minimapa
  */
void createMinimapColors(struct s_gameData *data);

/**
  * escreve os dados da grid na tela
  */
void gameGridPrint(t_gameGrid*);


#endif // GAMEGRID_H

