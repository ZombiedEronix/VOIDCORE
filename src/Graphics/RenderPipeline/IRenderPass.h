#pragma once
#include <vector>
#include "Graphics/RenderPipeline/RenderCommand.h"

class Renderer;

class IRenderPass
{
public:
	virtual void Execute(Renderer* renderer, std::vector<RenderCommand>& renderQueue) = 0;
};