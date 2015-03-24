#include "StaticProp.hpp"

StaticProp::StaticProp(){}

StaticProp::StaticProp(int entityID, glm::vec3 position){
	this->entityID = entityID;
	this->position = position;
}