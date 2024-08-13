#pragma once
//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "GLTFNormalMap.h"


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
	this->windW = 1920;
	this->windH = 1080;
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
		graphN.shadR = std::make_shared<ShaderResource>();

		
		//GLTF files
		//graphN.LoadGLTF("../resources/gltf/Cube/Cube.gltf"); scaling = 0.5f; normalShader = false;
		//graphN.LoadGLTF("../resources/gltf/Avocado/Avocado.gltf"); scaling = 15.0f; normalShader = true;
		//graphN.LoadGLTF("../resources/gltf/DamagedHelmet/DamagedHelmet.gltf"); scaling = 0.5f;
		graphN.LoadGLTF("../resources/gltf/FlightHelmet/FlightHelmet.gltf"); scaling = 2.0f;
		//graphN.LoadGLTF("../resources/gltf/NormalTangentMirrorTest/NormalTangentMirrorTest.gltf"); scaling = 0.5f;


		//Setting up shaders
		
		std::string vertexShaderString;
		std::string fragmentShaderString;
		if (normalShader) {
			vertexShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/BlinnPhongNormalVertex.glsl"); //Normal Map shaders
			fragmentShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/BlinnPhongNormalFragment.glsl");
		}
		else {
			vertexShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/BlinnPhongBaseVertex.glsl"); //Base Blinn Phong Shaders
			fragmentShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/BlinnPhongBaseFragment.glsl");
		}

		graphN.shadR->vertexShader = graphN.shadR->SetupShader(vertexShaderString.c_str(), GL_VERTEX_SHADER);
		graphN.shadR->fragmentShader = graphN.shadR->SetupShader(fragmentShaderString.c_str(), GL_FRAGMENT_SHADER);
		graphN.shadR->shaderProgram = std::make_shared<GLuint>(graphN.shadR->CreateProgram(graphN.shadR->vertexShader, graphN.shadR->fragmentShader));
		

		BlinnPhongMaterial bpMaterial = BlinnPhongMaterial(graphN.shadR->shaderProgram);
		bpMaterial.shininess = 10.0f;
		bpMaterial.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
		//bpmaterial.diffuse = { 0.2f,0.2f,1.0f,1.0f };
		//bpmaterial.specular = { 1.0f,1.0f,1.0f,1.0f };
		bpMaterial.diffuse = { 1.0f,1.0f,1.0f,1.0f };
		bpMaterial.specular = { 1.0f,1.0f,1.0f,1.0f };
		std::shared_ptr<BlinnPhongMaterial> bpMaterialPointer = std::make_shared<BlinnPhongMaterial>(bpMaterial);

		//Assign Blinn Phong Material to all primitives of all meshes
		for (int i = 0; i < graphN.meshVector.size(); i++) {
			MeshResource* mesh = graphN.meshVector[i].get();
			for (int j = 0; j < mesh->primitiveVector.size(); j++) {
				mesh->primitiveVector[j].material = bpMaterialPointer;
			}
		}

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
	float counter = 0.0f;
	glEnable(GL_DEPTH_TEST);

	Render::Grid grid;
	Camera camera = Camera(this->windW, this->windH, {0.0f,2.0f,2.0f});
	camera.projection = camera.ProjectionMatrix(1.0f, 0.1f, 2000.0f);


	PointLight pointLight = PointLight();
	pointLight.pos = {10,20,10};
	pointLight.intensity = 1;
	pointLight.colour = { 1,1,1,1 };

	DirLight dirLight = DirLight();
	dirLight.direction = { 0.0f,1.0f,0.0f };
	dirLight.colour = { 1.0f,1.0f,1.0f,1.0f };
	dirLight.intensity = 1.0f;


	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		
		
		//Update camera view
		camera.view = inverse(camera.ViewMatrix({ 0,0,0 }));
		mat4 vp = camera.projection * camera.view;


		graphN.DrawTris(camera.view, camera.projection, scaling);
		grid.Draw(&vp[0][0]);

		//camera.position = { -sin(counter/2) * 15, camera.position.y, -cos(counter/2) * 15 };
		pointLight.pos = { sin(counter) * 1, 0, cos(counter) * 1 };


		pointLight.ApplyLight(graphN.shadR);
		dirLight.ApplyLight(graphN.shadR->shaderProgram);



		//Apply processed input to object
		inputHandler.ApplyInput(&graphN);

		this->window->SwapBuffers();
		if (inputHandler.SpaceToggle()) {
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