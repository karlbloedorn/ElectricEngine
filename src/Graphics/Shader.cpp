#include "Shader.hpp"
using namespace std;

Shader::Shader()
{
	numAttributes = 0;
	programID = 0;
	vertexShaderID = 0;
	fragmentShaderID = 0;
}
Shader::~Shader()
{
}

bool Shader::SetupShader(const string & vertexPath, const string & fragmentPath, list<string> attributes, list<string> uniforms){
	// Compile the shaders

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (vertexShaderID == 0) {
		printf("error: failed to create vertex shader\n");
		return false;
	}
	if (fragmentShaderID == 0) {
		printf("error: failed to create fragment shader\n");
		return false;
	}
	bool vertexSuccess = CompileShader(vertexPath, vertexShaderID);
	bool fragmentSuccess = CompileShader(fragmentPath, fragmentShaderID);
	if (!vertexSuccess || !fragmentSuccess){
		printf("error: failed to compile a shader\n");
		exit(1);
		return false;
	}
	programID = glCreateProgram();

	// Add the attributes
	for (string attributeName : attributes){
		glBindAttribLocation(programID, numAttributes++, attributeName.c_str());
	}
	// Link the shaders and create program
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	int isLinked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS,&isLinked);
	if (isLinked == 0)
	{
		
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);
		glDeleteProgram(programID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		std::printf("%s\n", &(errorLog[0]));
		printf("error: failed to link shaders");
		return false;
	}
	// Clean up
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	for (string uniform : uniforms){
		GLuint location = glGetUniformLocation(programID, uniform.c_str());
		if (location == GL_INVALID_INDEX){
			printf("error: uniform %s is invalid\n", uniform.c_str());
		}
		if (uniform == "projectionMatrix"){
			this->projectionMatrixLocation = location;
		}
		else if (uniform == "modelMatrix"){
			this->modelMatrixLocation = location;
		}
		else if (uniform == "viewMatrix"){
			this->viewMatrixLocation = location;
		}
		else if (uniform == "texture0"){
			this->texture0 = location;
		}
		else if (uniform == "texture1"){
			this->texture1 = location;
		}
		else if (uniform == "texture2"){
			this->texture2 = location;
		}
		else if (uniform == "texture3"){
			this->texture3 = location;
		}
		else if (uniform == "texture4"){
			this->texture4 = location;
		}
		else if (uniform == "texture5"){
			this->texture5 = location;
		}		
	}
	return true;
}

void Shader::EnableShader(){
	glUseProgram(programID);
	for (int i = 0; i < numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}
void Shader::DisableShader(){
	glUseProgram(0);
	for (int i = 0; i < numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

bool Shader::CompileShader(const std::string& filePath, GLuint id) {
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail()) {
		perror(filePath.c_str());
		return false;
	}
	std::string fileContents = "";
	std::string line;
	while (std::getline(shaderFile, line)) {
		fileContents += line + "\n";
	}
	shaderFile.close();
	const char* contentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);
	glCompileShader(id);
	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(id);
		std::printf("%s\n", &(errorLog[0]));
		return false;
	}
	return true;
}