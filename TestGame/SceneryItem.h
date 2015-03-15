#pragma once
#include "Textures.h"
#include "Shader.h"
#include <string>
#include <glm/glm.hpp>
#include "Geometry.h"

class SceneryItem
{
public:
	SceneryItem();
	~SceneryItem();
	Triangle * triangles;
	bool LoadFromObj(string basePath, string filePath);
	int numTriangles;
	void Render(float * modelview, float * projection);
private:
	Shader * shader;
	GLuint vbo;
	GLuint texture;
};

