#section vertex
#version 450 core

#include "UIBase.glsl"

layout(location = 3) in vec4 inColor;
layout(location = 4) in vec4 inCornerSize;
layout(location = 5) in vec4 inBorderColor;
layout(location = 6) in vec4 inBorderThickness;

layout(location = 0) out vec2 TexCoords;
layout(location = 1) out vec4 FragColor;
layout(location = 2) out vec4 FragRect;
layout(location = 3) out vec4 FragCorners;
layout(location = 4) out vec2 LocalPos;
layout(location = 5) out vec4 FragBorderColor;
layout(location = 6) out vec4 FragBorderThickness;

void main() {
    TexCoords = inUV;
    FragColor = inColor;
    FragRect = inRect;
    FragCorners = inCornerSize;
    FragBorderColor = inBorderColor;
    FragBorderThickness = inBorderThickness;

    LocalPos = inPos - inRect.xy;

    gl_Position = UI_TransformPosition(inPos);
}

#section fragment
#version 450 core

layout(location = 0) out vec4 OutColor;

layout(location = 0) in vec2 TexCoords;
layout(location = 1) in vec4 FragColor;
layout(location = 2) in vec4 FragRect;
layout(location = 3) in vec4 FragCorners;
layout(location = 4) in vec2 LocalPos;
layout(location = 5) in vec4 FragBorderColor;
layout(location = 6) in vec4 FragBorderThickness;

float roundedBoxSDF(vec2 p, vec2 b, vec4 r) {
    vec2 q = abs(p) - b;
    float radius = (p.x > 0.0) ? ((p.y > 0.0) ? r.z : r.y) : ((p.y > 0.0) ? r.w : r.x);
    return length(max(q + radius, 0.0)) - radius;
}

void main() {
    vec4 bgColor = FragColor;

    vec2 size = FragRect.zw;
    vec2 halfSize = size * 0.5;
    vec2 pixelPos = LocalPos - halfSize;

    if(FragCorners.x > 0.0 || FragCorners.y > 0.0 || FragCorners.z > 0.0 || FragCorners.w > 0.0) {

        float distance = roundedBoxSDF(pixelPos, halfSize, FragCorners);
        float outerAlpha = smoothstep(1.0, -1.0, distance);

        if(FragBorderColor.a > 0.0) {
            float borderThickness = FragBorderThickness.x;

            float innerDistance = distance + borderThickness;
            float borderFactor = smoothstep(-1.0, 1.0, innerDistance);

            vec4 finalC = mix(bgColor, FragBorderColor, borderFactor);
            finalC.a *= outerAlpha;
            OutColor = finalC;
        } else {
            bgColor.a *= outerAlpha;
            OutColor = bgColor;
        }
    } else {
        OutColor = bgColor;
    }

    if(OutColor.a < 0.001)
        discard;
}
