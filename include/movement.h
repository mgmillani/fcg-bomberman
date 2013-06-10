#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "definitions.h"

/**
  * move um bando de caras por ai
  */
void moveDudes(t_character *dudes, int numDudes);

/**
  * baseando-se na direcao para a qual o personagem esta olhando, atualiza para onde esta andando
  */
void updateCharacterWalkDir(t_character *chr);

/**
  * atualiza a velocidade, aceleracao e posicao do personagem
  */
void moveCharacter(t_character *chr);

/**
  * atualiza para onde o personagem esta olhando baseando-se no movimento do mouse
  */
void treatMouseMovement(t_character *chr, int deltax, int deltay);

/**
  * determina o movimento do personagem com base nas teclas pressionadas
  */
void treatKeyStateCharacter(t_character *character, Uint8 *keystate,int numKeys);

#endif // MOVEMENT_H

