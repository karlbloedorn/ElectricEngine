#pragma once
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <string>

using namespace std;

class Textures
{
public:
	Textures();
	~Textures();
	static void SetupTexture(GLuint texture, string filePath, bool allowWrap);
};

