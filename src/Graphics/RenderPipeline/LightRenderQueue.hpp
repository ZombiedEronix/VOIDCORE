#include "Graphics/RenderPipeline/LightCommand.hpp"

class LightRenderQueue
{
    std::vector<LightInfo> m_Queue;

    public:
    std::vector<LightInfo>& GetData();
    void AddLight(LightInfo& info);
    void Clear();
};