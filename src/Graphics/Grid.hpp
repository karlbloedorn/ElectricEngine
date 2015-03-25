#pragma once
#include <string>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.hpp"
#include "Texture.hpp"
#include "Geometry.hpp"
#include "../Scene/StaticProp.hpp"

using namespace std;

class Grid {
public:
	Grid();
	Triangle2D * triangles;
	bool Load();
	int numTriangles;
	void RenderInstances(Shader * shader);
	GLuint vbo;
	GLuint texture;
};