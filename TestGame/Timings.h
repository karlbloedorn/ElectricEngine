#pragma once
#include <SDL2/SDL.h>

class Timings
{
public:
	Timings();
	~Timings();
	float FrameUpdate();
	bool CanPlayFootstep();

private:
	unsigned int lastFrameTime;
	unsigned int lastFootstepTime;
};

