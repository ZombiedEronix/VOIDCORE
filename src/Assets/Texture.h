#pragma once
#include "glad/glad.h"

class Texture
{
public:
	GLuint texID;
	int widthTexture, heightTexture, colorch;
	char filename[];

	Texture(const char* filename, GLint internalFormat);
	void Bind();
	void Unbind();
};

