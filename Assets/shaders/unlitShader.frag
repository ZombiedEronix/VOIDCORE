#version 450 core

out vec4 FragColor;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

layout(binding = 0) uniform sampler2D AlbedoTexture;
layout(binding = 1) uniform sampler2D NormalTexture;
layout(binding = 2) uniform sampler2D AmbientTexture;

layout (std140, binding = 0) uniform CameraBlock
{
    mat4 projection;
    mat4 view;
    float time;
    float deltaTime;
};

void main() {
    vec4 tex = texture(AlbedoTexture, texCoord);
    vec3 norm = normalize(Normal); 
    
    FragColor = tex;
}