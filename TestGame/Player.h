#pragma once
#include <glm/glm.hpp>

class Player
{
public:
	glm::vec3 position;
	glm::vec3 lookDirection;
	Player();
	~Player();
};

