#version 450 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inColor;

#include "../Common.glsl"

out vec2 TexCoords;
out vec4 FragColor;

void main() {
    TexCoords = inUV;
    FragColor = inColor;

    gl_Position = U_OrthoProjection * vec4(inPos, 0.0, 1.0);
}
