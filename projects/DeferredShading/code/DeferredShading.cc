#pragma once
//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "DeferredShading.h"


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
		//Set up framebuffer for geometry pass
		glGenFramebuffers(1, &fBuf.gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, fBuf.gBuffer);

		//Position colour buffer
		glGenTextures(1, &fBuf.gPosition);
		glBindTexture(GL_TEXTURE_2D, fBuf.gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->windW, this->windH, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fBuf.gPosition, 0);

		//Normal colour buffer
		glGenTextures(1, &fBuf.gNormal);
		glBindTexture(GL_TEXTURE_2D, fBuf.gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->windW, this->windH, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fBuf.gNormal, 0);

		//Albedo colour buffer
		glGenTextures(1, &fBuf.gAlbedo);
		glBindTexture(GL_TEXTURE_2D, fBuf.gAlbedo);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->windW, this->windH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, fBuf.gAlbedo, 0);

		//Depth buffer
		glGenTextures(1, &fBuf.gDepth);
		glBindTexture(GL_TEXTURE_2D, fBuf.gDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->windW, this->windH, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, fBuf.gDepth, 0);

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		//Setup Buffer for light pass
		glGenFramebuffers(1, &fBuf.lBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, fBuf.lBuffer);

		//Point light pass Buffer
		glGenTextures(1, &fBuf.lTex);
		glBindTexture(GL_TEXTURE_2D, fBuf.lTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->windW, this->windH, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fBuf.lTex, 0);

		attachments[1] = { GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, attachments);

		//Set up GraphicsNodes for various meshes
		std::vector<std::shared_ptr<GraphicsNode>> tempNodes;

		GraphicsNode graphN = GraphicsNode();
		graphN.shadR = std::make_shared<ShaderResource>();

		
		GraphicsNode avocado = GraphicsNode();
		avocado.LoadGLTF("../resources/gltf/Avocado/Avocado.gltf"); avocado.scaling = 15.0f; normalShader = true;
		tempNodes.push_back(std::make_shared<GraphicsNode>(avocado));

		GraphicsNode damagedHelm = GraphicsNode();
		damagedHelm.LoadGLTF("../resources/gltf/DamagedHelmet/DamagedHelmet.gltf"); damagedHelm.scaling = 0.5f; normalShader = true;
		tempNodes.push_back(std::make_shared<GraphicsNode>(damagedHelm));

		GraphicsNode flightHelm = GraphicsNode();
		flightHelm.LoadGLTF("../resources/gltf/FlightHelmet/FlightHelmet.gltf"); flightHelm.scaling = 2.0f;
		tempNodes.push_back(std::make_shared<GraphicsNode>(flightHelm));

		
		//GLTF files
		graphN.LoadGLTF("../resources/gltf/Cube/Cube.gltf"); scaling = 0.3f; normalShader = false; //Must load something here for materials to work, I don't quite know why but at this point I don't care to solve it
		//graphN.LoadGLTF("../resources/gltf/Avocado/Avocado.gltf"); scaling = 15.0f; normalShader = true;
		//graphN.LoadGLTF("../resources/gltf/DamagedHelmet/DamagedHelmet.gltf"); graphN.scaling = 0.5f; normalShader = true;
		//scaling = 0.5f;
		//graphN.LoadGLTF("../resources/gltf/FlightHelmet/FlightHelmet.gltf"); scaling = 2.0f;
		//graphN.LoadGLTF("../resources/gltf/NormalTangentMirrorTest/NormalTangentMirrorTest.gltf"); scaling = 0.5f;



		//Setting up shaders
		std::string vertexShaderString;
		std::string fragmentShaderString;

		//Direction light shader
		vertexShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/DeferredLightPassDirVertex.glsl");
		fragmentShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/DeferredLightPassDirFragment.glsl");
		graphN.shadR->vertexShader = graphN.shadR->SetupShader(vertexShaderString.c_str(), GL_VERTEX_SHADER);
		graphN.shadR->fragmentShader = graphN.shadR->SetupShader(fragmentShaderString.c_str(), GL_FRAGMENT_SHADER);
		graphN.shadR->shaderProgram = std::make_shared<GLuint>(graphN.shadR->CreateProgram(graphN.shadR->vertexShader, graphN.shadR->fragmentShader));
		fBuf.lBufferDirShaderProgram = graphN.shadR->shaderProgram;
		//Point light shader
		vertexShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/DeferredLightPassPointVertex.glsl");
		fragmentShaderString = graphN.shadR->LoadShaderFromFile("../resources/shaders/DeferredLightPassPointFragment.glsl");
		graphN.shadR->vertexShader = graphN.shadR->SetupShader(vertexShaderString.c_str(), GL_VERTEX_SHADER);
		graphN.shadR->fragmentShader = graphN.shadR->SetupShader(fragmentShaderString.c_str(), GL_FRAGMENT_SHADER);
		graphN.shadR->shaderProgram = std::make_shared<GLuint>(graphN.shadR->CreateProgram(graphN.shadR->vertexShader, graphN.shadR->fragmentShader));
		fBuf.lBufferPointShaderProgram = graphN.shadR->shaderProgram;

		BlinnPhongMaterial bpMaterial = BlinnPhongMaterial(graphN.shadR->shaderProgram);
		bpMaterial.shininess = 10.0f;
		bpMaterial.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
		bpMaterial.diffuse = { 1.0f,1.0f,1.0f,1.0f };
		bpMaterial.specular = { 1.0f,1.0f,1.0f,1.0f }; //This value is ignored in the end as the results are much more visually pleasing if the specular highlight corresponds to the colour of the light instead
		std::shared_ptr<BlinnPhongMaterial> bpMaterialPointer = std::make_shared<BlinnPhongMaterial>(bpMaterial);

		//Assign Blinn Phong Material to all primitives of all meshes
		for (int i = 0; i < tempNodes.size(); i++) {
			for (int j = 0; j < tempNodes[i]->meshVector.size(); j++) {
				MeshResource* mesh = tempNodes[i]->meshVector[j].get();
				for (int k = 0; k < mesh->primitiveVector.size(); k++) {
					mesh->primitiveVector[k].material = bpMaterialPointer;
				}
			}
		}

		//Add nodes to vector
		GLuint rows = 20;
		GLuint columns = 20;
		for (int i = 0; i < rows; i++) {	
			for (int j = 0; j < columns; j++) {
				int rowPos = i - (rows / 2);
				int colPos = j - (columns / 2);

				if (j % 3) {
					GraphicsNode avocado = *tempNodes[0].get();
					avocado.position = vec3(rowPos, 0, colPos);
					if (i % 2)
						avocado.scaling -= 3.0f; //Change scaling for increased visual interest
					else if (i % 3)
						avocado.scaling += 3.0f;
					nodeVector.push_back(std::make_shared<GraphicsNode>(avocado));
				}
				else if (j % 2) {
					GraphicsNode flightHelm = *tempNodes[2].get();
					flightHelm.position = vec3(rowPos, 0, colPos);
					if (i % 2)
						flightHelm.scaling += 0.5f;
					else if (i % 3)
						flightHelm.scaling -= 0.5f;
					nodeVector.push_back(std::make_shared<GraphicsNode>(flightHelm));
				}
				else {
					GraphicsNode damagedHelm = *tempNodes[1].get();
					damagedHelm.position = vec3(rowPos, 0, colPos);
					if (i % 2)
						damagedHelm.scaling -= 0.5f;
					else if (i % 3)
						damagedHelm.scaling += 0.5f;
					nodeVector.push_back(std::make_shared<GraphicsNode>(damagedHelm));

				}

			}
		}

		//Create Light Manager
		LightManager lightMan;
		lightMan.SetUp();
		this->lightMan = lightMan;

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

	ShaderResource shadR;
	MeshResource fullScreenQuad;
	fullScreenQuad.SetupFullScreenQuad(2, 2); //Sets up quad between -1  and 1, width 2, height 2


	float counter = 0.0f;

	Camera camera = Camera(this->windW, this->windH, {10.0f,5.0f,10.0f});
	camera.projection = camera.ProjectionMatrix(1.0f, 0.1f, 2000.0f);

	ShaderResource geoPassShadR;
	geoPassShadR.vertexShader = geoPassShadR.SetupShader(geoPassShadR.LoadShaderFromFile("../resources/shaders/DeferredGeoPassVertex.glsl").c_str(), GL_VERTEX_SHADER);
	geoPassShadR.fragmentShader = geoPassShadR.SetupShader(geoPassShadR.LoadShaderFromFile("../resources/shaders/DeferredGeoPassFragment.glsl").c_str(), GL_FRAGMENT_SHADER);
	geoPassShadR.shaderProgram = std::make_shared<GLuint>(geoPassShadR.CreateProgram(geoPassShadR.vertexShader, geoPassShadR.fragmentShader));
	fBuf.gBufferShaderProgram = geoPassShadR.shaderProgram;

	DefaultMaterial defMat = DefaultMaterial();
	lightMan.sphere->primitiveVector[0].material = std::make_shared<DefaultMaterial>(defMat);

	//Set Up all lights
	DirLight dirLight = DirLight();
	dirLight.direction = { 1.0f,-1.0f,0.0f }; //Pointing down
	dirLight.colour = { 1.0f,1.0f,1.0f,1.0f };
	dirLight.intensity = 0.5f;
	
	lightMan.NewDirLight(dirLight);

	//Setting up Point Lights
	PointLight pointLight = PointLight();
	pointLight.pos = {-1,0,0};
	pointLight.intensity = 0.1f;
	pointLight.colour = { 0,0,1,1 };

	GLuint rows = 20;
	GLuint columns = 20;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			int rowPos = i - (rows / 2);
			int colPos = j - (columns / 2);
			pointLight.pos = vec3(rowPos+0.5, 0, colPos+0.5);

			//Change colour of lights for more visual interest
			if (i % 3) {
				pointLight.colour = vec4(0,1,0, 1);
				if (i % 2)
					pointLight.colour = vec4(1, 0, 1, 1);
			}
			else if (i%2) {
				pointLight.colour = vec4(1, 0, 0, 1);
			}
			else {
				pointLight.colour = vec4(0, 0, 1, 1);
			}

			//Change height of lights for more visual interest
			if (j % 2) {
				pointLight.pos.y = 1;
				if (i % 2)
					pointLight.pos.y = 0;
			}
			else {
				pointLight.pos.y = 0;
				if (i % 2)
					pointLight.pos.y = 1;
			}
			lightMan.AddPointLight(pointLight);
		}
	}



	while (this->window->IsOpen())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fBuf.gBuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		

		
		//Update camera view
		camera.view = inverse(camera.ViewMatrix({ 0,0,0 })); 
		mat4 vp = camera.projection * camera.view;

		//Geometry pass
		for (int i = 0; i < nodeVector.size(); i++) {
			nodeVector[i]->DrawTrisDeferred(camera.view, camera.projection, nodeVector[i]->scaling, fBuf.gBufferShaderProgram);
			//Apply processed input to object
			inputHandler.ApplyInput(nodeVector[i].get());
		}
		lightMan.DrawPointLights(camera.view, camera.projection); //Uncomment to show debug point light spheres

		//Lighting pass
		//Set up all textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fBuf.gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fBuf.gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, fBuf.gAlbedo);

		//Point light pass
		shadR.UseProgram(fBuf.lBufferPointShaderProgram);
		shadR.EditUniform1i(fBuf.lBufferPointShaderProgram, "gPos", 0);
		shadR.EditUniform1i(fBuf.lBufferPointShaderProgram, "gNormal", 1);
		shadR.EditUniform1i(fBuf.lBufferPointShaderProgram, "gAlbedo", 2);
		shadR.EditUniform3fv(fBuf.lBufferPointShaderProgram, "camPos", camera.position);
		shadR.EditUniformMat4fv(fBuf.lBufferPointShaderProgram, "viewProjection", vp);
		shadR.UseProgram(0);

		glBindFramebuffer(GL_FRAMEBUFFER, fBuf.lBuffer);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		std::shared_ptr<Material> bpMaterial = nodeVector[0].get()->meshVector[0]->primitiveVector[0].material;
		bpMaterial->ApplyMaterial(fBuf.lBufferPointShaderProgram);
		lightMan.ApplyDeferredPointLight(fBuf.lBufferPointShaderProgram, camera.view, camera.projection);


		//Dir light
		TextureResource textR;
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, fBuf.lTex);
		shadR.UseProgram(fBuf.lBufferDirShaderProgram);
		shadR.EditUniform1i(fBuf.lBufferDirShaderProgram, "pointLightResults", 3);
		shadR.EditUniform1i(fBuf.lBufferDirShaderProgram, "gPos", 0);
		shadR.EditUniform1i(fBuf.lBufferDirShaderProgram, "gNormal", 1);
		shadR.EditUniform1i(fBuf.lBufferDirShaderProgram, "gAlbedo", 2);
		shadR.EditUniform3fv(fBuf.lBufferDirShaderProgram, "camPos", camera.position);
		shadR.EditUniformMat4fv(fBuf.lBufferDirShaderProgram, "viewProjection", vp);
		shadR.UseProgram(0);


		//Final draw
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		bpMaterial->ApplyMaterial(fBuf.lBufferDirShaderProgram);
		lightMan.ApplyDeferredDirLight(fBuf.lBufferDirShaderProgram);

		fullScreenQuad.DrawFullScreenQuad();
		shadR.UseProgram(0);

		this->window->SwapBuffers();

		if (inputHandler.SpaceToggle()) {
			counter += 0.05f;
			lightMan.UpdatePositions(sin(counter)/10,cos(counter*2)/10, cos(counter)/10);
			//lightMan.UpdatePositions(0, cos(counter * 2), 0);

			camera.position = { (sin(counter/10) * 5), camera.position.y, (-cos(counter/10) * 5) };
		}


#ifdef CI_TEST
		// if we're running CI, we want to return and exit the application after one frame
		// break the loop and hopefully exit gracefully
		break;
#endif
	}
}


} // namespace Example