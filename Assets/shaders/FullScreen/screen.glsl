#section vertex
#version 450 core
out vec2 TexCoords;

void main() {
    float x = -1.0 + float((gl_VertexID & 1) << 2);
    
    float y = -1.0 + float((gl_VertexID & 2) << 1);
    
    TexCoords = vec2(x * 0.5 + 0.5, y * 0.5 + 0.5);

    gl_Position = vec4(x, y, 0.0, 1.0);
}


#section fragment
#version 450 core
out vec4 FragColor;
in vec2 TexCoords;

layout(binding = 0) uniform sampler2D u_SceneTex; 
layout(binding = 1) uniform sampler2D u_UITex; 


void main() {
    vec4 scene = texture(u_SceneTex, TexCoords);
    vec4 ui = texture(u_UITex, TexCoords);
    FragColor = mix(scene, ui, ui.a);
}
