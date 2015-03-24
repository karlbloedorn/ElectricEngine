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
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include "../Scene/Entity.hpp"

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "../Scene/StaticProp.hpp"

#define degreesToRadians(x) x*(3.141592f/180.0f)

using namespace std;


class Rendering {

private:
	map<int, Mesh *> meshMap;
	int windowWidth;
	int windowHeight;
	TTF_Font * roboto50;
	TTF_Font * roboto200;
	Skybox * skybox;
	Shader * entityShader;

public:
	Rendering();
	bool Initialize(string & error, int windowHeight, int windowWidth, bool fullScreen, string assetPath);
	void RenderText(string text, TTF_Font * font, SDL_Color color, int x, int y);
	bool AddEntity(Entity * entity, string assetPath);
	void ShowLoading(float progress);
	void RenderGame(map<int, vector<int>> * renderMap, map<int, StaticProp *> * staticPropMap);
	SDL_Window * window;
	glm::vec3 playerPosition = glm::vec3(5, 0, 5);
	glm::vec3 cameraRotation = glm::vec3(3.25501370, -0.114001535, 0);
	bool wireframe = false;
};