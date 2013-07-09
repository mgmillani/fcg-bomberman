#ifndef DRAW_H
#define DRAW_H

#include "gameGrid.h"
#include "play.h"
#include "camera.h"
#include "bomb.h"

#include "definitions.h"

#define CIRCLESTEP (2*PI)/64

/**
  * move um bando de caras por ai
  */
void moveDudes(t_character *dudes, int numDudes);

void drawCrossHair(double radius,double width);

//void setViewport(GLint left, GLint right, GLint bottom, GLint top);

void drawWall(t_block *block);

/**
  * desenha todos os muros que existem
  */
void drawGrid(t_gameData *grid,t_gridTextures *texes,double cellHeight);

/**
  * desenha um retangulo na regiao dada, assumindo que uma textura ja existe
  */
void drawRectangle(t_rect3 *region,double texScaleX,double texScaleY);

/**
  * desenha um personagem em sua devida posicao
  */
void drawCharacter(t_character *chr);

/**
  * desenha os inimigos em suas devidas posições
  */
void drawEnemies(e_character *chra);

/**
  * desenha as bombas
  */
void drawBombs(t_list *bombs, double cellSize);

void drawScene(t_scene *scene,t_camera *cameram,t_character *chr,e_character *chra,int numChars,int width,int height,double crossWidth,t_gameData *game);

#endif // DRAW_H

