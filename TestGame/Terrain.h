#pragma once
#include <noise/noise.h>
#include <iostream>
#include <GL/glew.h>
#include "Shader.h"
#include "Textures.h"
#include <glm/glm.hpp>
#include "Geometry.h"

class Terrain
{
public:
	int xSize;
	int zSize;
	int numTriangles;
	Triangle * triangles;
	Terrain(int xSize, int zSize, noise::module::Perlin * noise);
	~Terrain();
	void Render(float * modelview, float * projection);
private:
	Shader * shader;
	GLuint vbo;
	GLuint texture;
};

