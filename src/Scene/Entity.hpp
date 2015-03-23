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

//	Position * position;

private:
	Entity(const Entity&);
	Entity& operator=(const Entity&);
};

