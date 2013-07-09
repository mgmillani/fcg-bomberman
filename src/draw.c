#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "draw.h"
#include "build.h"
#include "play.h"
#include "player.h"
#include "enemies.h"
#include "camera.h"
#include "bomb.h"

#include "definitions.h"
#include "enemies.h"

#include "debug.h"


void drawWall(t_block *block)
{
	drawRectangle(&block->left,block->texScaleX,block->texScaleY);
	drawRectangle(&block->up,block->texScaleX,block->texScaleY);
	drawRectangle(&block->down,block->texScaleX,block->texScaleY);
	drawRectangle(&block->right,block->texScaleX,block->texScaleY);
}

void drawGrid(t_gameData *data,t_gridTextures *texes,double cellHeight)
{
	t_gameGrid *grid = data->grid;
	unsigned int i,j,pos;
	double location[3] = {0,0,0};
	double size[3] = {grid->cellSize,cellHeight,grid->cellSize};
	t_block wall;
	wall.texScaleX = 1;
	wall.texScaleY = 1;

	glBindTexture(GL_TEXTURE_2D,texes->weakWall);
	for(location[2]=0,pos=0,i=0 ; i<grid->h ; i++,location[2]+=size[2])
	{
		for(j=0,location[0]=0 ; j<grid->w ; j++,pos++,location[0]+=size[0])
		{
			if(grid->grid[pos] == BreakableWall)
			{
				buildBlock(&wall,location,size);
				drawWall(&wall);
			}
		}
	}

	glBindTexture(GL_TEXTURE_2D,texes->strongWall);
	for(location[2]=0,pos=0,i=0 ; i<grid->h ; i++,location[2]+=size[2])
	{
		for(j=0,location[0]=0 ; j<grid->w ; j++,pos++,location[0]+=size[0])
		{
			if(grid->grid[pos] == UnbreakableWall)
			{
				buildBlock(&wall,location,size);
				drawWall(&wall);
			}
		}
	}

	t_rect3 floor;
	floor.pos[1] = 0;
	floor.v[0] = size[0];
	floor.v[1] = 0;
	floor.v[2] = 0;

	floor.w[0] = 0;
	floor.w[1] = 0;
	floor.w[2] = size[2];
	glBindTexture(GL_TEXTURE_2D,texes->floor);
	for(floor.pos[2]=-size[2]/2,pos=0,i=0 ; i<grid->h ; i++,floor.pos[2]+=size[2])
		for(j=0,floor.pos[0]=-size[0]/2 ; j<grid->w ; j++,pos++,floor.pos[0]+=size[0])
			if(grid->grid[pos] != UnbreakableWall && grid->grid[pos] != BreakableWall)
				drawRectangle(&floor,1,1);

	floor.v[2] = floor.w[2];
	floor.w[0] = floor.v[0];
	floor.v[0] = 0;
	floor.w[2] = 0;

	glBindTexture(GL_TEXTURE_2D,texes->ceiling);
	floor.pos[1] = cellHeight;
	for(floor.pos[2]=-size[2]/2,pos=0,i=0 ; i<grid->h ; i++,floor.pos[2]+=size[2])
		for(j=0,floor.pos[0]=-size[0]/2 ; j<grid->w ; j++,pos++,floor.pos[0]+=size[0])
			if(grid->grid[pos] != UnbreakableWall && grid->grid[pos] != BreakableWall)
				drawRectangle(&floor,1,1);

	//powerups
	double x,z;
	for(z=-size[2]/2,pos=0,i=0 ; i<grid->h ; i++,z+=size[2])
		for(j=0,x=-size[0]/2 ; j<grid->w ; j++,pos++,x+=size[0])
		{
			t_powerup *p = NULL;
			if(grid->grid[pos] == PowerupBomb)
				p = &data->bombPowerup;
			else if	(grid->grid[pos] == PowerupSpeed)
				p = &data->speedPowerup;
			else if	(grid->grid[pos] == PowerupPower)
				p = &data->powerPowerup;

			if(p!=NULL)
			{
				glPushMatrix();
				glTranslated(x+size[0]/2,0.5,z+size[0]/2);
				drawPowerup(p);
				glPopMatrix();
			}
		}

}
/**
  * desenha um retangulo na regiao dada, assumindo que uma textura ja existe
  */
void drawRectangle(t_rect3 *region,double texScaleX,double texScaleY)
{
	glBegin(GL_QUADS);

	double *pos = region->pos;
	double *v = region->v;
	double *w = region->w;

	glTexCoord2f(0, 0.0f);   // coords for the texture
	glVertex3f(pos[0],pos[1],pos[2]);

	glTexCoord2f(texScaleX, 0);   // coords for the texture
	glVertex3f(pos[0]+w[0],pos[1]+w[1],pos[2]+w[2]);

	glTexCoord2f(texScaleX, texScaleY);   // coords for the texture
	glVertex3f(pos[0]+v[0]+w[0],pos[1]+v[1]+w[1],pos[2]+v[2]+w[2]);

	glTexCoord2f(0, texScaleY);   // coords for the texture
	glVertex3f(pos[0]+v[0],pos[1]+v[1],pos[2]+v[2]);



	glEnd();
}

/**
  * desenha um personagem em sua devida posicao
  */
void drawCharacter(t_character *chr)
{
	glPushMatrix();
	glTranslated(chr->pos[0],chr->pos[1]+chr->wheelRadius,chr->pos[2]);
	//glRotatef(chr->look[2]*180/PI+90,0,1,0);
	glColor3f(1.0,1.0,1.0);
	drawRobotA(chr);
	glPopMatrix();
}

/**
  * desenha os inimigos em suas devidas posições
  */
void drawEnemies(e_character *chra,unsigned int numEnemies)
{
	unsigned int i;
	for(i=0 ; i<numEnemies ; i++)
	{
		glPushMatrix();
		glTranslated(chra[i].pos[0],chra[i].pos[1]+chra[i].wheelRadius,chra[i].pos[2]);
		//glColor3f(1.0,1.0,1.0);
		drawEnemy(chra+i);
		glPopMatrix();
	}
}


/**
  * desenha as bombas
  */
void drawBombs(t_list *bombs, double cellSize)
{
	t_listNode *bomb;

	for(bomb=bombs->first ; bomb!=NULL ; bomb=bomb->next)
	{
		t_bomb *b = bomb->key;
		double x = b->pos[0]*cellSize;
		double y = b->pos[1]*cellSize;
		glTranslated(x,0,y);
		drawBomb(b);
		glTranslated(-x,0,-y);
	}
}

void drawScene(t_scene *scene,t_camera *camera,t_character *chr,int numChars,int width,int height, double crossWidth,t_gameData *game)
{

	//projecao em perspectiva
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double ratio = (double)width/height;
	gluPerspective( 60.0, ratio, 0.1, 1024.0 );
	//atualiza a camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	updateCam(camera,chr);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(chr->lighting);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	drawGrid(game,game->textures,3.0);
	//glDisable(GL_TEXTURE_2D);
	drawCharacter(chr);
	drawEnemies(game->enemies,game->numEnemies);
	drawBombs(&(game->bombs),game->grid->cellSize);
	drawExplosions(game);

	//desenha o minimapa
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	gluOrtho2D(-1,1,-1,1);
	glTranslatef(-1,-1,0);
	glScalef(0.2,0.2,1.0);
	drawMinimap(game->grid,&game->minimap);
}

/**
  * desenha o minimapa na tela
  */
void drawMinimap(t_gameGrid *grid,t_minimap *colors)
{

	double x,y;
	double cellSize = 0.1;
	double x0 = -cellSize*grid->w/2;
	x = 0;
	//y = -cellSize*grid->h/2;
	y=0;

	int i,j;
	int pos=0;
	for(i=0 ; i<grid->h ; i++,y+=cellSize)
	{
		for(x=0,j=0 ; j<grid->w ; pos++,j++,x+=cellSize)
		{
			switch(grid->grid[pos])
			{
				case BreakableWall:
					glColor3fv(colors->breakable);
					break;
				case UnbreakableWall:
					glColor3fv(colors->unbreakable);
					break;
				case Fire:
					glColor3fv(colors->fire);
					break;
				default:
					glColor3fv(colors->floor);
			}

			glBegin(GL_QUADS);
			glVertex2f(x,y);
			glVertex2f(x,y+cellSize);
			glVertex2f(x+cellSize,y+cellSize);
			glVertex2f(x+cellSize,y);
			glEnd();
		}
	}

}






