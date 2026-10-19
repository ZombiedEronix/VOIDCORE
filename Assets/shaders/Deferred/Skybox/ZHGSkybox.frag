#version 450 core
out vec4 FragColor;


//CAMERA !!!
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