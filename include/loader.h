#ifndef LOADER_H
#define LOADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

/**
  * converte uma SDL_Surface para uma textura do OpenGL
  */
GLuint surface2texture(SDL_Surface *surface);

/**
  * carrega uma imagem em uma textura
  */
GLuint loadTexture(const char *fname);

#endif // LOADER_H
