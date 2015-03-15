#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glew.h>
#define degreesToRadians(x) x*(3.141592f/180.0f)
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <map>

#include "Timings.h"
#include "Overlay.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Shader.h"
#include "SceneryItem.h"

using namespace std;

int windowHeight = 900;
int windowWidth = 1800;
float cameraSpeed = 15.0f;
glm::vec3 speedVector(0.0, 0.0, 0.0);
float skyboxRotation = 0;
glm::vec3 playerPosition = glm::vec3(205, 14,205);
glm::vec3 cameraRotation = glm::vec3(0, 0, 0);
bool wireframe = false;

enum class GameState { PLAY, EXIT };
GameState curGameState = GameState::PLAY;
SDL_Window * window;
Mix_Chunk * jumpSound;
Mix_Chunk * landSound;
Mix_Chunk * footstepSound;

Timings * timings;
Overlay * overlay;
Skybox * skybox;
Terrain * terrain;
Shader * terrainShader;
noise::module::Perlin * perlin;

SceneryItem * testTree;

void processinput();
void gameloop();
void drawgame();

int main(int argc, char ** argv){
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Test Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);// | SDL_WINDOW_FULLSCREEN);
	if (window == nullptr){
		printf("SDL_CreateWindow failed");
		exit(1);
	}
	auto glcontext = SDL_GL_CreateContext(window);
	if (glcontext == nullptr){
		printf("SDL_GL context could not be created");
		exit(1);
	}
	auto glewinit = glewInit();
	if (glewinit != GLEW_OK){
		printf("glewInit context could not be created");
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_SetRelativeMouseMode(SDL_bool::SDL_TRUE);


	glClearColor(0.1,0.1,0.1,1.0);

	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG){
		printf("failed to init mp3");
		exit(1);
	}

	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) == -1){
		printf("Mix_OpenAudio %s\n", Mix_GetError());
		exit(1);
	}

	jumpSound = Mix_LoadWAV("assets/sounds/bodyimpact_jack_01.wav");
	if (!jumpSound) {
		printf("Mix_LoadWAV %s\n", Mix_GetError());
		exit(1);
	}

	landSound = Mix_LoadWAV("assets/sounds/land.wav");
	if (!landSound) {
		printf("Mix_LoadWAV %s\n", Mix_GetError());
		exit(1);
	}
	footstepSound = Mix_LoadWAV("assets/sounds/Fantozzi-SandR1.ogg");
	footstepSound->volume = 5;
	if (!footstepSound) {
		printf("Mix_LoadWAV %s\n", Mix_GetError());
		exit(1);
	}

	/*
	auto music = Mix_LoadMUS("assets/TownTheme.mp3");
	if (!music) {
		printf("Mix_LoadMUS %s\n", Mix_GetError());
		exit(1);
	}

*/

	//Mix_PlayMusic(music, 1);

	perlin =  new noise::module::Perlin();
	perlin->SetOctaveCount(2.2);
	perlin->SetFrequency(0.25);
	terrain = new Terrain(400,400, perlin);
	timings = new Timings();
	overlay = new Overlay();
	skybox = new Skybox();


	testTree = new SceneryItem();
	testTree->LoadFromObj("assets/meshes/palm_tree_lowpoly.obj");

	gameloop();
	return 0;
}

void gameloop(){

	while (curGameState != GameState::EXIT){

		float delta = timings->FrameUpdate() * 2.0;
		skyboxRotation += delta*0.005;
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
		float groundlevel = 7 + perlin->GetValue(playerPosition.x / 25.5, 0, (playerPosition.z / 25.5)) * 15;


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
		if ((playerPosition.y - groundlevel) < 0.1){
			if (keystates[SDL_SCANCODE_SPACE]){
				Mix_PlayChannel(-1, jumpSound, 0);
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
			if ( (proposed_location.y - groundlevel) < 0.1){
				playerPosition.y = groundlevel;
				speedVector.y = 0;

				if (moving && timings->CanPlayFootstep()){
					Mix_PlayChannel(-1, footstepSound, 0);
				}

			}

			drawgame();
	}
}

void drawgame(){
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	auto xRotate = glm::rotate(cameraRotation.x, glm::vec3(0, 1, 0));
	auto yRotate = glm::rotate(cameraRotation.y, glm::vec3(1, 0, 0));
	auto bothRotate = xRotate * yRotate;
	auto lookVector = bothRotate * glm::vec4(0, 0, 1, 0);

	auto cameraPosition = playerPosition + glm::vec3(0,4, 0);
	auto lookat = glm::lookAt(cameraPosition, glm::vec3(lookVector) + cameraPosition, glm::vec3(0.0, 1.0, 0.0));

	glLoadMatrixf(glm::value_ptr(lookat));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	auto perspective = glm::perspectiveFov<float>(1.27, windowWidth, windowHeight, 0.1f, 5000.0f);
	glLoadMatrixf(glm::value_ptr(perspective));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	if (wireframe){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	terrain->Render(glm::value_ptr(lookat), glm::value_ptr(perspective));



	auto scaled = glm::scale(lookat, glm::vec3(0.1, 0.1, 0.1));
	testTree->Render(glm::value_ptr(scaled), glm::value_ptr(perspective));
	auto moved = glm::translate(scaled, glm::vec3(250, 0.1, 250));

	testTree->Render(glm::value_ptr(moved), glm::value_ptr(perspective));


	glMatrixMode(GL_MODELVIEW);
	auto skyboxPosition = glm::translate( playerPosition* glm::vec3(1,1,1));
	auto combinedSkyboxMat = lookat*skyboxPosition;
	auto rotate = glm::rotate(combinedSkyboxMat, skyboxRotation, glm::vec3(0, 1, 0));

	glLoadMatrixf(glm::value_ptr(rotate));
	glEnable(GL_TEXTURE_2D);

	skybox->Render();

	// text
	/*
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, windowWidth, windowHeight, 0, -10, 10);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::stringstream stringstream;
	stringstream.precision(2);
	stringstream << "x: " << fixed << playerPosition.x << " y: " << fixed << playerPosition.y << " z: " << fixed << playerPosition.z;
	overlay->Render(stringstream.str(), SDL_Color{ 255, 255, 255 }, 10, 10);
	overlay->Render("Hello World", SDL_Color{ 255,255, 25 }, 10, 40);
	*/
	SDL_GL_SwapWindow(window);
}