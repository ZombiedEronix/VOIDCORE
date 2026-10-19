#version 450 core
#include "../Common.glsl"

out vec4 FragColor;
in vec2 TexCoords;

void main() {
    float r = sin(TexCoords.x * 3.0 + U_Time) * 0.5 + 0.5;
    float g = cos(TexCoords.y * 3.0 - U_Time * 1.5) * 0.5 + 0.5;
    float b = sin((TexCoords.x + TexCoords.y) * 2.0 + U_Time * 2.0) * 0.5 + 0.5;

    vec3 gradient = vec3(r, g, b);
    
    vec2 uv = TexCoords * (1.0 - TexCoords.yx);
    float vignette = uv.x * uv.y * 15.0;
    gradient *= pow(vignette, 0.25);

    FragColor = vec4(gradient, 1.0); 
}
