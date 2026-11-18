#section vertex
#version 450 core
#include "Common.glsl"

layout(location = 0) out vec3 ViewDir;

void main() {
    float x = -1.0 + float((gl_VertexID & 1) << 2);
    float y = -1.0 + float((gl_VertexID & 2) << 1);

    vec4 clipPos = vec4(x, y, 1.0, 1.0);
    vec4 viewPos = U_InvProjection * clipPos;
    viewPos = vec4(viewPos.xy, -1.0, 0.0);
    ViewDir = (U_InvView * viewPos).xyz;

    gl_Position = vec4(x, y, 0.9999, 1.0);
}

#section fragment
#version 450 core
layout(location = 0) out vec4 FragColor;
layout(location = 0) in vec3 ViewDir;

layout(std140, binding = 1) uniform MaterialData {
    vec4 ZenithColor;
    vec4 HorizonColor;
    vec4 GroundColor;
};

void main() {
    vec3 dir = normalize(ViewDir);
    float y = dir.y;

    vec3 finalColor;
    if (y > 0.0) {
        finalColor = mix(HorizonColor.rgb, ZenithColor.rgb, y);
    } else {
        finalColor = mix(HorizonColor.rgb, GroundColor.rgb, -y);
    }

    FragColor = vec4(finalColor, 1.0);
}
