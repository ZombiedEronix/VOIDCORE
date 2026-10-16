#pragma once
#include <string>
#include <memory>
#include "Device/IShader.hpp"

std::string get_file_contents(const char *filename);

// Тонкая обёртка над RHI-шейдером: движок не знает о конкретном API.
class Shader
{
private:
    std::unique_ptr<VoidCore::RHI::IShader> impl;

public:
    Shader(const char *vertexFile, const char *fragmentFile);

    void Load();
    void SetInt(const std::string &name, int value);
    void SetFloat(const std::string &name, float value);
    unsigned int GetID() const;
};
