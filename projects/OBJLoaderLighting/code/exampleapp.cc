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
		graphN.textR = std::make_shared<TextureResource>();
		graphN.shadR = std::make_shared<ShaderResource>();

		//Start preparing model resources
		graphN.textR->LoadTexture("../resources/textures/OHNO.png", false);
		graphN.textR->BindTexture(graphN.textR->texture);
		graphN.meshVector.push_back(std::make_shared<MeshResource>());
		graphN.meshVector[0]->LoadOBJFile("../resources/meshes/teapot.obj");
		//Setting up shaders
		std::string vertexShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/BlinnPhongBaseVertex.glsl");
		std::string fragmentShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/BlinnPhongBaseFragment.glsl");

		graphN.shadR->vertexShader = graphN.shadR->SetupShader(vertexShaderString.c_str(), GL_VERTEX_SHADER);
		graphN.shadR->fragmentShader = graphN.shadR->SetupShader(fragmentShaderString.c_str(), GL_FRAGMENT_SHADER);
		
		graphN.shadR->shaderProgram = std::make_shared<GLuint>(graphN.shadR->CreateProgram(graphN.shadR->vertexShader, graphN.shadR->fragmentShader));

		BlinnPhongMaterial bpMaterial = BlinnPhongMaterial(graphN.shadR->shaderProgram);
		bpMaterial.shininess = 10.0f;
		bpMaterial.texture = std::make_shared<GLuint>(graphN.textR->texture);
		graphN.meshVector[0].get()->primitiveVector[0].material = std::make_shared<BlinnPhongMaterial>(bpMaterial);


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
	GraphicsNode test = GraphicsNode();
	test.textR = std::make_shared<TextureResource>();
	test.shadR = std::make_shared<ShaderResource>();
	test.textR->LoadTexture("../resources/textures/OHNO.png", false);
	test.textR->BindTexture(test.textR->texture);
	test.meshVector.push_back(std::make_shared<MeshResource>());
	test.meshVector[0]->LoadOBJFile("../resources/meshes/Sphere.obj");
	test.meshVector[0].get()->primitiveVector[0].material = graphN.meshVector[0].get()->primitiveVector[0].material;


	float counter = 0.0f;
	glEnable(GL_DEPTH_TEST);

	Render::Grid grid;
	Camera camera = Camera(this->windW, this->windH, {0.0f,5.0f,-10.0f});
	camera.projection = camera.ProjectionMatrix(1.0f, 0.1f, 50.0f);


	PointLight pointLight = PointLight();
	pointLight.pos = {0,3,0};
	pointLight.intensity = 10;

	DirLight dirLight = DirLight();
	dirLight.direction = { 0.0f,1.0f,0.0f };
	dirLight.colour = { 1.0f,0.0f,0.0f,1.0f };
	dirLight.intensity = 1.0f;

	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		
		
		//Update camera view
		camera.view = inverse(camera.ViewMatrix({ 0,0,0 }));
		mat4 vp = camera.projection * camera.view;

		//float scaling = 0.05f; //For shoes and teapot
		//float scaling = 0.5f; //For bird
		float scaling = 1;


		grid.Draw(&vp[0][0]);

		//camera.position = { -sin(counter/2) * 15, camera.position.y, -cos(counter/2) * 15 };
		pointLight.pos = { sin(counter) * 5, 1, cos(counter) * 5 };
		test.position = pointLight.pos;
		test.DrawTris(camera.view, camera.projection, 0.5);

		pointLight.ApplyLight(graphN.shadR);
		dirLight.ApplyLight(graphN.shadR->shaderProgram);

		graphN.DrawTris(camera.view, camera.projection, scaling);
		
		//Apply processed input to object
		inputHandler.ApplyInput(&graphN);

		this->window->SwapBuffers();
		if (!inputHandler.SpaceToggle()) {
			counter += 0.05f;
		}
		
#ifdef CI_TEST
		// if we're running CI, we want to return and exit the application after one frame
		// break the loop and hopefully exit gracefully
		break;
#endif
	}
}


} // namespace Example