#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2022 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "render/window.h"
#include "render/grid.h"
#include "core/app.h"
#include <cstring>
#include "core/meshResource/MeshResource.h"
#include "core/textureResource/TextureResource.h"
#include "core/shaderResource/ShaderResource.h"
#include "core/graphicsNode/GraphicsNode.h"
#include "core/camera/Camera.h"
#include "core/inputHandler/InputHandler.h"
#include "core/pointLight/PointLight.h"
#include "core/dirLight/DirLight.h"
#include "core/material/Material.h"
#include "core/material/blinnPhongMaterial/BlinnPhongMaterial.h"



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
	GraphicsNode graphN;
	InputHandler inputHandler;

	Display::Window* window = NULL;
};
} // namespace Example