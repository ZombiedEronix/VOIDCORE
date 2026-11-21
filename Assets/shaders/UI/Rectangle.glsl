#section vertex
#version 450 core

#include "UIBase.glsl"

layout(location = 0) out vec2 TexCoords;
layout(location = 1) out vec4 FragRect;
layout(location = 2) out vec2 LocalPos;

void main() {
    TexCoords = inUV;
    FragRect = inRect;
    LocalPos = inPos - inRect.xy;

    gl_Position = UI_TransformPosition(inPos);
}

#section fragment
#version 450 core

layout(std140, binding = 1) uniform MaterialData {
    vec4 Background;
};

layout(location = 0) out vec4 OutColor;

layout(location = 0) in vec2 TexCoords;
layout(location = 1) in vec4 FragRect;
layout(location = 2) in vec2 LocalPos;

void main() {
    OutColor = Background;
}
