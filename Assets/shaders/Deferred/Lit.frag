#version 450 core

layout (location = 0) out vec4 gAlbedo;
layout (location = 1) out vec4 gNormal;

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

layout(binding = 0) uniform sampler2D AlbedoTexture;
layout(binding = 1) uniform sampler2D NormalTexture;

void main() {
    vec4 tex = texture(AlbedoTexture, texCoord);
    
    vec3 norm = normalize(Normal);
    
    vec3 packedNormal = norm * 0.5 + 0.5;
    
    gAlbedo = tex; 
    gNormal = vec4(packedNormal, 1.0);
}
