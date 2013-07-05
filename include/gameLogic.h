#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "player.h"
#include "gameGrid.h"

typedef enum {Coop, Versus} e_gameMode;

typedef struct s_match
{
	t_player *players;
	int numPlayers;
	e_gameMode gameMode;
	t_gameGrid *grid;

}t_match;

/**
  * executa uma partida do jogo
  * retorna 0
  */
int gameMatch(t_match *match);

#endif // GAMELOGIC_H

