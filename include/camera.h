#ifndef CAMERA_H
#define CAMERA_H

#include "definitions.h"

typedef struct s_camera
{
	double pos[3];
	double rot[3]; //spherical coordinates (r,theta,phi)
}t_camera;

/**
  * atualiza a camera em terceira pessoa
  */
void updateCamTP(t_camera *camera, t_character *chr);

/**
  * atualiza a camera em primeira pessoa
  */
void updateCamFP(t_camera *camera, t_character *chr);

/**
  * Atualiza a posição e orientação da camera
  */
void updateCam(t_camera *camera,t_character *chr);

#endif // CAMERA_H

