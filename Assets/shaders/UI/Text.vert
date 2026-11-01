#version 450 core

#include "../Common.glsl"
layout(location = 2) in vec4 inColor;

out vec2 TexCoords;
out vec4 FragColor;

void main() {
    TexCoords = inUV;
    FragColor = inColor;

    gl_Position = UI_TransformPosition(inPos);
}
