#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "enemies.h"
#include "definitions.h"

#include "debug.h"

/**
  * desenha os inimigos
  */
void drawEnemy(e_character *chra)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,chra->texture);
	gluQuadricTexture(chra->wheel,GLU_TRUE);
	gluQuadricTexture(chra->body,GLU_TRUE);
	gluQuadricTexture(chra->arm,GLU_TRUE);
	gluQuadricTexture(chra->shoulder,GLU_TRUE);
	gluQuadricTexture(chra->neck,GLU_TRUE);

	double lookAngle;	//angulo para onde o personagem esta olhando (em graus)
	lookAngle = chra->look[2]*180/PI;

	//desenha a roda
	glPushMatrix();
	glRotatef(chra->wheelRot,chra->wheelNorm[0],chra->wheelNorm[1],chra->wheelNorm[2]);
	gluSphere(chra->wheel,chra->wheelRadius,16,16);
	glPopMatrix();

	//rotaciona para onde o personagem esta olhando
	//desenha o corpo
	glPushMatrix();
	double distance = chra->wheelRadius/10;
	double wheelHeight = distance + chra->wheelRadius*2;
	double headHeight = chra->height - (chra->wheelRadius*3 + chra->neckHeight);
	glTranslated(0,headHeight,0);
	glRotatef(lookAngle,0,1,0);
	glRotatef(90,1,0,0);
	gluCylinder(chra->body,chra->wheelRadius,chra->wheelRadius*0.1,headHeight-chra->wheelRadius,16,4);
	glRotatef(180,1,0,0);
	gluCylinder(chra->neck,chra->wheelRadius,0,chra->neckHeight,16,4);
	glPopMatrix();

	//desenha os bracos
	glPushMatrix();
	glRotatef(lookAngle,0,1,0);
	glTranslated(chra->wheelRadius,headHeight,0);
	glRotatef(0,1,0,0);
	gluCylinder(chra->arm,chra->wheelRadius*0.7,0,(chra->height-wheelHeight)*0.8,16,4);
	glRotatef(180,1,0,0);
	gluCylinder(chra->shoulder,chra->wheelRadius*0.7,0,(chra->height-wheelHeight)*0.1,16,4);
	glPopMatrix();

	glPushMatrix();
	glRotatef(lookAngle,0,1,0);
	glTranslated(-chra->wheelRadius,headHeight,0);
	glRotatef(0,1,0,0);
	gluCylinder(chra->arm,chra->wheelRadius*0.7,0,(chra->height-wheelHeight)*0.8,16,4);
	glRotatef(180,1,0,0);
	gluCylinder(chra->shoulder,chra->wheelRadius*0.7,0,(chra->height-wheelHeight)*0.1,16,4);
	glPopMatrix();

	/*//desenha os chifres
	glPushMatrix();
	glRotatef(lookAngle,0,1,0);
	glTranslated(0,headHeight+chra->neckHeight+chra->wheelRadius,0);
	glRotatef(270,1,0,0);
	gluCylinder(chra->arm,chra->wheelRadius*0.3,0,(chra->height-wheelHeight)*0.3,16,4);
	glRotatef(180,1,0,0);
	gluCylinder(chra->shoulder,chra->wheelRadius*0.7,0,(chra->height-wheelHeight)*0.1,16,4);
	glPopMatrix();

	glPushMatrix();
	glRotatef(lookAngle,0,1,0);
	glTranslated(0,headHeight+chra->neckHeight+chra->wheelRadius,0);
	glRotatef(270,1,0,0);
	gluCylinder(chra->arm,chra->wheelRadius*0.3,0,(chra->height-wheelHeight)*0.3,16,4);
	glRotatef(180,1,0,0);
	gluCylinder(chra->shoulder,chra->wheelRadius*0.7,0,(chra->height-wheelHeight)*0.1,16,4);
	glPopMatrix();*/

	//cabeca
	glPushMatrix();
	glTranslated(0,headHeight+chra->neckHeight+chra->wheelRadius,0);
	glRotatef(lookAngle,0,1,0);
	gluSphere(chra->wheel,chra->wheelRadius,16,16);
	glPopMatrix();

}

/**
  * coloca um inimigo em cada EnemySpawnpoint
  */
void createEnemies(t_gameData *data)
{
	t_gameGrid *grid = data->grid;

	data->enemies = malloc(sizeof(*data->enemies)*grid->enemySpawnPoints);
	data->numEnemies = grid->enemySpawnPoints;
	unsigned int x,y,pos;
	unsigned int e=0;
	for(pos=0,y=0 ; y<grid->h ; y++)
	{
		for(x=0 ; x<grid->w ; x++,pos++)
		{
			if(grid->grid[pos] == EnemySpawn)
			{
				initEnemies(data->enemies+e,data->bombTexture);
				data->enemies[e].pos[0] = x*grid->cellSize;
				data->enemies[e].pos[1] = 0;
				data->enemies[e].pos[2] = y*grid->cellSize;
				e++;
			}
		}
	}
}









