#pragma once
#include "glad.h"

class EBO
{
	public:
		GLuint m_id;
		EBO(const void* data, GLsizeiptr size);
		void Bind();
		void Unbind();
		void Delete();

		
		
};

