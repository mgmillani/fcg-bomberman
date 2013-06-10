#ifndef PHYSICS_H
#define PHYSICS_H

#include "definitions.h"

/**
  * faz com que o personagem nao escape da cena
  */
void forceWithinScene(t_character *chr, t_scene *scene);

/**
  * simula a fisica dos personagens
  */
void simulatePhysics(t_character *chr,int numDudes, t_scene *scene,double gravity[3]);

#endif // PHYSICS_H

