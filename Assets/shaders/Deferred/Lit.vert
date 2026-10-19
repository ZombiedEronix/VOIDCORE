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