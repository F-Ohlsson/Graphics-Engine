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
#include "core/meshResource/MeshResource.h"
#include "core/textureResource/TextureResource.h"
#include "core/shaderResource/ShaderResource.h"
#include "core/camera/Camera.h"


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
	MeshResource meshR;
	ShaderResource shadR;
	TextureResource textR;
private:

	GLuint program = -1;
	GLuint vertexShader = -1;
	GLuint pixelShader = -1;
	GLuint triangle = -1;

	GLuint windW = 0;
	GLuint windH = 0;

	Display::Window* window = NULL;
};
} // namespace Example