#ifndef DRAW_H
#define DRAW_H

#include "gameGrid.h"
#include "definitions.h"

#define CIRCLESTEP (2*PI)/64

/**
  * move um bando de caras por ai
  */
void moveDudes(t_character *dudes, int numDudes);

void drawCrossHair(double radius,double width);

/**
Atualiza a posi��o e orienta��o da camera
*/
void updateCam(t_camera *camera,t_character *chr);

//void setViewport(GLint left, GLint right, GLint bottom, GLint top);

/**
  * desenha um cara
  */
void drawDude(t_character *dude);

void drawWall(t_block *block);

/**
  * desenha todos os muros que existem
  */
void drawGrid(t_gameGrid *grid,t_gridTextures *texes);

/**
  * desenha um retangulo na regiao dada, assumindo que uma textura ja existe
  */
void drawRectangle(t_rect3 *region,double texScaleX,double texScaleY);

void drawScene(t_scene *scene,t_camera *cameram,t_character *chr,int numChars,int width,int height,double crossWidth);

#endif // DRAW_H

