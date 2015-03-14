#pragma once
#include <SDL2/SDL.h>

class Timings
{
public:
	Timings();
	~Timings();
	float FrameUpdate();
private:
	unsigned int old_time;
};

