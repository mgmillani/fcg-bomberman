#include <stdlib.h>

#include <SDL/SDL.h>

#include "play.h"

#include "frameControl.h"
#include "draw.h"
#include "init.h"
#include "physics.h"
#include "movement.h"

#define WIDTH 800
#define HEIGHT 600
#define DEFAULT_FPS 60

char should_rotate = 1;

const char gFloorFile[] = "textures/floor.png";
const char gWallFile[] = "textures/wall.jpg";
const char gSkyFile[] = "textures/bluesky.jpg";
const char gDudeFile[] = "textures/stick.png";

double gravity[3] = {0,-0.005,0};
double crossWidth = 2;

void play()
{
    int numDudes = 1;
	//personagens
	GLuint dudeTex = loadTexture(gDudeFile);
	t_character *dudes = malloc(numDudes*sizeof(t_character));
	initCharacter(dudes,dudeTex);
	t_camera camera;
	initCamera(&camera,dudes);

	//as paredes
	t_scene scene;
	GLuint floor = loadTexture(gFloorFile);
	GLuint wall = loadTexture(gWallFile);
	GLuint sky = loadTexture(gSkyFile);
	initScene(&scene,floor,wall,sky);


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
		moveDudes(dudes+1,numDudes-1);
		simulatePhysics(dudes,numDudes,&scene,gravity);
		drawScene(&scene,&camera,dudes,numDudes,WIDTH,HEIGHT,crossWidth);
		SDL_GL_SwapBuffers();
		//draw_screen();
		controlFramerate(frameControl);
	}

}
