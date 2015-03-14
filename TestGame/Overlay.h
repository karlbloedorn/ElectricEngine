#pragma once

#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>

class Overlay
{
private:
	TTF_Font * font;
public:
	Overlay();
	~Overlay();
	void Render(std::string text, SDL_Color color, int x, int y);
};

