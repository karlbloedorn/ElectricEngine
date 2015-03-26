#pragma once
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <map>

#define degreesToRadians(x) x*(3.141592f/180.0f)

using namespace std;

class Controls {

private:
	unsigned int lastFrameTime;

public:
	Controls();
	bool moving = false;
	bool jump = false;
	bool quit = false;
	bool wireframe = false;
	glm::vec3 normalizedWalkingVector;
	glm::vec3 cameraRotation = glm::vec3(3.25501370, -0.114001535, 0);
	float GetInput();
};