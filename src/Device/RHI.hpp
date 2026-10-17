#pragma once
// Umbrella-заголовок. Движок включает только его (или IRenderDevice.hpp).
//
// ===================== Конвенции RHI (одинаковы для всех API) =====================
//  Глубина      NDC z ∈ [0,1]                  → GLM_FORCE_DEPTH_ZERO_TO_ONE
//  Матрицы      glm column-major, mul(M, v)     (в HLSL — column_major / transpose)
//  UBO          layout std140 (в HLSL — cbuffer, те же правила упаковки)
//  Слоты        Uniform buffers: 0..7   (Vulkan: set = 0, binding = slot)
//               Textures+Samplers: 0..15 (Vulkan: set = 1, binding = slot)
//               слот 0 UBO — зарезервирован под «Frame/Camera» блок
//  Вершины      location = layout(location = N); слот 0 — меш, слот 1 — инстансы
//  Шейдеры      исходник один (GLSL 450), дальше glslang → SPIR-V → SPIRV-Cross
//               → GLSL/HLSL/MSL (см. MIGRATION.md)
// ===================================================================================
#include "Device/RHITypes.hpp"
#include "Device/IRenderDevice.hpp"
#include "Device/RenderDevice.hpp"
