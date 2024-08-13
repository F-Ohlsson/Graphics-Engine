#pragma once
#include "config.h"
#include "core/app.h"
#include <memory>
#include <vector>

class Material {
public:
	std::shared_ptr<GLuint> shaderProgram;
	virtual void ApplyMaterial() = 0;
	virtual void ApplyMaterial(std::vector<GLuint> textures) = 0;
	virtual void ApplyMaterial(std::shared_ptr<GLuint> shaderProgram) = 0;
	virtual void UnbindMaterial() = 0;

};