#pragma once
#include "config.h"
#include "core/dirLight/DirLight.h"
#include "core/pointLight/PointLight.h"
#include "core/mathlib/vec3.h";
#include "core/mathlib/vec4.h";
#include "core/mathlib/mat4.h";
#include "core/graphicsNode/GraphicsNode.h";
#include "core/material/defaultMaterial/DefaultMaterial.h"
#include <vector>

class LightManager {
private:
	std::vector<PointLight> pointLightV;
	DirLight dirLight;
public:
		std::shared_ptr<MeshResource> sphere;

		LightManager();
		void ApplyLight(std::shared_ptr<ShaderResource> shaderResource, mat4 viewMatrix, mat4 projMatrix);
		void ApplyDeferredDirLight(std::shared_ptr<GLuint> shaderProgram );
		void ApplyDeferredPointLight(std::shared_ptr<GLuint> shaderProgram, mat4 viewMatrix, mat4 projMatrix);
		void AddPointLight(PointLight newPL);
		void AddPointLight(vec3 position, vec4 colour, float intensity);
		void DrawPointLights(mat4 viewMatrix, mat4 projMatrix);
		void NewDirLight(DirLight newDL);
		void NewDirLight(vec3 direction, vec4 colour, float intensity);

		void UpdatePositions(float x, float y, float z);
		void SetUp();
};