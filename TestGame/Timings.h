#pragma once
#include <SDL2/SDL.h>

class Timings
{
public:
	Timings();
	~Timings();
	float FrameUpdate();
	bool CanPlayFootstep();
	bool CanSendReceive();
	bool CanNetworkSync();
private:
	unsigned int lastFrameTime;
	unsigned int lastFootstepTime;
	unsigned int lastNetworkSync;
};

