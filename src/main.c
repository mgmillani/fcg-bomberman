#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "play.h"
#include "loader.h"
#include "gameGrid.h"
#include "list.h"

#include "definitions.h"
#include "debug.h"

#define WIDTH 800
#define HEIGHT 600
#define BPP 32

const char gProgName[] = "Bombot";

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
	glEnable(GL_CULL_FACE);
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

int main(int argc, char **argv)
{
	if(init(WIDTH,HEIGHT,BPP,SDL_OPENGL | SDL_SWSURFACE)!=0)
	{
		ERR("Failed to initialize SDL.");
		return -1;
	}

	t_gameData data;
	loadMap("config","Empty",&data);
	listInit(&(data.bombs));

	play(&data);

	SDL_Quit();
	return 0;

}
