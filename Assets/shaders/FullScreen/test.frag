//FROM VENTOY

#version 450 core
in vec2 TexCoords;
out vec4 FragColor;

#include "../Common.glsl"

uniform vec2 U_ScreenResolution;

vec3 bgColor = vec3(0.42, 0.01, 0.01);
vec3 rectColor = vec3(0.57, 0.04, 0.01);

const float noiseIntensity = 2.8;
const float noiseDefinition = 0.6;
const vec2 glowPos = vec2(-2., 0.);

const float total = 40.;
const float minSize = 0.03;
const float maxSize = 0.08 - minSize;
const float yDistribution = 0.5;

float random(vec2 co) {
    return fract(sin(mod(dot(co.xy, vec2(12.9898, 78.233)), 3.14159)) * 43758.5453);
}

float noise(in vec2 p) {
    p *= noiseIntensity;
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(mix(random(i + vec2(0.0, 0.0)), random(i + vec2(1.0, 0.0)), u.x), mix(random(i + vec2(0.0, 1.0)), random(i + vec2(1.0, 1.0)), u.x), u.y);
}

float fbm(in vec2 uv) {
    uv *= 5.0;
    mat2 m = mat2(1.6, 1.2, -1.2, 1.6);
    float f = 0.5000 * noise(uv);
    uv = m * uv;
    f += 0.2500 * noise(uv);
    uv = m * uv;
    f += 0.1250 * noise(uv);
    uv = m * uv;
    f += 0.0625 * noise(uv);
    uv = m * uv;

    return 0.5 + 0.5 * f;
}

vec3 bg(vec2 uv) {
    float velocity = mod(U_Time, 3600.0) / 1.6;
    float intensity = sin(uv.x * 3. + velocity * 2.) * 1.1 + 1.5;
    uv.y -= 2.;
    vec2 bp = uv + glowPos;
    uv *= noiseDefinition;

    float rb = fbm(vec2(uv.x * .5 - velocity * .03, uv.y)) * .1;
    uv += rb;

    float rz = fbm(uv * .9 + vec2(velocity * .35, 0.0));
    rz *= dot(bp * intensity, bp) + 1.2;

    float denominator = 0.1 - rz;
    if(abs(denominator) < 0.001)
        denominator = 0.001;

    vec3 col = bgColor / denominator;
    return sqrt(max(vec3(0.0), abs(col)));
}

float rectangle(vec2 uv, vec2 pos, float width, float height, float blur) {
    pos = (vec2(width, height) + .01) / 2. - abs(uv - pos);
    pos = smoothstep(0., max(blur, 0.001), pos);
    return pos.x * pos.y;
}

mat2 rotate2d(float _angle) {
    return mat2(cos(_angle), -sin(_angle), sin(_angle), cos(_angle));
}

void main() {
    float aspect = (U_ScreenResolution.x > 0.0) ? (U_ScreenResolution.x / U_ScreenResolution.y) : (16.0 / 9.0);

    vec2 uv = TexCoords * 2.0 - 1.0;
    uv.x *= aspect;

    vec3 color = bg(uv) * (2.0 - abs(uv.y * 2.0));

    float velX = -mod(U_Time, 3600.0) / 8.0;
    float velY = mod(U_Time, 3600.0) / 10.0;

    for(float i = 0.0; i < total; i++) {
        float index = i / total;
        float rnd = random(vec2(index, 1.0));

        vec3 pos = vec3(0.0);
        pos.x = fract(velX * rnd + index) * 4.0 - 2.0;
        pos.y = sin(index * rnd * 1000.0 + velY) * yDistribution;
        pos.z = maxSize * rnd + minSize;

        vec2 uvRot = uv - pos.xy + pos.z / 2.0;
        uvRot = rotate2d(i + U_Time / 2.0) * uvRot;
        uvRot += pos.xy + pos.z / 2.0;

        float rect = rectangle(uvRot, pos.xy, pos.z, pos.z, (maxSize + minSize - pos.z) / 2.0);

        color += rectColor * rect * (pos.z / maxSize);
    }

    FragColor = vec4(color, 1.0);
}
