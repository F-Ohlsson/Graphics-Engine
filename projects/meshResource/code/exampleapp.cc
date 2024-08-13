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
"layout(location=0) out vec4 Color;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"	gl_Position = transform * vec4(pos, 1);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	// empty
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
	window->SetKeyPressFunction([this](int32, int32, int32, int32)
	{
		this->window->Close();
	});

	if (this->window->Open())
	{
		this->mR = MeshResource();

		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//this->vertexShader = mR.SetupShader(vs, GL_VERTEX_SHADER);
		//this->pixelShader = mR.SetupShader(ps, GL_FRAGMENT_SHADER);
		

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

		mR.SetupCube(1,1,1);
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
		glDisable(GL_DEPTH_TEST);
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
	float radians = 0;
	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();


		mat4 transform = mat4();
		//translation matrix
		transform.m[0].z = sin(radians);

		//apply translation before rotation (right to left)
		transform = rotationz(radians) * transform;


		glUseProgram(this->program);
		int uniformLocation = glGetUniformLocation(this->program, "transform");
		glUniformMatrix4fv(uniformLocation, 1, GL_TRUE, &transform[0][0]);

		mR.DrawQuads();
		this->window->SwapBuffers();

		radians += 0.01f;
		//remove 2*pi if larger to prevent counter from expanding infinitely
		if (radians > (atan(1) * 8)) 
			radians -= float((atan(1) * 8));
		
#ifdef CI_TEST
		// if we're running CI, we want to return and exit the application after one frame
		// break the loop and hopefully exit gracefully
		break;
#endif
	}
}

} // namespace Example