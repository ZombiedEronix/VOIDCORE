#pragma once

class Renderer;

class IRenderPipeline
{
public:
    virtual ~IRenderPipeline() = default;

    virtual void Execute(Renderer *renderer) = 0;
};