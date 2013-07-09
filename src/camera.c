#include <math.h>

#include <SDL/SDL_opengl.h>

#include "camera.h"

/**
  * atualiza a camera em terceira pessoa
  */
void updateCamTP(t_camera *camera, t_character *chr)
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
	double height = chr->height;
	eye = camera->pos;
	eye[0] -= sin(camera->rot[1])*sin(camera->rot[2]);
	eye[1] -= cos(camera->rot[1]);
	eye[2] -= sin(camera->rot[1])*cos(camera->rot[2]);
	look[0] = eye[0] + sin(camera->rot[1])*sin(camera->rot[2]);
	look[1] = eye[1] + cos(camera->rot[1]);
	look[2] = eye[2] + sin(camera->rot[1])*cos(camera->rot[2]);

	gluLookAt(eye[0],eye[1]+height,eye[2],look[0],look[1]+height,look[2],0,1,0);
	//coloca a iluminacao no ponto certo
	float lpos[] = {chr->pos[0] , chr->pos[1] + 1.1*chr->height , chr->pos[2],1.0};
	glLightfv(chr->lighting, GL_POSITION, lpos);
}

/**
  * atualiza a camera em primeira pessoa
  */
void updateCamFP(t_camera *camera, t_character *chr)
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
	double height = chr->height;
	eye = camera->pos;
	look[0] = eye[0] + sin(camera->rot[1])*sin(camera->rot[2]);
	look[1] = eye[1] + cos(camera->rot[1]);
	look[2] = eye[2] + sin(camera->rot[1])*cos(camera->rot[2]);

	gluLookAt(eye[0],eye[1]+height,eye[2],look[0],look[1]+height,look[2],0,1,0);

	//coloca a iluminacao no ponto certo
	float lpos[] = {chr->pos[0] , chr->pos[1] + 1.1*chr->height , chr->pos[2],1.0};
	glLightfv(chr->lighting, GL_POSITION, lpos);

}

/**
  * Atualiza a posição e orientação da camera
  */
void updateCam(t_camera *camera,t_character *chr)
{

	if(chr->firstPerson)
		updateCamFP(camera,chr);
	else
		updateCamTP(camera,chr);
}
