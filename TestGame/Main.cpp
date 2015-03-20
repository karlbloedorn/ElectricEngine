#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <map>
#include <iostream>
#include "Timings.h"
#include "Overlay.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Shader.h"
#include "SceneryItem.h"
#include "Audio.h"

#define degreesToRadians(x) x*(3.141592f/180.0f)
#define GLM_FORCE_RADIANS
using namespace std;

int windowHeight = 1080;
int windowWidth = 1980;
float cameraSpeed = 15.0f;
glm::vec3 speedVector(0.0, 0.0, 0.0);
float skyboxRotation = 0;
glm::vec3 playerPosition = glm::vec3(50, 10.3822203, 50);
glm::vec3 cameraRotation = glm::vec3(3.25501370, -0.114001535, 0);
bool wireframe = false;

enum class GameState { PLAY, EXIT };
GameState curGameState = GameState::PLAY;
SDL_Window * window;

Audio * audio;
Timings * timings;
Overlay * overlay;
Skybox * skybox;
Terrain * terrain;
Shader * terrainShader;
noise::module::Perlin * perlin;

SceneryItem * rocks;
SceneryItem * weeds;

vector<glm::vec3> rockPositions;
vector<glm::vec3> weedPositions;


void processinput();
void gameloop();
void drawgame(const glm::vec3 & cameraPosition, const glm::vec4 & lookVector);
bool SubsystemInitialization(string & error);

float groundLevel(float x, float z){
	return 55 + perlin->GetValue(x / 505.5, 0, (z / 505.5)) * 15;;
}

int main(int argc, char ** argv){
	string initError;
	bool initSuccess = SubsystemInitialization(initError);
	if (!initSuccess){
		cout << initError << endl;
		exit(1);
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_SetRelativeMouseMode(SDL_bool::SDL_TRUE);

	glClearColor(0.1, 0.1, 0.1, 1.0);
	glEnable(GL_TEXTURE_2D);

	perlin = new noise::module::Perlin();
	perlin->SetSeed(999); //TODO this should come from server if connected.
	perlin->SetOctaveCount(8);
	perlin->SetLacunarity(2);
	perlin->SetFrequency(2);
	timings = new Timings();

	audio = new Audio(timings);
	terrain = new Terrain(perlin);
	overlay = new Overlay();
	skybox = new Skybox();

	rocks = new SceneryItem();
	weeds = new SceneryItem();

	//rocks->LoadFromObj("assets/meshes/rock/", "rock01.obj", "rock_diffuse.png");
	rocks->LoadFromObj("assets/meshes/derpsphere/", "derp.obj", "snow_gum_mod.png");
	weeds->LoadFromObj("assets/meshes/tinyweed4/", "tiny_weed_04.obj", "diffuse.tga");

	/*
	for (int i = 0; i < 20;i++){
		float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200 * 0.9))) - 100;
		float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200 * 0.9))) - 100; 

		float level = groundLevel(r1,r2);
		rockPositions.push_back(glm::vec3(r1, level+1.1, r2));		
	}

	*/
	rockPositions.push_back(glm::vec3(0, 60, 0));

	for (int i = 0; i < 10; i++){
		float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200 * 0.9))) - 100;
		float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200 * 0.9))) - 100;
		float level = groundLevel(r1, r2);
		weedPositions.push_back(glm::vec3(r1, level+0.0, r2));
	}

	gameloop();

	delete audio;

	return 0;
}

void gameloop(){

	while (curGameState != GameState::EXIT){

		float delta = timings->FrameUpdate() * 2.0;
		skyboxRotation += delta*0.006;
		SDL_Event evnt;

		while (SDL_PollEvent(&evnt)){
			switch (evnt.type){
			case SDL_QUIT:
				curGameState = GameState::EXIT;
				break;
			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym)
				{
				case SDLK_LSHIFT:
					wireframe = !wireframe;
					break;
				case SDLK_ESCAPE:
					curGameState = GameState::EXIT;
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				cameraRotation.y += 0.003f  * evnt.motion.yrel;
				cameraRotation.x -= 0.003f * evnt.motion.xrel;
				if (cameraRotation.y < degreesToRadians(-75)){
					cameraRotation.y = degreesToRadians(-75);
				}
				if (cameraRotation.y> degreesToRadians(89)){
					cameraRotation.y = degreesToRadians(89);
				}
				break;
			}
		}

		auto moveVector = glm::vec3(0.0, 0.0, 0.0);
		auto keystates = SDL_GetKeyboardState(NULL);

		auto moveMap2 = map<SDL_Scancode, glm::vec3> {
				{ SDL_SCANCODE_W, glm::vec3(0.0, 0.0, 1.0) },
				{ SDL_SCANCODE_A, glm::vec3(1.0, 0.0, 0.0) },
				{ SDL_SCANCODE_S, glm::vec3(0.0, 0.0, -1.0) },
				{ SDL_SCANCODE_D, glm::vec3(-1.0, 0.0, 0.0) }
		};
		float groundLevelAtPos = groundLevel(playerPosition.x, playerPosition.z);
		auto walkingVector = glm::vec3(0, 0, 0);
		for (auto var : moveMap2)
		{
			if (keystates[var.first]){
				walkingVector += var.second;
			}
		}

		bool moving = false;
		glm::vec3 normalizedWalkingVector = glm::vec3(0, 0, 0);
		if (glm::length(walkingVector) > 0){
			normalizedWalkingVector = glm::normalize(walkingVector);
			moving = true;
		}

		if ((playerPosition.y - groundLevelAtPos) < 0.1){
			if (keystates[SDL_SCANCODE_SPACE]){
				// TODO jump event to sound subsystem
				speedVector.y = 30; // 150.0f; // fun // regular 50.0f;
			}
		}	else {
			speedVector.y -= 28.8f * delta;
		}
		moveVector.x = normalizedWalkingVector.x;
		moveVector.z = normalizedWalkingVector.z;
		moveVector.y += speedVector.y * delta;

		auto xRotate = glm::rotate(cameraRotation.x, glm::vec3(0, -1, 0));
		auto movement = glm::vec4(moveVector * delta * cameraSpeed, 0) * xRotate;
		auto proposed_location = playerPosition + glm::vec3( movement);

		bool xOkay = true;
		bool yOkay = true;
		bool zOkay = true;
				
		playerPosition += glm::vec3(xOkay ? movement.x : 0, yOkay ? movement.y : 0, zOkay ? movement.z : 0);
		if ((proposed_location.y - groundLevelAtPos) < 0.1){
			playerPosition.y = groundLevelAtPos;
			speedVector.y = 0;
			if (moving && timings->CanPlayFootstep()){
				// TODO footstep event to sound subsystem
			}
		}

		// Calculate where the camera is pointed
		auto xCamRotate = glm::rotate(cameraRotation.x, glm::vec3(0, 1, 0));
		auto yCamRotate = glm::rotate(cameraRotation.y, glm::vec3(1, 0, 0));
		auto bothRotate = xCamRotate * yCamRotate;
		auto lookVector = bothRotate * glm::vec4(0, 0, 1, 0);
		auto cameraPosition = playerPosition + glm::vec3(0, 4, 0);


		rockPositions[0].x = sin(skyboxRotation*50) * 60;
		rockPositions[0].z = cos(skyboxRotation * 50) * 60;;// cos(delta / 100.0) * 50;

		rockPositions[0].y = groundLevel(rockPositions[0].x, rockPositions[0].z) + 3;

		audio->MusicOrchestral.setPosition(YSE::Vec(rockPositions[0].x, rockPositions[0].y, rockPositions[0].z));

		audio->Update( cameraPosition, lookVector);



		drawgame( cameraPosition, lookVector);
	}
}

void drawgame(const glm::vec3 & cameraPosition, const glm::vec4 & lookVector){
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto lookat = glm::lookAt(cameraPosition, glm::vec3(lookVector) + cameraPosition, glm::vec3(0.0, 1.0, 0.0));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	auto perspective = glm::perspectiveFov<float>(1.27, windowWidth, windowHeight, 0.1f, 5000.0f);
	glLoadMatrixf(glm::value_ptr(perspective));
	glMatrixMode(GL_MODELVIEW);
	auto skyboxPosition = glm::translate(playerPosition* glm::vec3(1, 1, 1));
	auto combinedSkyboxMat = lookat*skyboxPosition;
	auto rotate = glm::rotate(combinedSkyboxMat, skyboxRotation, glm::vec3(0, 1, 0));
	glLoadMatrixf(glm::value_ptr(rotate));

	skybox->Render();

	if (wireframe){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	terrain->Render(glm::value_ptr(lookat), glm::value_ptr(perspective));




	for (auto rockPosition : rockPositions){
		auto movedRock = glm::translate(lookat, rockPosition);
		rocks->Render(glm::value_ptr(movedRock), glm::value_ptr(perspective));
	}
	for (auto weedPosition : weedPositions){
		auto movedWeed = glm::translate(lookat, weedPosition);
		auto scaledWeed = glm::scale(movedWeed, glm::vec3(0.05, 0.05, 0.05));
		weeds->Render(glm::value_ptr(scaledWeed), glm::value_ptr(perspective));
	}

	// Setup for orthorgraphic UI overlay.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, windowWidth, windowHeight, 0, -10, 10);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::stringstream stringstream;
	stringstream.precision(2);
	stringstream << "x: " << fixed << playerPosition.x << " y: " << fixed << playerPosition.y << " z: " << fixed << playerPosition.z;
	overlay->Render(stringstream.str(), SDL_Color{ 255, 255, 255 }, 10, 10);
	overlay->Render("Hello World", SDL_Color{ 255,255, 25 }, 10, 40);
	
	SDL_GL_SwapWindow(window);

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL error: " << err << endl;
	}
}

bool SubsystemInitialization(string & error){

	SDL_Init(SDL_INIT_EVERYTHING);
	//if (SDLNet_Init() < 0)
	//{
	//	error = string("Failed to start network subsystem: ").append(SDLNet_GetError());
	//	return false;
	//}
	window = SDL_CreateWindow("Test Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL); // | SDL_WINDOW_FULLSCREEN);
	if (window == nullptr){
		error = string("Failed to start windowing subsystem");
		return false;
	}

	auto glcontext = SDL_GL_CreateContext(window);
	if (glcontext == nullptr){
		error = string("Failed to start graphics subsystem");
		return false;
	}
	auto glewinit = glewInit();
	if (glewinit != GLEW_OK){
		error = string("Failed to start graphics subsystem");
		return false;
	}

	return true;
}