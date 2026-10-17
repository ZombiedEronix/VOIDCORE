#pragma once
#include <vector>
#include "Graphics/RenderPipeline/RenderCommand.h"

class Renderer;

class IRenderPass
{
public:
	virtual ~IRenderPass() = default;

	// Вызывается ДО render pass: упорядочить очередь. Данные для GPU
	// (матрицы инстансов) собираются уже по отсортированной очереди.
	virtual void Sort(std::vector<RenderCommand>& renderQueue) {}

	// Вызывается ВНУТРИ render pass: только draw'ы.
	virtual void Execute(Renderer* renderer, std::vector<RenderCommand>& renderQueue) = 0;
};
