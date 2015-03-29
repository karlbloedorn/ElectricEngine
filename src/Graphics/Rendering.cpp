#include "Rendering.hpp"

Rendering::Rendering(){}

bool Rendering::Initialize(string & error, int windowHeight, int windowWidth, bool fullScreen, string assetPath, World * world){

	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;

	int opts = SDL_WINDOW_OPENGL;
	if (fullScreen){
		opts = opts | SDL_WINDOW_FULLSCREEN;
	}  
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
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
	/*
	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}
	if (!gl3wIsSupported(3, 2)) {
		fprintf(stderr, "OpenGL 3.2 not supported\n");
		return -1;
	}*/

	glbinding::Binding::initialize();
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));

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
	this->skybox = new Skybox();
	this->skybox->Load(assetPath);
	this->grid = new Grid();
	this->grid->Load(assetPath, world);

	this->rawTextureShader = new Shader();
	this->rawTextureShader->SetupShader(assetPath + "shaders/rawTexture.vert", assetPath + "shaders/rawTexture.frag",
	list < string > {
		"in_Position",
		"in_TextureCoord",
	},
	list < string > {
			"projectionMatrix",
				"modelMatrix",
				"viewMatrix",
				"texture0"
	});

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


	this->terrainShader = new Shader();
	this->terrainShader->SetupShader(assetPath + "shaders/terrain.vert", assetPath + "shaders/terrain.frag",
		list < string > {
		"in_Position"
	},
	list < string > {
			"projectionMatrix",
				"modelMatrix",
				"viewMatrix",
				"texture0",
				"texture1",
				"texture2",
				"texture3",
				"texture4",
				"texture5"
		});


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.1, 0.1, 0.1, 1.0);


	return true;
}

bool Rendering::AddEntity(Entity * entity, string assetPath){
	Mesh * mesh = new Mesh();
	mesh->LoadFromObj(assetPath + "meshes/" + entity->name +"/", entity->name + ".obj", "");
	this->meshMap[entity->entityID] = mesh;
	return true;
}

void Rendering::ShowLoading(float progress){
	/*
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
	*/
}

void Rendering::RenderText(std::string text, TTF_Font * font, SDL_Color color, int x, int y)
{
	/*
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
	SDL_FreeSurface(sFont);*/
}

void Rendering::RenderGame(map<int, vector<int>> * renderMap, map<int, StaticProp *> * staticPropMap){
//	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glAlphaFunc(GL_GREATER, 0.1);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (this->wireframe){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	auto perspective = glm::perspectiveFov<float>(1.27, windowWidth, windowHeight, 0.1f, 10000.0f);

	// Skybox rendering
	this->rawTextureShader->EnableShader();
	auto skyboxPosition = glm::translate(cameraPosition);
	auto skyboxRotate = glm::rotate(skyboxPosition, skyboxRotation, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(this->entityShader->projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(perspective));
	glUniformMatrix4fv(this->entityShader->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(lookAt));
	glUniformMatrix4fv(this->entityShader->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(skyboxRotate));
	skybox->Render(this->rawTextureShader);
	this->rawTextureShader->DisableShader();


	// Terrain rendering
	this->terrainShader->EnableShader();

	glUniformMatrix4fv(this->terrainShader->projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(perspective));
	glUniformMatrix4fv(this->terrainShader->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(lookAt));
	
	grid->RenderInstances(this->terrainShader, this->cameraPosition);
	this->terrainShader->DisableShader();



	// Entity rendering
	this->entityShader->EnableShader();
	glUniformMatrix4fv(this->entityShader->projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(perspective));
	glUniformMatrix4fv(this->entityShader->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(lookAt));

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