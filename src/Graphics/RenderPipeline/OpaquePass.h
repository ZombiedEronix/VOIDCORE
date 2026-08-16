#pragma once
#include "IRenderPass.h"
#include <vector>

class Renderer;

class OpaquePass : public IRenderPass
{
public:
    void Execute(Renderer* renderer, std::vector<RenderCommand>& renderQueue) override;
};