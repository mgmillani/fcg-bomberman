#ifndef INIT_H
#define INIT_H

#include "definitions.h"
#include "gameGrid.h"

/**
  * inicializa o personagem
  * se o ponteiro passado for NULL, aloca um novo personagem
  */
t_character *initCharacter(t_character *chr,GLuint texture,t_gameGrid *grid);

/**
  * inicializa a camera
  * se o ponteiro passado for NULL, aloca uma camera nova
  */
t_camera *initCamera(t_camera *camera, t_character *chr);

/**
  * inicializa a cena
  * se o ponteiro passado for NULL, aloca uma cena nova
  */
t_scene *initScene(t_scene *scene, t_gameGrid *grid);

#endif // INIT_H

