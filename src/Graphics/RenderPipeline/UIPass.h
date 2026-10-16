// #pragma once
// #include "IRenderPass.h"
// #include "RenderCommand.h"

// #include "Device/IVAO.hpp"
// #include "Device/IVBO.hpp"
// #include "Device/IShader.hpp"
// class UIPass : public IRenderPass
// {
// private:
//     std::unique_ptr<VoidCore::RHI::IVAO> uiVAO;
//     std::unique_ptr<VoidCore::RHI::IVBO> uiVBO;
//     std::unique_ptr<VoidCore::RHI::IShader> uiShader;

// public:
//     UIPass();
//     void Execute(Renderer *renderer, std::vector<RenderCommand> &uiQueue) override;
// };