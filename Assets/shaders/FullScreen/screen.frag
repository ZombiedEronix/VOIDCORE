#version 450 core
out vec4 FragColor;
in vec2 TexCoords;

layout(binding = 0) uniform sampler2D u_SceneTex; 
layout(binding = 1) uniform sampler2D u_UITex; 


void main() {
    vec4 scene = texture(u_SceneTex, TexCoords);
    vec4 ui = texture(u_UITex, TexCoords);
    FragColor = mix(scene, ui, ui.a);
}
