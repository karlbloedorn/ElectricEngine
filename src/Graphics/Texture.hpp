#pragma once
#include <SDL_image.h>
#include "OpenGL33.hpp"
#include <string>
#include <iostream>
using namespace std;
using namespace gl33core;
class Texture {

public:
	Texture();
	static void SetupTexture(GLuint texture, string filePath, bool allowWrap, bool filtering, uint8_t ** output, int * outHeight, int * outWidth);
};