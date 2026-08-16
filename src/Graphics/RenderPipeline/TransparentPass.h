#pragma once
#include "IRenderPass.h"
#include <vector>

class Renderer;

class TransparentPass : public IRenderPass
{
public:
    void Execute(Renderer* renderer, std::vector<RenderCommand>& renderQueue) override;
};