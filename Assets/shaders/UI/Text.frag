#version 450 core

layout(location = 0) out vec4 OutColor;

in vec2 TexCoords;
in vec4 FragColor;

layout(binding = 0) uniform sampler2D U_UiTexture;

void main() {
    vec4 texColor = texture(U_UiTexture, TexCoords);
    vec4 bgColor = texColor * FragColor;

    OutColor = bgColor;

    if(OutColor.a < 0.001)
        discard;
}
