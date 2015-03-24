#pragma once
#include <string>

class Entity {
public:
	int entityID;
	std::string name;
	bool continuousAudio;
	bool render;
	bool collide;
	Entity();
private:
	Entity(const Entity&);
	Entity& operator=(const Entity&);
};

