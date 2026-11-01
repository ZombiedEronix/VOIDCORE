#version 450 core
#include "UIBase.glsl"

out vec2 TexCoords;

void main() {
    TexCoords = inUV;
    gl_Position = UI_TransformPosition(inPos);
}
