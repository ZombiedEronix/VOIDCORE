#section vertex
#version 450 core
#include "UIBase.glsl"

out vec2 TexCoords;

void main() {
    TexCoords = inUV;
    gl_Position = UI_TransformPosition(inPos);
}


#section fragment
#version 450 core

layout(location = 0) out vec4 OutColor;

in vec2 TexCoords;

void main() {
    OutColor = vec4(1.0, 1.0, 1.0, 1.0); 
}