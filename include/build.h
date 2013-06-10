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

#endif // BUILD_H

