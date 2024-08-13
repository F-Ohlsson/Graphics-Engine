#pragma once
#include "config.h"
#include "core/app.h"
#include <iostream>

class TextureResource {
public:
	GLuint texture;

	TextureResource();
	~TextureResource();
	void LoadTexture(const char* name, bool rgba);
	void BindTexture(GLuint data);
};