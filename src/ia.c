#include "ia.h"
#include "play.h"
#include "random.h"

#include "debug.h"

/**
  * faz um passo da ia dos inimigos
  * eles se movem aleatoriamente ate o jogador estar em uma reta em relacao a eles
  */
void enemyAI(t_gameData *data)
{

	unsigned int e;
	int px,py;
	t_gameGrid *grid = data->grid;
	px = data->player->pos[0] / grid->cellSize + 0.5;
	py = data->player->pos[2] / grid->cellSize + 0.5;
	for(e=0 ; e<data->numEnemies ; e++)
	{
		e_character *enemy = data->enemies + e;
		int ex,ey;
		ex = enemy->pos[0]/data->grid->cellSize + 0.5;
		ey = enemy->pos[2]/data->grid->cellSize + 0.5;
		//verifica se o jogador pode ser visto
		//esta na mesma coluna
		int dx=-2,dy=-2;
		char clear = 0;
		if(px == ex)
		{
			dx = 0;
			if(py < ey)
				dy = -1;
			else
				dy = 1;
			clear = 1;
		}
		//mesma linha
		else if(py == ey)
		{
			dy = 0;
			if(px < ex)
				dx = -1;
			else
				dx = 1;
			clear = 1;
		}
		//se o jogador estiver em linha reta
		if(clear)
		{
			//verifica se o caminho esta livre
			int distance = 0;
			for(;ey>=0 && ey<grid->h && ex>=0 && ex<grid->w && clear ; ey+=dy,ex+=dx,distance++)
			{
				int pos = ex + ey*grid->w;
				//se achou o jogador, vai!
				if(ex == px && ey==py)
				{
					//se a distancia for 1, mata o jogador
					data->player->dead = 1;
					break;
				}
				switch(grid->grid[pos])
				{
					case BreakableWall:
					case UnbreakableWall:
					case Bomb:
					case Fire:
						clear = 0;
						break;
					default:
						break;
				}
			}
			//se estiver, anda naquela direcao
			if(clear)
			{
				enemy->numMovement = 0;
				enemy->acc[0] += dx*enemy->walkSpeed;
				enemy->acc[2] += dy*enemy->walkSpeed;
			}
		}

		//se move aleatoriamente
		if(!clear)
		{
			if(enemy->numMovement == 0)
			{
				//gera uma quantidade aleatoria de passos
				enemy->numMovement = randrange(10)+1;
				switch(randrange(3))
				{
					case 0:
						enemy->dir[0] = 1;
						enemy->dir[2] = 0;
						break;
					case 1:
						enemy->dir[0] = -1;
						enemy->dir[2] = 0;
						break;
					case 2:
						enemy->dir[0] = 0;
						enemy->dir[2] = 1;
						break;
					case 3:
						enemy->dir[0] = 0;
						enemy->dir[2] = -1;
						break;
				}
			}
			enemy->acc[0] += enemy->dir[0]*enemy->walkSpeed;
			enemy->acc[2] += enemy->dir[2]*enemy->walkSpeed;
		}


	}


}
