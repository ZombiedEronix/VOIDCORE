// #version 450 core

// layout(location = 0) out vec4 OutColor;

// in vec2 TexCoords;

// layout(binding = 0) uniform sampler2D U_UiTexture;

// void main() {
//     vec4 texColor = texture(U_UiTexture, TexCoords);

//     OutColor = texColor;
// }

#version 450 core
layout(location = 0) out vec4 OutColor;
in vec2 TexCoords;

void main() {
    OutColor = vec4(1.0, 1.0, 1.0, 1.0); 
}