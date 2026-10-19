#version 450 core
#include "../Common.glsl"

layout(location = 0) out vec4 fragColor;

// 1. Функция расстояния (SDF) для сферы
float sdSphere(vec3 p, float r) {
    return length(p) - r;
}

float map(vec3 p) {
    vec3 repeatedPos = mod(p + vec3(2.0), vec3(4.0)) - vec3(2.0);
    float wave = sin(p.x * 2.0 + U_Time) * cos(p.y * 2.0 + U_Time) * 0.15;
    return sdSphere(repeatedPos, 1.0) + wave;
}

vec3 calcNormal(vec3 p) {
    const float h = 0.001;
    const vec2 k = vec2(1.0, -1.0);
    return normalize(k.xyy * map(p + k.xyy * h) + 
                     k.yyx * map(p + k.yyx * h) + 
                     k.yxy * map(p + k.yxy * h) + 
                     k.xxx * map(p + k.xxx * h));
}

void main() {
    vec2 resolution = vec2(1920.0, 1080.0); 
    vec2 uv = gl_FragCoord.xy / resolution;
    vec2 screenPos = uv * 2.0 - vec2(1.0);
    
    mat4 invU_View = inverse(U_View);
    vec3 rayOrigin = vec3(invU_View[3].xyz);
    
    mat4 invProj = inverse(U_Projection);
    vec4 target = invProj * vec4(screenPos, 1.0, 1.0);
    vec3 targetNormalized = normalize(target.xyz / target.w); 
    vec3 rayDir = vec3(invU_View * vec4(targetNormalized, 0.0));

    float t = 0.0;
    int maxSteps = 80;
    float maxDist = 40.0;
    bool hit = false;
    
    for(int i = 0; i < maxSteps; i++) {
        vec3 currentPos = rayOrigin + rayDir * t;
        float dist = map(currentPos);
        
        if(dist < 0.001) {
            hit = true;
            break;
        }
        
        t += dist;
        if(t > maxDist) break;
    }
    
    vec3 finalColor = vec3(0.0);

    if(hit) {
        vec3 hitPos = rayOrigin + rayDir * t;
        vec3 normal = calcNormal(hitPos);
        
        vec3 lightDir = normalize(vec3(1.0, 1.0, -1.0));
        float diff = max(dot(normal, lightDir), 0.0);
        
        vec3 objectColor = vec3(0.5) + vec3(0.5) * cos(U_Time + hitPos.xyz + vec3(0.0, 2.0, 4.0));
        vec3 ambient = vec3(0.05);
        
        float fog = 1.0 / (1.0 + t * t * 0.01);
        
        finalColor = (objectColor * diff + ambient) * fog;
    } else {
        finalColor = vec3(0.02, 0.02, 0.05) * (1.0 - screenPos.y);
    }

    fragColor = vec4(finalColor, 1.0);
}
