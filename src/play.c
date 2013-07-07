#include <stdlib.h>

#include <SDL/SDL.h>

#include "play.h"

#include "frameControl.h"
#include "draw.h"
#include "init.h"
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

double gravity[3] = {0,-0.005,0};
double crossWidth = 2;

/**
  * laco principal do jogo
  */
void play(t_gameData *game)
{
	int numDudes = 1;
	//personagens
	GLuint dudeTex = loadTexture(gDudeFile);
	t_character *dudes = malloc(numDudes*sizeof(t_character));
	initCharacter(dudes,dudeTex,game->grid);
	t_camera camera;
	initCamera(&camera,dudes);

	//lista para as bombas
	t_list bombs;
	listInit(&bombs);

	//o cenario
	t_scene scene;
	initScene(&scene,game->grid);

	unsigned int fps = DEFAULT_FPS;
	t_frameController frameControl;
	initFrameController(&frameControl,fps);

	char play = 1;
	while(play)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Uint8 *keystate;
		int numKeys;
		frameControl.frameStart = SDL_GetTicks();
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				play = 0;
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
		//moveDudes(dudes+1,numDudes-1);
		simulatePhysics(dudes,numDudes,&scene,gravity);
		//ERR("Char pos: %.2lf %.2lf %.2lf\n",dudes->pos[0],dudes->pos[1],dudes->pos[2]);
		drawScene(&scene,&camera,dudes,numDudes,WIDTH,HEIGHT,crossWidth,game);
		SDL_GL_SwapBuffers();
		//draw_screen();
		controlFramerate(frameControl);
	}

}
