#include "Rendering.hpp"

Rendering::Rendering(){}

bool Rendering::Initialize(string & error, int windowHeight, int windowWidth, bool fullScreen, string assetPath){

	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;

	int opts = SDL_WINDOW_OPENGL;
	if (fullScreen){
		opts = opts | SDL_WINDOW_FULLSCREEN;
	}
	
	this->window = SDL_CreateWindow("Test Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->windowWidth, this->windowHeight, opts);
	if (window == nullptr){
		error = string("Failed to start windowing subsystem");
		return false;
	}
	auto glcontext = SDL_GL_CreateContext(this->window);
	if (glcontext == nullptr){
		error = string("Failed to start graphics subsystem");
		return false;
	}
	auto glewinit = glewInit();
	if (glewinit != GLEW_OK){
		error = string("Failed to start graphics subsystem");
		return false;
	}
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(1);
	}
	this->roboto50 = TTF_OpenFont( string(assetPath + "fonts/roboto.ttf").c_str(), 50);
	if (!this->roboto50) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(1);
	}

	this->roboto200 = TTF_OpenFont(string(assetPath + "fonts/roboto.ttf").c_str(), 200);
	if (!this->roboto200) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(1);
	}
	this->skybox = new Skybox(assetPath);

	this->entityShader = new Shader();
	this->entityShader->SetupShader(assetPath + "shaders/entities.vert", assetPath + "shaders/entities.frag",
		list < string > {
		"in_Position",
			"in_TextureCoord",
			"in_Normal"
	},
	list < string > {
			"projectionMatrix",
				"modelMatrix",
				"viewMatrix",
				"texture0"
	});

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glEnable(GL_TEXTURE_2D);
	return true;
}

bool Rendering::AddEntity(Entity * entity, string assetPath){
	Mesh * mesh = new Mesh();
	mesh->LoadFromObj(assetPath + "meshes/" + entity->name +"/", entity->name + ".obj", "");
	this->meshMap[entity->entityID] = mesh;
	return true;
}

void Rendering::ShowLoading(float progress){

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.1);
	glDisable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, this->windowWidth, this->windowHeight, 0, -10, 10);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	std::stringstream stringstream;
	stringstream.precision(0);
	stringstream << fixed << progress << "%";
	this->RenderText("Loading", this->roboto50, SDL_Color{ 255, 255, 25 }, 10, 10);
	this->RenderText(stringstream.str(), this->roboto200, SDL_Color{ 255, 255, 255 }, 10, 70);
	SDL_GL_SwapWindow(window);
}

void Rendering::RenderText(std::string text, TTF_Font * font, SDL_Color color, int x, int y)
{
	SDL_Surface * sFont = TTF_RenderText_Blended(font, text.c_str(), color);
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sFont->pixels);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0); glVertex2f(x, y);
		glTexCoord2f(1, 0); glVertex2f(x + sFont->w, y);
		glTexCoord2f(1, 1); glVertex2f(x + sFont->w, y + sFont->h);
		glTexCoord2f(0, 1); glVertex2f(x, y + sFont->h);
	}
	glEnd();
	glDeleteTextures(1, &texture);
	SDL_FreeSurface(sFont);
}

void Rendering::RenderGame(map<int, vector<int>> * renderMap, map<int, StaticProp *> * staticPropMap){
	auto skyboxRotation = 0.0f;
	auto xCamRotate = glm::rotate(cameraRotation.x, glm::vec3(0, 1, 0));
	auto yCamRotate = glm::rotate(cameraRotation.y, glm::vec3(1, 0, 0));
	auto bothRotate = xCamRotate * yCamRotate;
	auto lookVector = bothRotate * glm::vec4(0, 0, 1, 0);
	auto cameraPosition = playerPosition + glm::vec3(0, 12, 0);
	auto lookat = glm::lookAt(cameraPosition, glm::vec3(lookVector) + cameraPosition, glm::vec3(0.0, 1.0, 0.0));

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.1);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (this->wireframe){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	this->entityShader->EnableShader();
	glUniformMatrix4fv(this->entityShader->projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(perspective));
	glUniformMatrix4fv(this->entityShader->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(lookat));

	for (auto entity : *renderMap){
		int entityID = entity.first;
		vector<int> instances = entity.second;
		
		if (instances.size() > 0){
			this->meshMap[entityID]->RenderInstances(this->entityShader, instances, staticPropMap);
		}
	}
	this->entityShader->DisableShader();

	SDL_GL_SwapWindow(window);

}