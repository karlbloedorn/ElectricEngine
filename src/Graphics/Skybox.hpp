#pragma once
#include <unordered_map>
#include "OpenGL33.hpp"
#include "Texture.hpp"
#include "Geometry.hpp"
#include "Shader.hpp"

using namespace gl33core;

class Skybox
{
private:
	Triangle * triangles;
	int numTriangles;
	GLuint textures[6];
	GLuint vbo;
public:
	Skybox();
	bool Load(std::string assetsPath);
	void Render(Shader * shader);

};