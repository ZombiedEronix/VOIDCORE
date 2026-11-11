#section vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 instanceModelMatrix;
#include "../Common.glsl"

out vec3 Normal;
out vec2 texCoord;
out vec3 FragPos;

void main() {
    
    texCoord = aTex;
    Normal = mat3(transpose(inverse(instanceModelMatrix))) * aNormal;


    vec4 worldPos = instanceModelMatrix * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    vec4 viewPos = U_View * worldPos;
    float dist = -viewPos.z;
    gl_Position = U_Projection * U_View * worldPos;
}

#section fragment
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
