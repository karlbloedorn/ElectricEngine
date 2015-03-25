#pragma once
#include <iostream>
#include <GL/gl3w.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <list>

using namespace std;

class Shader
{
private:
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	int numAttributes;
	bool CompileShader(const string& filePath, GLuint id);
	GLuint GetUniformLocation(const string & uniformName);
public:
	Shader();
	~Shader();
	bool SetupShader(const string & vertexPath, const string & fragmentPath, list<string> attributes, list<string> uniforms);
	void EnableShader();
	void DisableShader();
	GLuint projectionMatrixLocation;
	GLuint modelMatrixLocation;
	GLuint viewMatrixLocation;
	GLuint texture0;
	GLuint texture1;
	GLuint texture2;
	GLuint texture3;
	GLuint texture4;
};
