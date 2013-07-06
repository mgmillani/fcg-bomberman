
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "configLoader.h"
#include "gameGrid.h"
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
	//carrega o grid
	t_abp *gridFile = abpSearchNode(gMapKeyword,mapInfo,(int (*)(const void*,const void*))strcmp);
	t_gameGrid *grid = loadGrid(gridFile->data,NULL);

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
	return surface2texture(img);
}
