#pragma once
#include <unordered_map>
#include <GL/glew.h>
#include "Textures.h"

class Skybox
{
private: 
	GLuint textures[6];
public:
	Skybox();
	~Skybox();
	void Render();
};

