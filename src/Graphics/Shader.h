#pragma once
#include <string>
#include <fstream>
#include <glm.hpp>
#include <glad.h>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <Core/Debug.h>

std::string get_file_contents(const char *filename);

class Shader
{
private:
	private:
	int GetUniformLocation(const std::string &name);
	std::unordered_map<std::string, int> uniformCache;
public:
	unsigned int ID;
	Shader(const char *vertexfile, const char *fragmentFile);
	void Load();
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void Delete();
};