#pragma once
#include <string>
#include <glbinding/gl/functions33core.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "tiny_obj_loader.hpp"

#include "Shader.hpp"
#include "Texture.hpp"
#include "Geometry.hpp"
#include "../Scene/StaticProp.hpp"

using namespace std;
using namespace gl33core;

class Mesh {
public:
	Mesh();
	Triangle * triangles;
	bool LoadFromObj(string basePath, string filePath, string forceTexture);
	int numTriangles;
	void RenderInstances(Shader * shader, vector<int> renderList, map<int, StaticProp *> * staticPropMap);
	GLuint vbo;
	GLuint texture;
};