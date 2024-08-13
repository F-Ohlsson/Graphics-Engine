#pragma once
//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>


const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=2) in vec2 texCoord;\n"

"layout(location=0) out vec4 Color;\n"
"layout(location=1) out vec2 TexCoord;\n"

"uniform mat4 mvp;\n"

"void main()\n"
"{\n"
"	gl_Position =  mvp * vec4(pos, 1);\n"
"	Color = color;\n"
"	TexCoord = texCoord;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"layout(location=1) in vec2 TexCoord;\n"

"out vec4 Color;\n"

"uniform sampler2D tex;\n"

"void main()\n"
"{\n"
"	Color = texture(tex, TexCoord);\n"
"}\n";

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
	glDeleteShader(this->vertexShader);
	glDeleteShader(this->pixelShader);
	glDeleteProgram(this->program);
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

	window->SetKeyPressFunction([this](int32, int32, int32, int32)
	{
		//this->window->Close();
	});

	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//Start preparing cube resources
		this->textR = TextureResource();
		textR.LoadTexture("../../../resources/textures/STRIPES.png", true);
		textR.BindTexture(textR.texture);

		this->meshR = MeshResource();

		meshR.SetupCube(0.5, 0.5, 0.5);

		this->shadR = ShaderResource();
		this->vertexShader = shadR.SetupShader(vs, GL_VERTEX_SHADER);
		this->pixelShader = shadR.SetupShader(ps, GL_FRAGMENT_SHADER);
		

		// create a program object
		GLint shaderLogSize;
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->pixelShader);
		glLinkProgram(this->program);
		glValidateProgram(this->program);
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
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
	glEnable(GL_DEPTH_TEST);

	float radians = 0.0f;
	float radius = 2.0f;
	Render::Grid grid;
	//												Because of the postition rotation, only y value is ultimately used
	Camera camera = Camera(this->windW, this->windH, {0.0f,1.0f,0.0f});
	camera.projection = camera.ProjectionMatrix(1.0f, 0.1f, 50.0f);

	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		
		
		//Update camera view and posiition
		camera.view = inverse(camera.ViewMatrix({ 0,0,0 }));
		camera.position = { sin(radians) * radius, camera.position.y, cos(radians) * radius };

		//								Uncomment for demo-scene looking stuff
		mat4 modelMatrix = mat4()  /** rotationx(radians) * rotationz(radians)*/;
		mat4 vp = camera.projection * camera.view;
		mat4 mvp = vp * modelMatrix;

		
		//Send mvp to shader
		glUseProgram(this->program);
		int uniformLocation = glGetUniformLocation(this->program, "mvp");
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &mvp[0][0]);
		//Send texture to shader
		uniformLocation = glGetUniformLocation(this->program, "tex");
		glUniform1i(uniformLocation, 0);


		//Start drawing
		meshR.DrawQuads();
		//Only viewprojection to grid
		grid.Draw(&vp[0][0]);
		
		
		this->window->SwapBuffers();

		radians += 0.01f;
		//remove 2*pi if larger to prevent counter from expanding infinitely
		if (radians > (atan(1) * 4 * 2)) 
			radians -= float((atan(1) * 8));
		
#ifdef CI_TEST
		// if we're running CI, we want to return and exit the application after one frame
		// break the loop and hopefully exit gracefully
		break;
#endif
	}
}

} // namespace Example