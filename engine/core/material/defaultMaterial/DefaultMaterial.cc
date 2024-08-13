#pragma once
#include "config.h"
#include "DefaultMaterial.h"

DefaultMaterial::DefaultMaterial() {
	//
	//How to make sure it's not already loaded?
	//

	ShaderResource shadR = ShaderResource();
	std::string shaderStringVertex = shadR.LoadShaderFromFile("../resources/shaders/DefaultVertexShader.glsl");
	this->vertexShader = std::make_shared<GLuint>(shadR.SetupShader(shaderStringVertex.c_str(), GL_VERTEX_SHADER));

	std::string shaderStringFragment = shadR.LoadShaderFromFile("../resources/shaders/DefaultFragmentShader.glsl");
	this->fragmentShader = std::make_shared<GLuint>(shadR.SetupShader(shaderStringFragment.c_str(), GL_FRAGMENT_SHADER));

	this->shaderProgram = std::make_shared<GLuint>(shadR.CreateProgram(*this->vertexShader.get(), *this->fragmentShader.get()));

	TextureResource textR = TextureResource();
	textR.LoadTexture("../resources/textures/default.png", false);
	
	this->texture = std::make_shared<GLuint>(textR.texture);

}

DefaultMaterial::DefaultMaterial(std::shared_ptr<GLuint> shaderProgram) {
	GLuint shaders[2]; //How many shaders to fetch from program
	glGetAttachedShaders(*shaderProgram.get(), 2, 0, shaders);

	this->vertexShader = std::make_shared<GLuint>(shaders[0]);
	this->fragmentShader = std::make_shared<GLuint>(shaders[1]);
	this->shaderProgram = shaderProgram;
}


DefaultMaterial::~DefaultMaterial() {

}

void DefaultMaterial::ApplyMaterial() {
	ShaderResource shadR;
	TextureResource textR;
 	shadR.UseProgram(this->shaderProgram);
	//shadR.EditUniform1f(this->shaderProgram, "shininess", this->shininess);
	if(this->texture != NULL)
		textR.BindTexture(*this->texture.get());
	shadR.EditUniform1i(this->shaderProgram, "tex");
	//shadR.EditUniform4fv(this->shaderProgram, "materialAmbientColour", this->ambient);
	//shadR.EditUniform4fv(this->shaderProgram, "materialDiffuseColour", this->diffuse);
	//shadR.EditUniform4fv(this->shaderProgram, "materialSpecularColour", this->specular);
}

void DefaultMaterial::ApplyMaterial(std::vector<GLuint> textures) {
	int i = 0;
}

void DefaultMaterial::ApplyMaterial(std::shared_ptr<GLuint> shaderProgram) {
	int i = 0;
}


void DefaultMaterial::UnbindMaterial() {
	std::shared_ptr<ShaderResource> shadR = std::make_shared<ShaderResource>();
	std::shared_ptr<TextureResource> textR = std::make_shared<TextureResource>();
	textR->BindTexture(0);
	shadR->UseProgram(0);
}
