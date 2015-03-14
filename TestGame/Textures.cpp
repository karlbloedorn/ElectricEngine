#include "Textures.h"


Textures::Textures()
{
}


Textures::~Textures()
{
}

void Textures::SetupTexture(GLuint texture, string filePath, bool allowWrap){
	SDL_Surface * surface = IMG_Load(filePath.c_str());
	if (!surface) {
		printf("IMG_Load: %s\n", IMG_GetError());
		// handle error
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (!allowWrap){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, 3, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(surface);
}
