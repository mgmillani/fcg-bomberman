#include <stdlib.h>

#include "play.h"


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
