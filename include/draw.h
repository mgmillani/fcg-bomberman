#ifndef DRAW_H
#define DRAW_H

#include "gameGrid.h"
#include "play.h"

#include "definitions.h"

#define CIRCLESTEP (2*PI)/64

/**
  * move um bando de caras por ai
  */
void moveDudes(t_character *dudes, int numDudes);

void drawCrossHair(double radius,double width);

/**
Atualiza a posição e orientação da camera
*/
void updateCam(t_camera *camera,t_character *chr);

//void setViewport(GLint left, GLint right, GLint bottom, GLint top);

void drawWall(t_block *block);

/**
  * desenha todos os muros que existem
  */
void drawGrid(t_gameGrid *grid,t_gridTextures *texes,double cellSize,double cellHeight);

/**
  * desenha um retangulo na regiao dada, assumindo que uma textura ja existe
  */
void drawRectangle(t_rect3 *region,double texScaleX,double texScaleY);

void drawScene(t_scene *scene,t_camera *cameram,t_character *chr,int numChars,int width,int height,double crossWidth,t_gameData *game);

#endif // DRAW_H

