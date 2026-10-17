#pragma once
#include <string>
#include "Device/RHI.hpp"

std::string get_file_contents(const char *filename);

// Пара стадий (VS + FS). Сам по себе ничего не рисует: из него Material
// собирает IPipeline. Никаких SetInt/SetFloat по имени — параметры идут через
// uniform-буферы и текстурные слоты (binding задаётся в самом шейдере).
class Shader
{
private:
    VoidCore::RHI::Ref<VoidCore::RHI::IShader> vs;
    VoidCore::RHI::Ref<VoidCore::RHI::IShader> fs;

public:
    Shader(const char *vertexFile, const char *fragmentFile);

    VoidCore::RHI::IShader *GetVS() const { return vs.get(); }
    VoidCore::RHI::IShader *GetFS() const { return fs.get(); }
    bool IsValid() const { return vs && fs; }
};
