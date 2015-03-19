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
	bool CanAudioUpdate();
private:
	unsigned int lastFrameTime;
	unsigned int lastFootstepTime;
	unsigned int lastNetworkSync;
	unsigned int lastAudioUpdate;
};

