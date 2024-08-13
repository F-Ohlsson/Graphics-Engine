#pragma once
#include "config.h"
#include <memory>
#include "core/meshResource/MeshResource.h"
#include "core/textureResource/TextureResource.h"
#include "core/shaderResource/ShaderResource.h"
#include "core/mathlib/vec4.h"
#include "core/mathlib/mat4.h"
#include <nlohmann/json.hpp>
#include <fx/gltf.h>
#include <vector>



class GraphicsNode {
public:
	vec3 position = vec3();
	float radiansX = 0.0f; //Keeps track of current rotation along X axis
	float radiansY = 0.0f; //Keeps track of current rotation along Y axis
	float scaling;


	//Legacy, will break old projects if removed
	std::shared_ptr<MeshResource> meshR;
	std::shared_ptr<TextureResource> textR;
	std::shared_ptr<ShaderResource> shadR;
	//


	std::vector<std::shared_ptr<MeshResource>> meshVector;
	std::vector<std::shared_ptr<TextureResource>> textureVector;
	std::vector<std::shared_ptr<ShaderResource>> shaderVector;


	GraphicsNode();
	GraphicsNode(const GraphicsNode&);
	~GraphicsNode();
	GraphicsNode(GraphicsNode&& move) noexcept;
	GraphicsNode& operator =(GraphicsNode&&) noexcept;
	void DrawQuads(mat4 viewProjection); //Doesn't use index buffer or vertex array. It's not good
	void DrawTrisDeferred(mat4 view, mat4 projection, float scaling, std::shared_ptr<GLuint> geometryShaderProgram);
	void DrawTris(mat4 view, mat4 projection, float scaling); //Uses index buffer and vertex array. Seems better
	void LoadGLTF(std::string filename);
	std::string getAccessorString(int attribArrayIndex);
};