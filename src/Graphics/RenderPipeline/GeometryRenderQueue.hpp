#pragma once
#include <vector>
#include "GeometryRenderCommand.h"

class Renderer;

class GeometryRenderQueue
{
private:
    std::vector<GeometryRenderCommand> m_Queue;

public:
    GeometryRenderQueue() = default;
    ~GeometryRenderQueue() = default;

    void AddCommand(const GeometryRenderCommand &command);

    const std::vector<GeometryRenderCommand> &GetData() const;
    
    void Sort();

    void Execute(Renderer *renderer);

    void Clear();

    bool IsEmpty() const { return m_Queue.empty(); }

    size_t GetCount() const { return m_Queue.size(); }
};
