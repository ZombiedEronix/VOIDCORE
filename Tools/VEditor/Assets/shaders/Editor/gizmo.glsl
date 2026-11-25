#section vertex
#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

#include "Common.glsl"

layout(location = 0) out vec4 vColor;


void main() {
    vColor = aColor;
    gl_Position = U_Projection * U_View * vec4(aPos, 1.0);
}

#section fragment
#version 450 core

layout(location = 0) in vec4 vColor;
layout(location = 0) out vec4 FragColor;

void main() {
    FragColor = vColor;
}
