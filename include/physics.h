#ifndef PHYSICS_H
#define PHYSICS_H

#include "play.h"
#include "definitions.h"

/**
  * faz com que o personagem nao escape da cena
  */
void forceWithinScene(t_character *chr, t_scene *scene);

/**
  * atualiza os vetores da particula
  */
void moveParticle(t_particle *part);

void enemyCollision(e_character *chr,t_gameData *data);

void playerCollision(t_character *chra,t_gameData *data);

/**
  * simula a fisica dos personagens
  */
void simulatePhysics(t_gameData *data, t_scene *scene,double gravity[3]);

#endif // PHYSICS_H

