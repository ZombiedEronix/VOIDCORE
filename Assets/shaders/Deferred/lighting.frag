#version 450 core
#include "../Common.glsl"

out vec4 FragColor;
in vec2 TexCoords;

layout(binding = 0) uniform sampler2D AlbedoTexture;
layout(binding = 1) uniform sampler2D NormalTexture;
layout(binding = 2) uniform sampler2D DepthTexture;

struct PointLight {
    vec4 positionAndRadius;
    vec4 colorAndIntensive;
    vec4 rotationAndType;
};

#define MAX_LIGHTS 64

layout(std140, binding = 1) uniform LightBlock {
    PointLight U_Lights[MAX_LIGHTS];
    int U_LightCount;
};

void main() {
    vec3 albedo = texture(AlbedoTexture, TexCoords).rgb;
    vec3 packedNormal = texture(NormalTexture, TexCoords).rgb;
    float rawDepth = texture(DepthTexture, TexCoords).r;

    if(rawDepth >= 1.0) {
        FragColor = vec4(albedo, 1.0);
        return;
    }

    vec3 normal = normalize(packedNormal * 2.0 - 1.0);
    vec3 camPos = U_InvProjection[3].xyz;

    vec4 clipPos = vec4(TexCoords * 2.0 - 1.0, rawDepth * 2.0 - 1.0, 1.0);
    vec4 viewPos = U_InvProjection * clipPos;
    viewPos /= viewPos.w;
    vec3 fragPos = (U_InvView * viewPos).xyz;

    vec3 viewDir = normalize(camPos - fragPos);

    vec3 ambient = vec3(0.02) * albedo;
    vec3 accumulatedLighting = vec3(0.0);

    int activeLights = min(U_LightCount, MAX_LIGHTS);

    for(int i = 0; i < activeLights; i++) {
        vec3 lightPos = U_Lights[i].positionAndRadius.xyz;
        float lightRadius = U_Lights[i].positionAndRadius.w;
        vec3 lightColor = U_Lights[i].colorAndIntensive.rgb;
        float intensity = U_Lights[i].colorAndIntensive.w;

        int lightType = int(U_Lights[i].rotationAndType.w);

        vec3 lightDir;
        float attenuation = 1.0;

        switch(lightType) {
            case 0: //POINT
                lightDir = lightPos - fragPos;
                float distance = length(lightDir);

                if(distance > lightRadius)
                    continue;

                lightDir = normalize(lightDir);
                attenuation = clamp(1.0 - (distance / lightRadius), 0.0, 1.0);
                attenuation *= attenuation;
                break;

            case 1: // DIRECTIONAL LIGHT
                lightDir = normalize(U_Lights[i].rotationAndType.xyz);
                attenuation = 1.0;
                break;

            case 2: // SPOT
                lightDir = lightPos - fragPos;
                float spotDist = length(lightDir);

                if(spotDist > lightRadius)
                    continue;

                lightDir = normalize(lightDir);

                attenuation = clamp(1.0 - (spotDist / lightRadius), 0.0, 1.0);
                attenuation *= attenuation;

                vec3 lightForward = normalize(U_Lights[i].rotationAndType.xyz);

                float theta = dot(-lightDir, lightForward);

                float innerCutoff = 0.965;
                float outerCutoff = 0.906;

                float epsilon = innerCutoff - outerCutoff;
                float intensitySpot = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

                attenuation *= intensitySpot;
                break;
        }

        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * intensity;

        vec3 halfDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfDir), 0.0), 32.0);
        vec3 specular = spec * lightColor * intensity;

        accumulatedLighting += (albedo * diffuse + specular) * attenuation;
    }

    vec3 finalColor = ambient + accumulatedLighting;
    FragColor = vec4(finalColor, 1.0);
}
