#pragma once
#include "config.h"
#include "core/app.h"
#include "core/mathlib/vec4.h"
#include "core/shaderResource/ShaderResource.h"
#include "core/meshResource/MeshResource.h"


class PointLight {
public:
	PointLight();
	~PointLight();

	vec3 pos;
	vec4 colour;

	float intensity = 1.0f;
	
	void ApplyLight(std::shared_ptr<ShaderResource> shaderResource);
};