#pragma once
#include "config.h"
#include "core/mathlib/vec3.h"
#include "core/mathlib/vec4.h"
#include "core/shaderResource/ShaderResource.h"

class DirLight {
public:
	DirLight();
	~DirLight();

	vec3 direction;
	vec4 colour;
	float intensity;

	void ApplyLight(std::shared_ptr<GLuint> shaderProgram);
};