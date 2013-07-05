#ifndef LOADER_H
#define LOADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "abp.h"
#include "play.h"
#include "gameGrid.h"

/**
  *  carrega o grid do jogo a partir de uma imagem
  */
t_gameGrid *loadGrid(const char *fname,t_gameGrid *grid);

/**
  * carrega um mapa
  */
t_gameData *loadMap(const char *configFile,const char *mapName,t_gameData *data);

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
t_gridTextures *loadTextures(t_abp *types, t_gridTextures *textures);

/**
  * converte uma SDL_Surface para uma textura do OpenGL
  */
GLuint surface2texture(SDL_Surface *surface);

/**
  * carrega uma imagem em uma textura
  */
GLuint loadTexture(const char *fname);

#endif // LOADER_H
