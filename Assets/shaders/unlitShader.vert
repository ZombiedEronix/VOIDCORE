#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 instanceModelMatrix;

layout (std140, binding = 0) uniform CameraBlock
{
    mat4 projection;
    mat4 view;
    float time;
    float deltaTime;
};

out vec3 Normal;
out vec2 texCoord;
out vec3 FragPos;

void main() {
    
    texCoord = aTex;
    Normal = mat3(transpose(inverse(instanceModelMatrix))) * aNormal;
    vec4 worldPos = instanceModelMatrix * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    vec4 viewPos = view * worldPos;
    float dist = -viewPos.z;
    gl_Position = projection * view * worldPos;
}