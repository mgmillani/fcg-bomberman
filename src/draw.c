#include <math.h>

#include "draw.h"
#include "definitions.h"
#include "build.h"

#include "debug.h"

/**
Atualiza a posição e orientação da camera
*/

void updateCam(t_camera *camera,t_character *chr)
{

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	int i;
	for(i=0 ; i<3 ; i++)
	{
		camera->pos[i] = chr->pos[i];
		camera->rot[i] = chr->look[i];
	}

	double *eye;
	double look[3];
	double height = chr->height + sin(chr->headBob)*0.065;
	//ERR("Bob: %lf\n",height-chr->height);
	eye = camera->pos;
	look[0] = eye[0] + sin(camera->rot[1])*sin(camera->rot[2]);
	look[1] = eye[1] + cos(camera->rot[1]);
	look[2] = eye[2] + sin(camera->rot[1])*cos(camera->rot[2]);

	gluLookAt(eye[0],eye[1]+height,eye[2],look[0],look[1]+height,look[2],0,1,0);

	/*listenerPos[0] = posX;
	listenerPos[1] = posY;
	listenerPos[2] = posZ;

	source0Pos[0] = posX;
	source0Pos[1] = posY;
	source0Pos[2] = posZ;*/

}

void drawCrossHair(double radius,double width)
{

	double i;
	glColor4f(1,0,0,0.65);
	glLineWidth(width);
	glBegin(GL_LINE_LOOP);
		for(i=0 ; i<2*PI; i+= CIRCLESTEP)
		{
			glVertex2f(radius*cos(i),radius*sin(i));
		}
		glVertex2f(radius,0);
	glEnd();

	glColor4f(0,1,0,0.65);
	glBegin(GL_TRIANGLE_FAN);
		radius *=0.2;
		glVertex2f(0,0);
		for(i=0 ; i<2*PI; i+= CIRCLESTEP)
			glVertex2f(radius*cos(i),radius*sin(i));
		glVertex2f(radius,0);

	glEnd();
}

/**
  * desenha um cara
  */
void drawDude(t_character *dude)
{

	double width = dude->height/2;
	double vector[2];	//vetor normal a direcao de movimento. y = 0
	vector[0] = -dude->dir[2];
	vector[1] = dude->dir[0];
	double height = dude->height + sin(dude->headBob)*0.065;

	t_rect3 area;
	area.pos[0] = dude->pos[0] - width*vector[0]/2;
	area.pos[1] = dude->pos[1] + height*1.1;
	area.pos[2] = dude->pos[2] - width*vector[1]/2;
	area.w[0] = width*vector[0];
	area.w[1] = 0;
	area.w[2] = width*vector[1];

	area.v[0] = 0;
	area.v[1] = -dude->height*1.1;
	area.v[2] = 0;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,dude->texture);

	drawRectangle(&area,1,1);

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

void drawScene(t_scene *scene,t_camera *camera,t_character *chr,int numChars,int width,int height, double crossWidth)
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,scene->textures[Floor]);
	drawRectangle(&(scene->walls[Floor]),10,10);
	glBindTexture(GL_TEXTURE_2D,scene->textures[FrontWall]);
	drawRectangle(&(scene->walls[FrontWall]),2.5,2.5);
	drawRectangle(&(scene->walls[BackWall]),2.5,2.5);
	drawRectangle(&(scene->walls[LeftWall]),2.5,2.5);
	drawRectangle(&(scene->walls[RightWall]),2.5,2.5);
	glBindTexture(GL_TEXTURE_2D,scene->textures[Ceiling]);
	drawRectangle(&(scene->walls[Ceiling]),1,1);

	//desenha os caras
	int i;
	for(i=1 ; i<numChars ; i++)
	{
		drawDude(chr+i);
	}

	//desenha a crosshair
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float minx = -1;
	float maxx = 1;
	float miny = -1/ratio;
	float maxy = 1/ratio;
	glOrtho(minx,maxx,miny,maxy,0.0f,1.0f);
	drawCrossHair(0.05,crossWidth);
}
