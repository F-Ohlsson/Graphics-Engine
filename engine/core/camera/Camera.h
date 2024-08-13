#pragma once
#include"config.h"
#include "core/app.h"
#include "core/mathlib/vec3.h"
#include "core/mathlib/vec4.h"
#include "core/mathlib/mat4.h"

class Camera {
public:
	vec3 position = vec3();
	mat4 view = mat4();
	mat4 projection = mat4();

	float rotation = 0.0f;

	GLuint height;
	GLuint width;

	float speed = 1.0f;

	Camera(GLuint width, GLuint height, vec3 position);
	mat4 ProjectionMatrix(float FOVrad, float nearPlane, float farPlane);
	mat4 ViewMatrix(vec3 direction);
	mat4 LookAt(vec3 direction);
};