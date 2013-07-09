
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

/**
  * constroi uma caixa fechada (6 lados) com as dimensoes dadas, centrada em (0,0,0)
  */
void buildBox(t_rect3 box[6], double width, double height, double depth)
{

	double w,h,d;
	w = width/2;
	h = height/2;
	d = depth/2;

	box[0].pos[0] = -w;
	box[0].pos[1] = -h;
	box[0].pos[2] = d;

	box[1].pos[0] = w;
	box[1].pos[1] = -h;
	box[1].pos[2] = d;

	box[2].pos[0] = w;
	box[2].pos[1] = -h;
	box[2].pos[2] = -d;

	box[3].pos[0] = -w;
	box[3].pos[1] = -h;
	box[3].pos[2] = -d;

	box[4].pos[0] = -w;
	box[4].pos[1] = -h;
	box[4].pos[2] = -d;

	box[5].pos[0] = -w;
	box[5].pos[1] = h;
	box[5].pos[2] = -d;

	box[0].v[0] = 0;
	box[0].v[1] = 0;
	box[0].v[2] = -depth;

	box[1].v[0] = -width;
	box[1].v[1] = 0;
	box[1].v[2] = 0;

	box[2].v[0] = 0;
	box[2].v[1] = 0;
	box[2].v[2] = depth;

	box[3].v[0] = width;
	box[3].v[1] = 0;
	box[3].v[2] = 0;

	box[4].v[0] = 0;
	box[4].v[1] = 0;
	box[4].v[2] = depth;

	box[5].v[0] = width;
	box[5].v[1] = 0;
	box[5].v[2] = 0;

	unsigned int i;
	for(i=0 ; i<4 ; i++)
	{
		box[i].w[0] = 0;
		box[i].w[1] = height;
		box[i].w[2] = 0;
	}

	box[4].w[0] = width;
	box[4].w[1] = 0;
	box[4].w[2] = 0;

	box[5].w[0] = 0;
	box[5].w[1] = 0;
	box[5].w[2] = depth;
}















