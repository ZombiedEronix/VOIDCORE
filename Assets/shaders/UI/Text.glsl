#section vertex
#version 450 core
#include "UIBase.glsl"

layout(location = 0) out vec2 TexCoords;

void main() {
    TexCoords = inUV;
    gl_Position = UI_TransformPosition(inPos);
}

#section fragment
#version 450 core

layout(location = 0) in vec2 TexCoords;
layout(location = 0) out vec4 OutColor;
layout(binding = 0) uniform sampler2D uFontAtlas;

void main() {
    float alpha = texture(uFontAtlas, TexCoords).a;
    OutColor = vec4(1.0, 1.0, 1.0, alpha);
}
