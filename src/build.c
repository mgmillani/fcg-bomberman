
#include "definitions.h"
#include "build.h"

void buildLeftWall(t_rect3 *region,double w, double h)
{
	region->pos[0] = -w/2;
	region->pos[1] = 0;
	region->pos[2] = -w/2;

	region->v[0] = 0;
	region->v[1] = 0;
	region->v[2] = w;

	region->w[0] = 0;
	region->w[1] = h;
	region->w[2] = 0;
}

void buildRightWall(t_rect3 *region,double w, double h)
{
	region->pos[0] = w/2;
	region->pos[1] = 0;
	region->pos[2] = -w/2;

	region->v[0] = 0;
	region->v[1] = 0;
	region->v[2] = w;

	region->w[0] = 0;
	region->w[1] = h;
	region->w[2] = 0;
}

void buildFrontWall(t_rect3 *region,double w, double h)
{
	region->pos[0] = -w/2;
	region->pos[1] = 0;
	region->pos[2] = w/2;

	region->v[0] = w;
	region->v[1] = 0;
	region->v[2] = 0;

	region->w[0] = 0;
	region->w[1] = h;
	region->w[2] = 0;
}

void buildBackWall(t_rect3 *region,double w, double h)
{
	region->pos[0] = -w/2;
	region->pos[1] = 0;
	region->pos[2] = -w/2;

	region->v[0] = w;
	region->v[1] = 0;
	region->v[2] = 0;

	region->w[0] = 0;
	region->w[1] = h;
	region->w[2] = 0;
}

void buildFloor(t_rect3 *region,double w, double h)
{
	region->pos[0] = -w/2;
	region->pos[1] = 0;
	region->pos[2] = -w/2;

	region->v[0] = w;
	region->v[1] = 0;
	region->v[2] = 0;

	region->w[0] = 0;
	region->w[1] = 0;
	region->w[2] = h;
}

void buildCeiling(t_rect3 *region,double w, double h)
{
	region->pos[0] = -w/2;
	region->pos[1] = h/3;
	region->pos[2] = -w/2;

	region->v[0] = w;
	region->v[1] = 0;
	region->v[2] = 0;

	region->w[0] = 0;
	region->w[1] = 0;
	region->w[2] = h;
}
