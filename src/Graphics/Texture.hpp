#pragma once
#include <SDL_image.h>
#include <GL/glew.h>
#include <string>
#include <iostream>
using namespace std;

class Texture {

public:
	Texture();
	static void SetupTexture(GLuint texture, string filePath, bool allowWrap);
};