#version 450 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inColor;
layout(location = 3) in vec2 inSize;
layout(location = 4) in vec4 inCorners;
layout(location = 5) in vec4 inBounds;
layout(location = 6) in vec4 inBColor;

#include "../Common.glsl"

out vec2 TexCoords;
out vec4 FragColor;
out vec2 FragSize;
out vec4 FragCorners;
out vec2 LocalPos;
out vec4 FragBorderColor;

void main() {
    TexCoords = inUV;
    FragColor = inColor;
    FragSize = inSize;
    FragCorners = inCorners;
    FragBorderColor = inBColor;

    LocalPos = (inPos - inBounds.xy) / (inBounds.zw - inBounds.xy);

    gl_Position = U_OrthoProjection * vec4(inPos, 0.0, 1.0);
}
