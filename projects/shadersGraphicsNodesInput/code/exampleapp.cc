#pragma once
//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"


using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	//empty
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	this->windW = 640;
	this->windH = 480;
	this->window->SetSize(this->windW,this->windH);
	this->inputHandler = InputHandler();

	//Process input from keyboard and mouse
	window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
	{
		if(key == GLFW_KEY_ESCAPE )
			this->window->Close();		
		inputHandler.CheckKeyboardInput(key, action);
	});
	window->SetMousePressFunction([this](int32 button, int32 action, int32 mods) {
		inputHandler.CheckMouseInput(button, action);
	});
	window->SetMouseMoveFunction([this](float64 x, float64 y) {
		inputHandler.CheckMouseMovement(x, y);
	});

	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		
		//Setup base GraphicsNode
		graphN = GraphicsNode();
		graphN.meshR = std::make_shared<MeshResource>();
		graphN.textR = std::make_shared<TextureResource>();
		graphN.shadR = std::make_shared<ShaderResource>();

		//Start preparing cube resources
		graphN.textR->LoadTexture("../resources/textures/STRIPES.png", true);
		graphN.textR->BindTexture(graphN.textR->texture);
		graphN.meshR->SetupCube(0.5, 0.5, 0.5);

		//Setting up shaders
		std::string vertexShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/VertexShader.txt");
		std::string fragmentShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/FragmentShader.txt");

		graphN.shadR->vertexShader = graphN.shadR->SetupShader(vertexShaderString.c_str(), GL_VERTEX_SHADER);
		graphN.shadR->fragmentShader = graphN.shadR->SetupShader(fragmentShaderString.c_str(), GL_FRAGMENT_SHADER);
		
		graphN.shadR->shaderProgram = std::make_shared<GLuint>(graphN.shadR->CreateProgram(graphN.shadR->vertexShader, graphN.shadR->fragmentShader));

 		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Close()
{
	if (this->window->IsOpen())
		this->window->Close();

	Core::App::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
	glEnable(GL_DEPTH_TEST);

	Render::Grid grid;
	Camera camera = Camera(this->windW, this->windH, {0.0f,1.5f,3.0f});
	camera.projection = camera.ProjectionMatrix(1.0f, 0.1f, 50.0f);

	//Set Up Second Cube just to show it doesn't need reloading mesh/texture/shader
	//GraphicsNode secondNode = GraphicsNode();
	//secondNode.meshR = graphN.meshR;
	//secondNode.textR = graphN.textR;
	//secondNode.shadR = graphN.shadR;
	//secondNode.position = { 1,0,-1};

	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		
		
		//Update camera view
		camera.view = inverse(camera.ViewMatrix({ 0,0,0 }));
		mat4 vp = camera.projection * camera.view;

		graphN.DrawQuads(vp);
		//secondNode.DrawQuads(vp);
		//Only viewprojection to grid
		grid.Draw(&vp[0][0]);
		
		//Apply processed input to object
		inputHandler.ApplyInput(&graphN);

		this->window->SwapBuffers();
		
#ifdef CI_TEST
		// if we're running CI, we want to return and exit the application after one frame
		// break the loop and hopefully exit gracefully
		break;
#endif
	}
}


} // namespace Example