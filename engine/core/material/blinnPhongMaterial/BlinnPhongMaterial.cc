#pragma once
#include "config.h"
#include "BlinnPhongMaterial.h"

BlinnPhongMaterial::BlinnPhongMaterial() {
	//
	//How to make sure it's not already loaded?
	//

	ShaderResource shadR = ShaderResource();
	std::string shaderStringVertex = shadR.LoadShaderFromFile("../resources/shaders/BlinnPhongVertex.glsl");
	this->vertexShader = std::make_shared<GLuint>(shadR.SetupShader(shaderStringVertex.c_str(), GL_VERTEX_SHADER));

	std::string shaderStringFragment = shadR.LoadShaderFromFile("../resources/shaders/BlinnPhongFragment.glsl");
	this->fragmentShader = std::make_shared<GLuint>(shadR.SetupShader(shaderStringFragment.c_str(), GL_FRAGMENT_SHADER));

	this->shaderProgram = std::make_shared<GLuint>(shadR.CreateProgram(*this->vertexShader.get(), *this->fragmentShader.get()));
}

BlinnPhongMaterial::BlinnPhongMaterial(std::shared_ptr<GLuint> shaderProgram) {
	GLuint shaders[2]; //How many shaders to fetch from program
	glGetAttachedShaders(*shaderProgram.get(), 2, 0, shaders);

	this->vertexShader = std::make_shared<GLuint>(shaders[0]);
	this->fragmentShader = std::make_shared<GLuint>(shaders[1]);
	this->shaderProgram = shaderProgram;
}


BlinnPhongMaterial::~BlinnPhongMaterial() {

}

void BlinnPhongMaterial::ApplyMaterial() {
	ShaderResource shadR;
	TextureResource textR;
 	shadR.UseProgram(this->shaderProgram);
	shadR.EditUniform1f(this->shaderProgram, "shininess", this->shininess);
	if(this->texture != NULL)
		textR.BindTexture(*this->texture.get());
	shadR.EditUniform1i(this->shaderProgram, "tex");
	shadR.EditUniform4fv(this->shaderProgram, "materialAmbientColour", this->ambient);
	shadR.EditUniform4fv(this->shaderProgram, "materialDiffuseColour", this->diffuse);
	shadR.EditUniform4fv(this->shaderProgram, "materialSpecularColour", this->specular);
}

void BlinnPhongMaterial::ApplyMaterial(std::shared_ptr<GLuint> shaderProgram) {
	ShaderResource shadR;
	TextureResource textR;
	shadR.UseProgram(shaderProgram);
	shadR.EditUniform1f(shaderProgram, "shininess", this->shininess);
	if (this->texture != NULL)
		textR.BindTexture(*this->texture.get());
	shadR.EditUniform1i(shaderProgram, "tex");
	shadR.EditUniform4fv(shaderProgram, "materialAmbientColour", this->ambient);
	shadR.EditUniform4fv(shaderProgram, "materialDiffuseColour", this->diffuse);
	shadR.EditUniform4fv(shaderProgram, "materialSpecularColour", this->specular);
}

void BlinnPhongMaterial::ApplyMaterial(std::vector<GLuint> textures) {
	std::shared_ptr<ShaderResource> shadR = std::make_shared<ShaderResource>();
	std::shared_ptr<TextureResource> textR = std::make_shared<TextureResource>();
	shadR->UseProgram(this->shaderProgram);
	shadR->EditUniform1f(this->shaderProgram, "shininess", this->shininess);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	shadR->EditUniform1i(this->shaderProgram, "tex", 0);
	if (textures.size() > 1) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		shadR->EditUniform1i(this->shaderProgram, "normalMap", 1);
	}
	
	shadR->EditUniform4fv(this->shaderProgram, "materialAmbientColour", this->ambient);
	shadR->EditUniform4fv(this->shaderProgram, "materialDiffuseColour", this->diffuse);
	shadR->EditUniform4fv(this->shaderProgram, "materialSpecularColour", this->specular);
}

void BlinnPhongMaterial::UnbindMaterial() {
	std::shared_ptr<ShaderResource> shadR = std::make_shared<ShaderResource>();
	std::shared_ptr<TextureResource> textR = std::make_shared<TextureResource>();
	textR->BindTexture(0);
	shadR->UseProgram(0);
}
