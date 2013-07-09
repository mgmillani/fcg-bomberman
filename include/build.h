#ifndef BUILD_H
#define BUILD_H

#include "definitions.h"

void buildLeftWall(t_rect3 *region,double w, double h);
void buildRightWall(t_rect3 *region,double w, double h);
void buildFrontWall(t_rect3 *region,double w, double h);
void buildBackWall(t_rect3 *region,double w, double h);
void buildFloor(t_rect3 *region,double w, double h);
void buildCeiling(t_rect3 *region,double w, double h);
void buildFloorOld(t_rect *region);

/**
  * constroi um bloco com as dimensoes dadas
  * as coordenadas representam o centro do bloco
  */
void buildBlock(t_block *block,double pos[3],double size[3]);

/**
  * constroi uma caixa fechada (6 lados) com as dimensoes dadas, centrada em (0,0,0)
  */
void buildBox(t_rect3 box[6], double width, double height, double depth);

#endif // BUILD_H

