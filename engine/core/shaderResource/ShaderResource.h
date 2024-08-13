#pragma once
#include "config.h"
#include "core/app.h"
#include <string>
#include <fstream>
#include <sstream>
#include "core/mathlib/mat4.h"
#include "core/mathlib/vec4.h"

class ShaderResource{
public:
	GLuint vertexShader;
	GLuint fragmentShader;
	std::shared_ptr<GLuint> shaderProgram;

	ShaderResource();
	~ShaderResource();

	GLuint SetupShader(const GLchar* input, GLuint type);
	std::string LoadShaderFromFile(char* filePath);

	GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);
	void UseProgram(std::shared_ptr<GLuint> shaderProgram);

	GLuint FetchUniform(std::shared_ptr<GLuint> program, const char* uniformName);
	void EditUniformMat4fv(std::shared_ptr<GLuint> program, const char* uniformName, mat4 data);
	void EditUniform4fv(std::shared_ptr<GLuint> program, const char* uniformName, vec4 data);
	void EditUniform3fv(std::shared_ptr<GLuint> program, const char* uniformName, vec3 data);
	void EditUniform1i(std::shared_ptr<GLuint> program, const char* uniformName, GLuint textureLocation = 0);
	void EditUniform1f(std::shared_ptr<GLuint> program, const char* uniformName, float data);
};