#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2022 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "render/window.h"
#include "core/app.h"
#include "core/meshResource/MeshResource.h"
#include "core/shaderResource/ShaderResource.h"


namespace MeshResourceApp
{
class MeshResourceApp : public Core::App
{
public:
	/// constructor
	MeshResourceApp();
	/// destructor
	~MeshResourceApp();

	/// open app
	bool Open();
	/// close app
	void Close();
	/// run app
	void Run();
	MeshResource mR;
	ShaderResource sR;
private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
	Display::Window* window;
};
} // namespace Example