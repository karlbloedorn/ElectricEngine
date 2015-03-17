#pragma once
#include <noise/noise.h>
#include <iostream>
#include <GL/glew.h>
#include "Shader.h"
#include "Textures.h"
#include <glm/glm.hpp>
#include "Geometry.h"

class Chunk
{
	public:
		static const int ChunkSize;
		Chunk(int x, int z);
		~Chunk();
		int x, z;
		void Generate(noise::module::Perlin * noiseGenerator);
		void LoadVBO();
		void UnloadVBO();
		void Render();
	private:
		int numTriangles;
		Triangle * triangles;
		GLuint vbo;
};

