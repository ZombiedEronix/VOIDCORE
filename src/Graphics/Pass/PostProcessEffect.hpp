#pragma once

#include "Device/ICommandList.hpp"

namespace VoidCore::Rendering
{
    class PostProcessEffect
    {
    public:
        virtual ~PostProcessEffect() = default;
        virtual void Init() = 0;
        virtual void Apply(RHI::ICommandList *cmd, RHI::ITexture *source, RHI::ITexture *destination) = 0;
    };
}