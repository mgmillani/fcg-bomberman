#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "player.h"
#include "definitions.h"

#include "debug.h"

/**
  * desenha um robo da classe A
  */
void drawRobotA(t_character *chr)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,chr->texture);
	gluQuadricTexture(chr->wheel,GLU_TRUE);
	gluQuadricTexture(chr->body,GLU_TRUE);
	gluQuadricTexture(chr->arm,GLU_TRUE);
	gluQuadricTexture(chr->shoulder,GLU_TRUE);
	gluQuadricTexture(chr->neck,GLU_TRUE);

	double lookAngle;	//angulo para onde o personagem esta olhando (em graus)
	lookAngle = chr->look[2]*180/PI;

	//desenha a roda
	glPushMatrix();
	glRotatef(chr->wheelRot,chr->wheelNorm[0],chr->wheelNorm[1],chr->wheelNorm[2]);
	gluSphere(chr->wheel,chr->wheelRadius,16,16);
	glPopMatrix();

	//rotaciona para onde o personagem esta olhando
	//desenha o corpo
	glPushMatrix();
	double distance = chr->wheelRadius/10;
	double wheelHeight = distance + chr->wheelRadius*2;
	double headHeight = chr->height - (chr->wheelRadius*3 + chr->neckHeight);
	glTranslated(0,headHeight,0);
	glRotatef(lookAngle,0,1,0);
	glRotatef(90,1,0,0);
	gluCylinder(chr->body,chr->wheelRadius,chr->wheelRadius*0.1,headHeight-chr->wheelRadius,16,4);
	//pescoco
	//glTranslatef(0,chr->height-wheelHeight,0);
	glRotatef(180,1,0,0);
	gluCylinder(chr->neck,chr->wheelRadius,0,chr->neckHeight,16,4);
	glPopMatrix();

	//desenha os bracos
	glPushMatrix();
	glRotatef(lookAngle,0,1,0);
	glTranslated(chr->wheelRadius,headHeight,0);
	glRotatef(45,1,0,0);
	gluCylinder(chr->arm,chr->wheelRadius*0.7,0,(chr->height-wheelHeight)*0.8,16,4);
	glRotatef(180,1,0,0);
	gluCylinder(chr->shoulder,chr->wheelRadius*0.7,0,(chr->height-wheelHeight)*0.1,16,4);
	glPopMatrix();

	glPushMatrix();
	glRotatef(lookAngle,0,1,0);
	glTranslated(-chr->wheelRadius,headHeight,0);
	glRotatef(45,1,0,0);
	gluCylinder(chr->arm,chr->wheelRadius*0.7,0,(chr->height-wheelHeight)*0.8,16,4);
	glRotatef(180,1,0,0);
	gluCylinder(chr->shoulder,chr->wheelRadius*0.7,0,(chr->height-wheelHeight)*0.1,16,4);
	glPopMatrix();


	//desenha somente em terceira pessoa
	if(chr->firstPerson==0)
	{
		//cabeca
		glPushMatrix();
		glTranslated(0,headHeight+chr->neckHeight+chr->wheelRadius,0);
		glRotatef(lookAngle,0,1,0);
		gluSphere(chr->wheel,chr->wheelRadius,16,16);
		glPopMatrix();
	}
}




