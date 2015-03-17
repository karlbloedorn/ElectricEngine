#include "Overlay.h"

Overlay::Overlay()
{
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(1);
	}

	font = TTF_OpenFont("assets/roboto.ttf", 26);
	if (!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(1);
	}
}

Overlay::~Overlay()
{

}

void Overlay::Render(std::string text, SDL_Color color, int x, int y)
{
	SDL_Surface * sFont = TTF_RenderText_Blended(font, text.c_str(), color);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sFont->pixels);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0); glVertex2f(x, y);
		glTexCoord2f(1, 0); glVertex2f(x + sFont->w, y);
		glTexCoord2f(1, 1); glVertex2f(x + sFont->w, y + sFont->h);
		glTexCoord2f(0, 1); glVertex2f(x, y + sFont->h);
	}
	glEnd();
	glDeleteTextures(1, &texture);
	SDL_FreeSurface(sFont);
}