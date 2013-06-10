
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "loader.h"

/**
  * converte uma SDL_Surface para uma textura do OpenGL
  */
GLuint surface2texture(SDL_Surface *surface)
{
	GLuint texture,width,height;
	if(surface->w == 0 || surface->h ==0)
		return 0;
	width = surface->w;
	height = surface->h;

	GLint ncolors,internalFormat;
	GLenum textureFormat;

	//determina o numero de canais da surface
	ncolors = surface->format->BytesPerPixel;

	switch(ncolors)
	{
		//com alpha channel
		case 4:
			internalFormat = GL_RGBA;
			if (surface->format->Rmask == 0xff)
				textureFormat = GL_RGBA;
			else
				textureFormat = GL_BGRA;
			break;

		//sem alpha channel
		case 3:
			internalFormat = GL_RGB;
			if (surface->format->Rmask == 0xff)
				textureFormat = GL_RGB;
			else
				textureFormat = GL_BGR;
			break;
		default:
			fprintf(stderr,"Wrong number of colors :%d\n",ncolors);
			return 0;
	}

	//gera o handle da textura
	glGenTextures(1,&texture);

	//faz o binding da textura criada
	glBindTexture(GL_TEXTURE_2D,texture);

	//determina as propriedades de stretching da textura
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	//usa a surface para criar a textura
	glTexImage2D(GL_TEXTURE_2D,0,internalFormat,width,height,0,textureFormat,
								GL_UNSIGNED_BYTE,surface->pixels);

	SDL_FreeSurface(surface);

	return texture;
}

/**
  * carrega uma imagem em uma textura
  */
GLuint loadTexture(const char *fname)
{
	SDL_Surface *img = IMG_Load(fname);
	return surface2texture(img);
}
