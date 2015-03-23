#include "Scene/Assets.hpp"
#include "Graphics/Rendering.hpp"

Assets * assets;
Rendering * rendering;
void gameloop();

int main(int argc, char * argv[]){
	string assetPath = "../../assets/";
	string initError;
	SDL_Init(SDL_INIT_EVERYTHING);
	rendering = new Rendering();
	bool initRenderingSuccess = rendering->Initialize(initError, 1080, 1920, false, assetPath);
	bool initSuccess = initRenderingSuccess;
	if (!initSuccess){
		cout << initError << endl;
		exit(1);
	}

	// show menu
	std::function<void(float)> progressFunction = [](float percent) {
		cout << "Loading " << percent*100 << " %" << endl;
		rendering->ShowLoading(percent * 100);
	};

	// loading
	progressFunction(0.01f);
	assets = new Assets(assetPath);
	assets->LoadEntities();
	progressFunction(0.05f);

	int numEntities = assets->entities->size();
	for (int i = 0; i < numEntities; i++){
		Entity & curEntity = assets->entities->at(i);
		if (curEntity.render == true){
			rendering->AddEntity(&curEntity, assetPath);
		}
		progressFunction(0.05f + ((i + 1) / (numEntities * 1.0f)) * 0.95f);
	}

	// load entities models and textures
	progressFunction(1.0f);

	gameloop(); //todo eventually show menu and put in a loop with an exit button.

	return 0;
}
void gameloop(){
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_SetRelativeMouseMode(SDL_bool::SDL_TRUE);

	while (1){
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)){
			switch (evnt.type){
			case SDL_QUIT:
				return;
			case SDL_KEYUP:
				switch (evnt.key.keysym.sym){
				case SDLK_TAB:
					break;
				case SDLK_LSHIFT:
					rendering->wireframe = !rendering->wireframe;
					break;
				}
			case SDL_KEYDOWN:

				switch (evnt.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return;
				}
				break;
			case SDL_MOUSEMOTION:
				rendering->cameraRotation.y += 0.003f  * evnt.motion.yrel;
				rendering->cameraRotation.x -= 0.003f * evnt.motion.xrel;
				if (rendering->cameraRotation.y < degreesToRadians(-75)){
					rendering->cameraRotation.y = degreesToRadians(-75);
				}
				if (rendering->cameraRotation.y> degreesToRadians(89)){
					rendering->cameraRotation.y = degreesToRadians(89);
				}
				break;
			}
		}

		rendering->RenderGame();
	}
}