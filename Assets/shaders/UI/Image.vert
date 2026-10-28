#version 450 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;

#include "../Common.glsl"

out vec2 TexCoords;

void main() {
    TexCoords = inUV;

    gl_Position = U_OrthoProjection * vec4(inPos, 0.0, 1.0);
}
