#pragma once
#include "glad.h"
#include "VBO.h"
class VAO
{
public:
	GLuint m_Id;
	VAO();
	void Bind();
	void Unbind();
	void Delete();
	void LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void LinkAttribDivisor(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset, GLuint divisor);
};

