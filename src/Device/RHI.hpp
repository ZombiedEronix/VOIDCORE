#pragma once
// Umbrella header. The engine includes only this (or IRenderDevice.hpp).
//
// ===================== RHI conventions (same for all APIs) =====================
// NDC depth z ∈ [0,1] → GLM_FORCE_DEPTH_ZERO_TO_ONE
// Matrices glm column-major, mul(M, v) (in HLSL - column_major / transpose)
// UBO layout std140 (in HLSL — cbuffer, same packing rules)
// Слоты        Uniform buffers: 0..7   (Vulkan: set = 0, binding = slot)
//               Textures+Samplers: 0..15 (Vulkan: set = 1, binding = slot)
// slot 0 UBO - reserved for “Frame/Camera” block
// Vertices location = layout(location = N); slot 0 - mesh, slot 1 - instances
// Shaders source one (GLSL 450), then glslang → SPIR-V → SPIRV-Cross
// → GLSL/HLSL/MSL (см. MIGRATION.md)
// ===================================================================================
#include "Device/RHITypes.hpp"
#include "Device/IRenderDevice.hpp"
#include "Device/RenderDevice.hpp"
