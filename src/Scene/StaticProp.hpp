#pragma once
#include <string>
#include <glm/glm.hpp>

class StaticProp {
public:
	StaticProp();
	StaticProp(int entityID, glm::vec3 position);

	int entityID; 
	int propID;
	bool hidden = false;
	glm::vec3 position;

private:
	StaticProp(const StaticProp&);
	StaticProp& operator=(const StaticProp&);
};

