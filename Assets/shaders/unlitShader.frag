#version 450 core

out vec4 FragColor;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D tex0;

struct Light
{
    vec4 position;
    vec4 color;
};

layout (std140, binding = 0) uniform CameraBlock
{
    mat4 projection;
    mat4 view;
    float time;
    float deltaTime;
    int lightCount;
    Light lights[16];
};

vec3 CalculatePointLight(Light light, vec3 normal, vec3 fragPos) {
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    float dist = length(light.position.xyz - fragPos);
    
    if (dist > light.position.w) return vec3(0.0);

    float diff = max(dot(normal, lightDir), 0.0);
    float attenuation = smoothstep(light.position.w, 0.0, dist);
    
    return light.color.rgb * diff * attenuation;
}

void main() {
    vec4 tex = texture(tex0, texCoord);
    vec3 norm = normalize(Normal);

    vec3 totalLight = vec3(0.2);

    for(int i = 0; i < lightCount; i++) {
        totalLight += CalculatePointLight(lights[i], norm, FragPos);
    }
    
    FragColor = tex * vec4(totalLight, 0.5);
}