#include <fmod/fmod.hpp>
#include "../Scene/Entity.hpp"
#include "../Scene/StaticProp.hpp"
#include <fmod/fmod_errors.h>
#include <glm/glm.hpp>

using namespace std;

class Sound {
private:
	FMOD::System *system = NULL;
	FMOD::Sound * birdSound;
	FMOD::Channel * birdChannel = 0;
public:
	Sound();
	void Setup(string assetPath);
	void Update(glm::vec3 position, glm::vec3 look, glm::vec3 up, glm::vec3 birdPosition);
	bool AddEntity(Entity * entity, string assetPath);

};