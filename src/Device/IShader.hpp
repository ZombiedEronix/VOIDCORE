#pragma once
#include <string>

namespace VoidCore::RHI
{
    class IShader
    {
    public:
        virtual ~IShader() = default;

        virtual void Use() = 0;
        virtual void SetInt(const std::string &name, int value) = 0;
        virtual void SetFloat(const std::string &name, float value) = 0;
        virtual unsigned int GetID() const = 0;
    };
}
