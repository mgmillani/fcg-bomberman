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
const char gVictory[] = "textures/screens/victory.jpg";
const char gDefeat[] = "textures/screens/defeat.jpg";

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

	GLfloat density = 0.3;
	GLfloat fogColor[4] = {0.2,0.15,0.1,1};
	glEnable(GL_FOG);
	glFogf (GL_FOG_START, 5);
	glFogf (GL_FOG_END, 15.1);
	glFogi(GL_FOG_MODE,GL_LINEAR);
	glFogfv(GL_FOG_COLOR,fogColor);
	glFogf(GL_FOG_DENSITY,density);

	glClearColor(0.2f,0.15f,0.1f,0.0f);
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

void gameOver(GLuint screen)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	gluOrtho2D(1,-1,-1,1);
	glColor3f(1,1,1);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,screen);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0.0f);   // coords for the texture
	glVertex2f(1,1);

	glTexCoord2f(0, 1.0);   // coords for the texture
	glVertex2f(1,-1);

	glTexCoord2f(1.0, 1.0);   // coords for the texture
	glVertex2f(-1,-1);

	glTexCoord2f(1.0, 0);   // coords for the texture
	glVertex2f(-1,1);


	glEnd();

	SDL_GL_SwapBuffers();

	SDL_Delay(3000);
}

int main(int argc, char **argv)
{
	if(init(WIDTH,HEIGHT,BPP,SDL_OPENGL | SDL_SWSURFACE)!=0)
	{
		ERR("Failed to initialize SDL.");
		return -1;
	}

	t_gameData data;
	loadMap("config","Factory",&data);
	listInit(&(data.bombs));
	listInit(&(data.explosions));

	GLuint victory = loadTexture(gVictory);
	GLuint defeat = loadTexture(gDefeat);

	char status = play(&data);


	if(status == 1)
	{
		gameOver(victory);
	}
	else
	{
		gameOver(defeat);
	}

	SDL_Quit();
	return 0;

}
