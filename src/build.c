
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

/**
  * constroi um bloco com as dimensoes dadas
  */
void buildBlock(t_block *block,double pos[3],double size[3])
{

	block->left.pos[0] = pos[0] - size[0]/2;
	block->left.pos[1] = pos[1];
	block->left.pos[2] = pos[2] + size[2]/2;

	block->up.pos[0] = pos[0] + size[0]/2;
	block->up.pos[1] = pos[1];
	block->up.pos[2] = pos[2] + size[2]/2;

	block->right.pos[0] = pos[0] + size[0]/2;
	block->right.pos[1] = pos[1];
	block->right.pos[2] = pos[2] - size[2]/2;

	block->down.pos[0] = pos[0] - size[0]/2;
	block->down.pos[1] = pos[1];
	block->down.pos[2] = pos[2] - size[2]/2;

	t_rect3 *sides[] = {&block->left,&block->right,&block->up,&block->down};

	block->left.v[0] = 0;
	block->left.v[1] = 0;
	block->left.v[2] = -size[2];

	block->up.v[0] = -size[0];
	block->up.v[1] = 0;
	block->up.v[2] = 0;

	block->right.v[0] = 0;
	block->right.v[1] = 0;
	block->right.v[2] = size[2];

	block->down.v[0] = size[0];
	block->down.v[1] = 0;
	block->down.v[2] = 0;

	unsigned int i;
	for(i=0 ; i<4 ; i++)
	{
		sides[i]->w[0] = 0;
		sides[i]->w[1] = size[1];
		sides[i]->w[2] = 0;
	}

}















