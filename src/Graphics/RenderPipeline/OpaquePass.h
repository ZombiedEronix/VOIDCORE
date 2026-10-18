#pragma once
#include "IRenderPass.h"
#include <vector>

class Renderer;

class OpaquePass : public IRenderPass
{
public:
    void Sort(std::vector<GeometryRenderCommand>& renderQueue) override;
    void Execute(Renderer* renderer, std::vector<GeometryRenderCommand>& renderQueue) override;
};
