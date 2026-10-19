#include "LightRenderQueue.hpp"

void LightRenderQueue::AddLight(LightInfo& info)
{
    m_Queue.push_back(info);
}

std::vector<LightInfo>& LightRenderQueue::GetData()
{
    return m_Queue;
}

void LightRenderQueue::Clear()
{
    m_Queue.clear();
}