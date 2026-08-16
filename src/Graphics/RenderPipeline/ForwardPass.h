#pragma once
#include "IRenderPass.h"

#include <vector>
#include <algorithm>

class Renderer;


class ForwardPass
{
public:
	void Execute(Renderer* renderer, std::vector<LightCommand>& renderQueue)
	{

	}
};