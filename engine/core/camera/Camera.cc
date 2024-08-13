#pragma once
#include"config.h"
#include"Camera.h"

Camera::Camera(GLuint width, GLuint height, vec3 position) {
	this->height = height;
	this->width = width;
	this->position = position;
}

mat4 Camera::ProjectionMatrix(float FOVrad, float nearPlane, float farPlane) {
	float aspectRatio = (float)height / (float)width;

	//These calculations assume a symmetrical view-cone
	float top = cosf(FOVrad * 0.5f) / sinf(FOVrad * 0.5f) / 2;
	float bottom = -top;
	float h = top - bottom;
	float w = h * aspectRatio;

	mat4 projMat = mat4(
		{w,0,0,0},
		{0,h,0,0},
		{0,0,-((farPlane+nearPlane)/(farPlane-nearPlane)), -1 },
		{0,0,-((2 * farPlane * nearPlane) / (farPlane - nearPlane)),0}
	);


	return projMat;
}

mat4 Camera::ViewMatrix(vec3 lookPoint) {
	
	vec3 forward = normalize(this->position - lookPoint);
	vec3 right = normalize(cross(vec3(0.0f, 1.0f, 0.0f), forward));
 	vec3 up = normalize(cross(forward, right));
	

	mat4 viewMat = mat4(
		{right.x,right.y,right.z,0},
		{up.x,up.y,up.z,0},
		{ forward.x,forward.y,forward.z,0},
		{ position.x,position.y,position.z,1}
	);
	
	return  viewMat;
}

mat4 Camera::LookAt(vec3 direction) {

	vec3 forward = normalize(direction);
	vec3 right = normalize(cross(vec3(0.0f, 1.0f, 0.0f), forward));

	if (isnan(right.x)) {
		if (direction.y > 0)
			right = { 1, 0, 0 };
		else
			right = { -1,0,0 };
	}

	vec3 up = normalize(cross(forward, right));


	mat4 viewMat = mat4(
		{ right.x,right.y,right.z,0 },
		{ up.x,up.y,up.z,0 },
		{ forward.x,forward.y,forward.z,0 },
		{ 0,0,0,1 }
	);

	return  viewMat;
}
