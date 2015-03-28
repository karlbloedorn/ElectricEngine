#include "Scene/Assets.hpp"
#include "Graphics/Rendering.hpp"
#include "Controls.hpp"

Controls * controls;
Assets * assets;
Rendering * rendering;
void gameloop();
map<int, vector<int>> renderMap;
map<int, Entity *> entityMap;
map<int, StaticProp *> staticPropMap;
glm::vec3 playerPosition = glm::vec3(440, 50, 420);

float playerSpeed = 20.0f;


int main(int argc, char * argv[]){
	string assetPath = "../../assets/";
	string initError;
	SDL_Init(SDL_INIT_EVERYTHING);
	rendering = new Rendering();
	controls = new Controls();

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
			entityMap[curEntity.entityID] = &curEntity;
			rendering->AddEntity(&curEntity, assetPath);
		}
		progressFunction(0.05f + ((i + 1) / (numEntities * 1.0f)) * 0.95f);
	}

	StaticProp * test2 = new StaticProp(2, glm::vec3(450,55, 410));
	test2->propID = 1;
	staticPropMap[1] = test2;

	StaticProp * test3 = new StaticProp(1, glm::vec3(450, 47.5, 405));
	test3->propID = 2;
	staticPropMap[2] = test3;


	renderMap[2] = vector<int>();
	renderMap[2].push_back(1);
	renderMap[1].push_back(2);


	//renderMap[4].push_back(3);

	//renderMap[5].push_back(4);

	int count = 30;
	for (int i = 0; i <count; i++){
		for (int j = 0; j < count; j++){
			//StaticProp * test = new StaticProp(2, glm::vec3(i * 5, 0, j * 5));
			//test->propID = i*count + j;
			//staticPropMap[i *count + j] = test;
			//renderMap[2].push_back(i * count + j);
		}
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
		float delta = controls->GetInput();
		if (controls->quit){
			return;
		}
		rendering->wireframe = controls->wireframe;
		auto moveVector = glm::vec3(0.0, 0.0, 0.0);

		moveVector.x = controls->normalizedWalkingVector.x;
		moveVector.z = controls->normalizedWalkingVector.z;
		moveVector.y = controls->normalizedWalkingVector.y; // += 0 * delta;
		auto xRotate = glm::rotate(controls->cameraRotation.x, glm::vec3(0, -1, 0));
		auto movement = glm::vec4(moveVector * delta * playerSpeed, 0) * xRotate;
		auto proposed_location =  playerPosition + glm::vec3(movement);
		bool xOkay = true;
		bool yOkay = true;
		bool zOkay = true;
		playerPosition += glm::vec3(xOkay ? movement.x : 0, yOkay ? movement.y : 0, zOkay ? movement.z : 0);

		auto xCamRotate = glm::rotate(controls->cameraRotation.x, glm::vec3(0, 1, 0));
		auto yCamRotate = glm::rotate(controls->cameraRotation.y, glm::vec3(1, 0, 0));
		auto bothRotate = xCamRotate * yCamRotate;
		auto lookVector = bothRotate * glm::vec4(0, 0, 1, 0);
		auto cameraPosition = playerPosition + glm::vec3(0, 12, 0);

		auto lookat = glm::lookAt(cameraPosition, glm::vec3(lookVector) + cameraPosition, glm::vec3(0.0, 1.0, 0.0));
		rendering->lookAt = lookat;
		rendering->cameraPosition = cameraPosition;
		rendering->skyboxRotation += delta*0.006;

		rendering->RenderGame(&renderMap, &staticPropMap);


	}
}