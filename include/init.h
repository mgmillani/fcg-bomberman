#ifndef INIT_H
#define INIT_H

#include "definitions.h"

/**
  * inicializa o personagem
  * se o ponteiro passado for NULL, aloca um novo personagem
  */
t_character *initCharacter(t_character *chr,GLuint texture);

/**
  * inicializa a camera
  * se o ponteiro passado for NULL, aloca uma camera nova
  */
t_camera *initCamera(t_camera *camera, t_character *chr);

/**
  * inicializa a cena
  * se o ponteiro passado for NULL, aloca uma cena nova
  * ground, wall e ceiling correspondem as texturas que serao usadas
  */
t_scene *initScene(t_scene *scene, GLuint ground, GLuint wall, GLuint ceiling);

#endif // INIT_H

