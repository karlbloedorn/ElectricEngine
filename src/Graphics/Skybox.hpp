#pragma once
#include <unordered_map>
#include <GL/glew.h>
#include "Texture.hpp"

class Skybox
{
private:
	GLuint textures[6];
public:
	Skybox(std::string assetsPath);
	void Render();
};