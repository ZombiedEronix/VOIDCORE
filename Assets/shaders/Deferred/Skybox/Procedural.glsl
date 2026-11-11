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

in vec3 ViewDir;

uniform vec3 ZenithColor;
uniform vec3 HorizonColor;
uniform vec3 GroundColor;

void main() {
    vec3 dir = normalize(ViewDir);
    
    float y = dir.y;
    
    vec3 finalColor;
    if (y > 0.0) {
        finalColor = mix(HorizonColor, ZenithColor, y);
    } else {
        finalColor = mix(HorizonColor, GroundColor, -y);
    }
    
    FragColor = vec4(finalColor, 1.0);
}