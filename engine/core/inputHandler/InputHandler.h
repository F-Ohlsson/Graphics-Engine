#pragma once
#include "config.h"
#include "core/app.h"
#include "render/window.h"
#include "core/graphicsNode/GraphicsNode.h"
#include "core/mathlib/mat4.h"
#include "core/mathlib/vec3.h"
#include <iostream>


class InputHandler {
public:
	InputHandler();
	~InputHandler();

	void CheckKeyboardInput(int32 key, int32 action);
	void CheckMouseInput(int32 button, int32 action);
	void CheckMouseMovement(float64 x, float64 y);
	void UpdateMousePosition();
	void ApplyInput(GraphicsNode* graphN);
	bool SpaceToggle();

private:
	bool moveNorth = false;
	bool moveWest = false;
	bool moveSouth = false;
	bool moveEast = false;
	bool rotate = false;
	bool spacePress = false;
	bool toggle = false;
	vec3 previousMouseCoord = { 0,0,0 };
	vec3 currentMouseCoord = { 0,0,0 };
};