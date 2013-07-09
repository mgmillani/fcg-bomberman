#ifndef ENEMIES_H
#define ENEMIES_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "play.h"
#include "definitions.h"

/**
  * desenha os inimigos
  */
void drawEnemy(e_character *chra);

/**
  * desenha os inimigos
  */
//void drawEnemies(e_character *chra,int numEnemies);

/**
  * coloca um inimigo em cada EnemySpawnpoint
  */
void createEnemies(t_gameData *data);

#endif // ENEMIES_H
