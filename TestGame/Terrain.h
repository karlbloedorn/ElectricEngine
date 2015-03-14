#pragma once
#include <noise/noise.h>
#include <iostream>
#include <GL/glew.h>
#include "Shader.h"
#include "Textures.h"
#include <glm/glm.hpp>

struct Vertex{
	struct Position{
		float x;
		float y;
		float z;
	} position;

	struct TextureCoord{
		float x;
		float y;
	} textureCoord;

	struct Normal{
		float x;
		float y;
		float z;
	} normal;
};

struct Triangle{
	Vertex a;
	Vertex b;
	Vertex c;
};

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

