#pragma once
#include <yse/yse.hpp>
#include <glm/glm.hpp>
#include "Timings.h"

class Audio
{
private: 
	Timings * timings;
public:
	Audio(Timings * timings);
	~Audio();
	void Update(const glm::vec3 & cameraPosition, const glm::vec4 & cameraRotation);
	YSE::sound MusicOrchestral;
	YSE::reverb GameReverb;
};

