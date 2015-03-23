#pragma once
#include "Geometry.hpp"
#include <string>
#include "Shader.hpp"
#include <GL/glew.h>
#include "Texture.hpp"
#include "tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace std;

class Mesh {
public:
	Mesh();
	Triangle * triangles;
	bool LoadFromObj(string basePath, string filePath, string forceTexture);
	int numTriangles;
	void RenderInstances(Shader * shader);
	GLuint vbo;
	GLuint texture;
};