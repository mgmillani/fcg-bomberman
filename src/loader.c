
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "configLoader.h"
#include "gameGrid.h"
#include "powerup.h"
#include "loader.h"
#include "play.h"
#include "list.h"
#include "abp.h"

#include "debug.h"

const char gMapKeyword[] = "map";

const char gWeakKeyword[] = "weak";
const char gStrongKeyword[] = "strong";
const char gFloorKeyword[] = "floor";
const char gCeilingKeyword[] = "ceiling";
const char gBombKeyword[] = "bomb";
const char gFuseKeyword[] = "fuse";
const char gFireKeyword[] = "fire";
const char gSmokeKeyword[] = "smoke";
const char gBorderPWKeyword[] = "border";
const char gBombPWKeyword[] = "bomb_powerup";
const char gSpeedPWKeyword[] = "speed_powerup";
const char gPowerPWKeyword[] = "power_powerup";

t_gameGrid *loadGrid(const char *fname,t_gameGrid *grid)
{
	if(grid == NULL)
		grid = malloc(sizeof(*grid));

	t_colorMap colorMap;
	colorMap.emptyColor[0] = 0;
	colorMap.emptyColor[1] = 0;
	colorMap.emptyColor[2] = 0;

	colorMap.breakableColor[0] = 255;
	colorMap.breakableColor[1] = 0;
	colorMap.breakableColor[2] = 0;

	colorMap.unbreakableColor[0] = 0;
	colorMap.unbreakableColor[1] = 255;
	colorMap.unbreakableColor[2] = 0;

	colorMap.spawnColor[0] = 0;
	colorMap.spawnColor[1] = 0;
	colorMap.spawnColor[2] = 255;

	colorMap.enemySpawnColor[0] = 0;
	colorMap.enemySpawnColor[1] = 255;
	colorMap.enemySpawnColor[2] = 255;
	gameGridLoad(grid,fname,&colorMap);
	//gameGridPrint(grid);

	return grid;
}

/**
  * carrega um mapa
  */
t_gameData *loadMap(const char *configFile,const char *mapName,t_gameData *data)
{
	if(data == NULL)
		data = malloc(sizeof(*data));

	createMinimapColors(data);

	t_list errors;
	listInit(&errors);
	t_list *cfg = loadConfig(configFile,mapName,&errors);
	//se ocorreu algum erro
	if(errors.length > 0)
	{
		t_listNode *er;
		ERR("Error while loading textures from %s\n",configFile);
		for(er=errors.first ; er != NULL ; er=er->next)
		{
			ERR("Error:%s\n",(char *)er->key);
		}
		free(data);
		configDestroy(cfg,&errors);
		return NULL;
	}

	//carrega as texturas
	t_abp *mapInfo = listSearch(cfg,mapName,(int (*)(const void*,const void*))strcmp)->data;
	t_gridTextures *textures = loadTextures(mapInfo,NULL);
	//bomba
	t_abp *path = abpSearchNode(gBombKeyword,mapInfo,(int (*)(const void*,const void*))strcmp);
	data->bombTexture = loadTexture(path->data);
	//pavio
	path = abpSearchNode(gFuseKeyword,mapInfo,(int (*)(const void*,const void*))strcmp);
	data->fuseTexture = loadTexture(path->data);
	//fogo
	path = abpSearchNode(gFireKeyword,mapInfo,(int (*)(const void*,const void*))strcmp);
	data->fireTexture = loadTexture(path->data);
	//fumaca
	path = abpSearchNode(gSmokeKeyword,mapInfo,(int (*)(const void*,const void*))strcmp);
	data->smokeTexture = loadTexture(path->data);
	//borda dos powerups
	path = abpSearchNode(gBorderPWKeyword,mapInfo,(int (*)(const void*,const void*))strcmp);
	GLuint border = loadTexture(path->data);
	//powerup de bomba
	path = abpSearchNode(gBombPWKeyword,mapInfo,(int (*)(const void*,const void*))strcmp);
	GLuint pw = loadTexture(path->data);
	powerupInit(&data->bombPowerup,0.3,border,pw);
	//powerup de velocidade
	path = abpSearchNode(gSpeedPWKeyword,mapInfo,(int (*)(const void*,const void*))strcmp);
	pw = loadTexture(path->data);
	powerupInit(&data->speedPowerup,0.3,border,pw);
	//powerup de poder
	path = abpSearchNode(gPowerPWKeyword,mapInfo,(int (*)(const void*,const void*))strcmp);
	pw = loadTexture(path->data);
	powerupInit(&data->powerPowerup,0.3,border,pw);

	//carrega o grid
	path = abpSearchNode(gMapKeyword,mapInfo,(int (*)(const void*,const void*))strcmp);
	t_gameGrid *grid = loadGrid(path->data,NULL);

	data->grid = grid;
	data->textures = textures;

	return data;
}

/**
  * carrega as texturas descritas em types. eh usado o seguinte padrao:
  * ----------------+-------------------------------------------------
  *      chave      |                   dado
  * ----------------+-------------------------------------------------
  * gWeakKeyword    | destino para a textura de muros destrutiveis
  * gStrongKeyword  | destino para a textura de muros indestrutiveis
  * gFloorKeyword   | destino para a textura do piso
  * gCeilingKeyword | destino para a textura do teto
  * ----------------+-------------------------------------------------
  */
t_gridTextures *loadTextures(t_abp *types, t_gridTextures *textures)
{
	if(textures == NULL)
		textures = malloc(sizeof(*textures));

	t_abp *path = abpSearchNode(gWeakKeyword,types,(int (*)(const void*,const void*))strcmp);
	if(path == NULL)
	{
		ERR("%s not defined\n",gWeakKeyword);
		return NULL;
	}
	textures->weakWall = loadTexture(path->data);

	path = abpSearchNode(gStrongKeyword,types,(int (*)(const void*,const void*))strcmp);
	if(path == NULL)
	{
		ERR("%s not defined\n",gStrongKeyword);
		return NULL;
	}
	textures->strongWall = loadTexture(path->data);

	path = abpSearchNode(gFloorKeyword,types,(int (*)(const void*,const void*))strcmp);
	if(path == NULL)
	{
		ERR("%s not defined\n",gFloorKeyword);
		return NULL;
	}
	textures->floor = loadTexture(path->data);

	path = abpSearchNode(gCeilingKeyword,types,(int (*)(const void*,const void*))strcmp);
	if(path == NULL)
	{
		ERR("%s not defined\n",gCeilingKeyword);
		return NULL;
	}
	textures->ceiling = loadTexture(path->data);

	return textures;
}

/**
  * converte uma SDL_Surface para uma textura do OpenGL
  */
GLuint surface2texture(SDL_Surface *surface)
{
	GLuint texture,width,height;
	if(surface->w == 0 || surface->h ==0)
		return 0;
	width = surface->w;
	height = surface->h;

	GLint ncolors,internalFormat;
	GLenum textureFormat;

	//determina o numero de canais da surface
	ncolors = surface->format->BytesPerPixel;

	switch(ncolors)
	{
		//com alpha channel
		case 4:
			internalFormat = GL_RGBA;
			if (surface->format->Rmask == 0xff)
				textureFormat = GL_RGBA;
			else
				textureFormat = GL_BGRA;
			break;

		//sem alpha channel
		case 3:
			internalFormat = GL_RGB;
			if (surface->format->Rmask == 0xff)
				textureFormat = GL_RGB;
			else
				textureFormat = GL_BGR;
			break;
		default:
			fprintf(stderr,"Wrong number of colors :%d\n",ncolors);
			return 0;
	}

	//gera o handle da textura
	glGenTextures(1,&texture);

	//faz o binding da textura criada
	glBindTexture(GL_TEXTURE_2D,texture);

	//determina as propriedades de stretching da textura
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	//usa a surface para criar a textura
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

	SDL_FreeSurface(surface);

	return texture;
}

/**
  * carrega uma imagem em uma textura
  */
GLuint loadTexture(const char *fname)
{
	SDL_Surface *img = IMG_Load(fname);
	if(img == NULL)
		ERR("Unable to open file : %s\n",fname);
	return surface2texture(img);
}
