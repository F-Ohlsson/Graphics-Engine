#pragma once
#include "config.h"
#include "core/material/Material.h"
#include "core/shaderResource/ShaderResource.h"
#include "core/textureResource/TextureResource.h"
#include "core/mathlib/vec4.h"
#include <string>
#include <memory>
#include <vector>


class BlinnPhongMaterial: public Material {
public:
	std::shared_ptr<GLuint> vertexShader;
	std::shared_ptr<GLuint> fragmentShader;
	std::shared_ptr<GLuint> texture;

	float shininess = 10.0f;
	vec4 ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	vec4 diffuse = vec4(0.0f, 0.0f, 0.5f, 1.0f);
	vec4 specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	BlinnPhongMaterial();
	BlinnPhongMaterial(std::shared_ptr<GLuint> shaderProgram);

	~BlinnPhongMaterial();
	void ApplyMaterial();
	void ApplyMaterial(std::vector<GLuint> textures);
	void ApplyMaterial(std::shared_ptr<GLuint> shaderProgram);
	void UnbindMaterial();


};