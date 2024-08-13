#pragma once
#include "config.h"
#include "core/pointLight/PointLight.h"

PointLight::PointLight() {
}
PointLight::~PointLight() {
}


void PointLight::ApplyLight(std::shared_ptr<ShaderResource> shaderResource) {
	shaderResource->UseProgram(shaderResource->shaderProgram);
	shaderResource->EditUniform3fv(shaderResource->shaderProgram, "pointLightPosition", this->pos);
	shaderResource->EditUniform4fv(shaderResource->shaderProgram, "pointLightColour", this->colour);
	shaderResource->EditUniform1f(shaderResource->shaderProgram, "pointLightIntensity", this->intensity);
	shaderResource->UseProgram(0);
}
