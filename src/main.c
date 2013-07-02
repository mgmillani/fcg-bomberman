#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "frameControl.h"
#include "loader.h"
#include "physics.h"
#include "draw.h"
#include "movement.h"
#include "init.h"
#include "gameGrid.h"

#include "definitions.h"
#include "debug.h"

#define WIDTH 800
#define HEIGHT 600
#define BPP 32
#define DEFAULT_FPS 60

char should_rotate = 1;

const char gProgName[] = "Bombot";
const char gFloorFile[] = "textures/floor.png";
const char gWallFile[] = "textures/wall.jpg";
const char gSkyFile[] = "textures/bluesky.jpg";
const char gDudeFile[] = "textures/stick.png";

//inits stuff and the screen
int init(int width,int height,int bpp,int options)
{
	SDL_putenv("SDL_VIDEO_CENTERED=center");
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		return -1;

	SDL_WM_SetCaption(gProgName,NULL);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	SDL_SetVideoMode(width,height,bpp,options);

	//tranca o mouse na tela
	//SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(SDL_DISABLE);

	//opengl stuff
	glEnable(GL_LINE_SMOOTH);

	glClearColor(0.05f,0.25f,0.70f,0.0f);
	glViewport(0,0,width,height);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double ratio = (double)width/height;
	gluPerspective( 60.0, ratio, 0.1, 1024.0 );

	//glOrtho(0.0f,width,height,0.0f,0.0f,1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//SDL_image
	if(IMG_Init(IMG_INIT_PNG)==0)
		return -1;

	return 0;
}

void loadGrid(const char *fname)
{
	t_gameGrid grid;
	t_colorMap colorMap;
	colorMap.emptyColor[0] = 0;
	colorMap.emptyColor[1] = 0;
	colorMap.emptyColor[2] = 0;

	colorMap.breakableColor[0] = 255;
	colorMap.breakableColor[1] = 0;
	colorMap.breakableColor[2] = 0;

	colorMap.unbreakableColor[0] = 0;
	colorMap.unbreakableColor[1] = 255;
	colorMap.unbreakableColor[2] = 0;
	gameGridLoad(&grid,fname,&colorMap);
	gameGridPrint(&grid);
}

int main(int argc, char **argv)
{
	if(init(WIDTH,HEIGHT,BPP,SDL_OPENGL | SDL_SWSURFACE)!=0)
	{
		ERR("Failed to initialize SDL.");
		return -1;
	}

	loadGrid("maps/factory.png");

	int numDudes = 170;
	//personagens
	GLuint dudeTex = loadTexture(gDudeFile);
	t_character *dudes = malloc(numDudes*sizeof(t_character));
	int i;
	for(i=0 ; i<numDudes ; i++)
		initCharacter(dudes+i,dudeTex);
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

	double gravity[3] = {0,-0.005,0};
	double crossWidth = 2;

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

	return 0;

}
