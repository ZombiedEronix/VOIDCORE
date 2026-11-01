#version 450 core

#include "UIBase.glsl"

out vec4 FragColor;
out vec4 FragRect;
out vec4 FragCorners;
out vec2 LocalPos;
out vec4 FragBorderColor;
out vec4 FragBorderThickness;

void main() {
    TexCoords = inUV;
    FragColor = inColor;
    FragRect = inRect;
    FragCorners = inCornerSize;
    FragBorderColor = inBorderColor;
    FragBorderThickness = inBorderThickness;

    LocalPos = inPos - inRect.xy;

    gl_Position = U_OrthoProjection * vec4(inPos, 0.0, 1.0);
}

