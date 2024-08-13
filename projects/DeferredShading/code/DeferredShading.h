#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2022 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "core/camera/Camera.h"
#include "core/graphicsNode/GraphicsNode.h"
#include "core/inputHandler/InputHandler.h"
#include "core/lightManager/LightManager.h"
#include "core/material/blinnPhongMaterial/BlinnPhongMaterial.h"
#include "core/material/defaultMaterial/DefaultMaterial.h"
#include "core/meshResource/MeshResource.h"
#include "core/shaderResource/ShaderResource.h"
#include "core/textureResource/TextureResource.h"
#include "render/window.h"
#include "render/grid.h"
#include <cstring>
#include <fx/gltf.h>
#include <nlohmann/json.hpp>
#include <vector>

struct FrameBuffer {
	GLuint gBuffer;
	std::shared_ptr<GLuint> gBufferShaderProgram;

	GLuint lBuffer;
	std::shared_ptr<GLuint> lBufferPointShaderProgram;
	std::shared_ptr<GLuint> lBufferDirShaderProgram;



	GLuint gPosition;
	GLuint gNormal;
	GLuint gAlbedo;
	GLuint gDepth;

	GLuint lTex;
};


namespace Example
{
class ExampleApp : public Core::App
{
public:
	/// constructor
	ExampleApp();
	/// destructor
	~ExampleApp();

	/// open app
	bool Open();
	/// close app
	void Close();
	/// run app
	void Run();

private:
	GLuint windW = 0;
	GLuint windH = 0;
	float scaling = 1.0f;
	bool normalShader = true;
	std::vector<std::shared_ptr<GraphicsNode>> nodeVector;
	InputHandler inputHandler;
	LightManager lightMan;
	FrameBuffer fBuf;


	Display::Window* window = NULL;
};
} // namespace Example