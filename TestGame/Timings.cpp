#include "Timings.h"

Timings::Timings()
{
	old_time = SDL_GetTicks();
}

float Timings::FrameUpdate()
{
	auto current_time = SDL_GetTicks();
	auto delta = (current_time - old_time) / 1000.0f;
	old_time = current_time;
	return delta;
}

Timings::~Timings()
{


}
