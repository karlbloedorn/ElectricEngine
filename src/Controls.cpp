#include "Controls.hpp"

Controls::Controls(){
	lastFrameTime = SDL_GetTicks();
}

float Controls::GetInput(){
	auto currentTime = SDL_GetTicks();
	auto delta = (currentTime - lastFrameTime) / 1000.0f;
	lastFrameTime = currentTime;

	moving = false;
	quit = false;
	jump = false;

	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)){
		switch (evnt.type){
		case SDL_QUIT:
			quit = true;
			return 0;
		case SDL_KEYUP:
			switch (evnt.key.keysym.sym){
			case SDLK_TAB:
				break;
			case SDLK_LSHIFT:
				this->wireframe = !this->wireframe;
				break;
			}
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				return 0;
			}
			break;
		case SDL_MOUSEMOTION:
			this->cameraRotation.y += 0.003f  * evnt.motion.yrel;
			this->cameraRotation.x -= 0.003f * evnt.motion.xrel;
			if (this->cameraRotation.y < degreesToRadians(-75)){
				this->cameraRotation.y = degreesToRadians(-75);
			}
			if (this->cameraRotation.y> degreesToRadians(89)){
				this->cameraRotation.y = degreesToRadians(89);
			}
			break;
		}

		
	}
	auto keystates = SDL_GetKeyboardState(NULL);

	auto moveMap2 = map<SDL_Scancode, glm::vec3> {
		{ SDL_SCANCODE_W, glm::vec3(0.0, 0.0, 1.0) },
		{ SDL_SCANCODE_A, glm::vec3(1.0, 0.0, 0.0) },
		{ SDL_SCANCODE_S, glm::vec3(0.0, 0.0, -1.0) },
		{ SDL_SCANCODE_D, glm::vec3(-1.0, 0.0, 0.0) },
		{ SDL_SCANCODE_Q, glm::vec3(0.0,1.0, 0.0) },
		{ SDL_SCANCODE_Z, glm::vec3(0.0, -1.0, 0.0) }
	};

	auto walkingVector = glm::vec3(0, 0, 0);
	for (auto var : moveMap2)
	{
		if (keystates[var.first]){
			walkingVector += var.second;
		}
	}
	normalizedWalkingVector = glm::vec3(0, 0, 0);
	if (glm::length(walkingVector) > 0){
		normalizedWalkingVector = glm::normalize(walkingVector);
		moving = true;
	}
	return delta;
}

