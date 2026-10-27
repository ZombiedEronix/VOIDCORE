#version 450 core

layout(location = 0) out vec4 OutColor;

in vec2 TexCoords;
in vec4 FragColor;
in vec2 FragSize;
in vec4 FragCorners;
in vec2 LocalPos;
in vec4 FragBorderColor;

layout(binding = 0) uniform sampler2D U_UiTexture;

float roundedBoxSDF(vec2 p, vec2 b, vec4 r) {
    vec2 q = abs(p) - b;
    float radius = (p.x > 0.0) ? ((p.y > 0.0) ? r.z : r.y) : ((p.y > 0.0) ? r.w : r.x);
    return length(max(q + radius, 0.0)) - radius;
}

void main() {
    vec4 texColor = texture(U_UiTexture, TexCoords);
    vec4 bgColor = texColor * FragColor;

    vec2 halfSize = FragSize * 0.5;
    vec2 pixelPos = (LocalPos - 0.5) * FragSize;

    if(FragCorners.x > 0.0 || FragCorners.y > 0.0 || FragCorners.z > 0.0 || FragCorners.w > 0.0) {

        float distance = roundedBoxSDF(pixelPos, halfSize, FragCorners);
        float outerAlpha = smoothstep(1.0, -1.0, distance);

        if(FragBorderColor.a > 0.0) {
            float borderThickness = 1.5;

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
