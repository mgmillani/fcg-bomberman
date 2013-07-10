#include <stdlib.h>

#include <SDL/SDL.h>

#include "play.h"

#include "frameControl.h"
#include "draw.h"
#include "init.h"
#include "ia.h"
#include "physics.h"
#include "movement.h"
#include "loader.h"

#include "debug.h"

#define WIDTH 800
#define HEIGHT 600
#define DEFAULT_FPS 60

char should_rotate = 1;

const char gFloorFile[] = "textures/floor.png";
const char gWallFile[] = "textures/wall.jpg";
const char gSkyFile[] = "textures/bluesky.jpg";
const char gDudeFile[] = "textures/paintWheel.jpg";
const char gEnemyAFile[] = "textures/enemya.jpg";
const char gEnemyBFile[] = "textures/enemyb.jpg";
const char gEnemyCFile[] = "textures/enemyc.jpg";
const char gEnemyDFile[] = "textures/enemyd.jpg";

double gravity[3] = {0,-0.005,0};
double crossWidth = 2;

char checkVictory(t_gameData *game)
{
	// se o jogador morreu, game over
	if(game->player->dead == 1)
		return -1;
	//se todas os inimigos morreram, vitoria
	int i;
	int dead = 0;
	for(i=0 ; i<game->numEnemies ; i++)
	{
		dead += game->enemies[i].dead;
	}
	if(dead == game->numEnemies)
		return 1;

	return 0;
}

/**
  * laco principal do jogo
  */
char play(t_gameData *game)
{
	int numDudes = 1;
	//personagens
	GLuint dudeTex = loadTexture(gDudeFile);
	t_character *dudes = malloc(numDudes*sizeof(t_character));
	initCharacter(dudes,dudeTex,game->grid);
	t_camera camera;
	initCamera(&camera,dudes);

	game->player = dudes;

	//lista para as bombas
	t_list bombs;
	listInit(&bombs);

	//o cenario
	t_scene scene;
	initScene(&scene,game->grid);

	unsigned int fps = DEFAULT_FPS;
	t_frameController frameControl;
	initFrameController(&frameControl,fps);

	char play = 0;
	while(play == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Uint8 *keystate;
		int numKeys;
		frameControl.frameStart = SDL_GetTicks();
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				play = -1;
			else if(event.type == SDL_KEYDOWN)
			{
				treatKeyDownCharacters(dudes,&event);
			}
			else if(event.type == SDL_MOUSEMOTION)
			{
				if(event.motion.x != WIDTH/2 || event.motion.y != HEIGHT/2)
				{
					treatMouseMovement(dudes,event.motion.xrel,event.motion.yrel);
					SDL_WarpMouse(WIDTH/2,HEIGHT/2);
				}
			}
		}

		keystate = SDL_GetKeyState(&numKeys);
		//verifica se ESC foi pressionado
		if(keystate[SDLK_ESCAPE]==1)
			play = 0;

		treatKeyStateCharacter(dudes, keystate, numKeys);
		updateCharacterWalkDir(dudes);
		treatCharacterAction(dudes,game);

		enemyAI(game);
		simulatePhysics(game,&scene,gravity);

		//verifica se alguma bomba explodiu
		checkBombExplosion(game);
		//tira o fogo do cenario
		cleanFire(game->grid);
		simulateExplosion(game);

		//desenha a cena
		drawScene(&scene,&camera,dudes,numDudes,WIDTH,HEIGHT,crossWidth,game);

		SDL_GL_SwapBuffers();
		controlFramerate(frameControl);

		play = checkVictory(game);
	}

	return play;

}
