#pragma once
#include "config.h"
#include "core/material/Material.h"
#include "core/shaderResource/ShaderResource.h"
#include "core/textureResource/TextureResource.h"
#include "core/mathlib/vec4.h"
#include <string>
#include <memory>
#include <vector>


class DefaultMaterial : public Material {
public:
	std::shared_ptr<GLuint> vertexShader;
	std::shared_ptr<GLuint> fragmentShader;
	std::shared_ptr<GLuint> texture;

	vec4 colour;

	DefaultMaterial();
	DefaultMaterial(std::shared_ptr<GLuint> shaderProgram);

	~DefaultMaterial();
	void ApplyMaterial();
	void ApplyMaterial(std::vector<GLuint> textures);
	void ApplyMaterial(std::shared_ptr<GLuint> shaderProgram);

	void UnbindMaterial();


};