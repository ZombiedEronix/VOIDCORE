
#include "../Common.glsl"
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inRect;

vec4 UI_TransformPosition(vec2 pos) {
    return U_OrthoProjection * vec4(pos, 0.0, 1.0);
}

bool UI_AlphaTest(float alpha) {
    return (alpha < 0.001);
}