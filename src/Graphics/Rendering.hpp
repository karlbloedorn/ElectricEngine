#pragma once
#define GLM_FORCE_RADIANS
#include <string>
#include <functional>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_ttf.h>
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/GLU.h>
#else
#  include <GL/gl.h>
#  include <GL/GLU.h>
#endif/*__APPLE__*/
#include "../Scene/Entity.hpp"
#include "Mesh.hpp"
#include "Grid.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "../Scene/Entity.hpp"
#include "../Scene/StaticProp.hpp"

using namespace std;

class Rendering {

private:
	map<int, Mesh *> meshMap;
	int windowWidth;
	int windowHeight;
	TTF_Font * roboto50;
	TTF_Font * roboto200;
	Skybox * skybox;
	Grid * grid;
	Shader * entityShader;
	Shader * terrainShader;
public:
	Rendering();
	bool Initialize(string & error, int windowHeight, int windowWidth, bool fullScreen, string assetPath);
	void RenderText(string text, TTF_Font * font, SDL_Color color, int x, int y);
	bool AddEntity(Entity * entity, string assetPath);
	void ShowLoading(float progress);
	void RenderGame(map<int, vector<int>> * renderMap, map<int, StaticProp *> * staticPropMap);
	SDL_Window * window;
	float skyboxRotation = 0;
	glm::vec3 cameraPosition;
	glm::mat4x4 lookAt;
	bool wireframe = false;
};