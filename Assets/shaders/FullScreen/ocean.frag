//FROM VENTOY

#version 450 core

in vec2 TexCoords;
out vec4 FragColor;

#include "../Common.glsl"

uniform sampler2D U_NormalTexture;
uniform sampler2D U_LightPassTexture;

const int NUM_STEPS = 8;
const float PI = 3.1415926535;
const float EPSILON = 1e-3;

const int ITER_GEOMETRY = 1;
const int ITER_FRAGMENT = 5;

const float SEA_HEIGHT = 0.6;
const float SEA_CHOPPY = 4.0;
const float SEA_SPEED = 0.5;
const float SEA_FREQ = 0.16;

const vec3 SEA_BASE_COLOR = vec3(0.03, 0.0, 0.0);
const vec3 SEA_WATER_COLOR = vec3(0.35, 0.05, 0.05);
const vec3 SKY_TOP = vec3(0.002, 0.005, 0.01);
const vec3 SKY_BOT = vec3(0.02, 0.03, 0.06);          

#define SEA_TIME (1.0 + U_Time * SEA_SPEED)
const mat2 octave_m = mat2(1.6, 1.2, -1.2, 1.6);

mat3 fromEuler(vec3 ang) {
    vec2 a1 = vec2(sin(ang.x), cos(ang.x));
    vec2 a2 = vec2(sin(ang.y), cos(ang.y));
    vec2 a3 = vec2(sin(ang.z), cos(ang.z));
    mat3 m;
    m[0] = vec3(a1.y * a3.y + a1.x * a2.x * a3.x, a1.y * a2.x * a3.x + a3.y * a1.x, -a2.y * a3.x);
    m[1] = vec3(-a2.y * a1.x, a1.y * a2.y, a2.x);
    m[2] = vec3(a3.y * a1.x * a2.x + a1.y * a3.x, a1.x * a3.x - a1.y * a3.y * a2.x, a2.y * a3.y);
    return m;
}

float hash(vec2 p) {
    float h = dot(p, vec2(127.1, 311.7));
    return fract(sin(h) * 43758.5453123);
}

float noise(in vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec2 u = f * f * (3.0 - 2.0 * f);
    return -1.0 + 2.0 * mix(mix(hash(i + vec2(0.0, 0.0)), hash(i + vec2(1.0, 0.0)), u.x), mix(hash(i + vec2(0.0, 1.0)), hash(i + vec2(1.0, 1.0)), u.x), u.y);
}

float diffuse(vec3 n, vec3 l, float p) {
    return pow(dot(n, l) * 0.4 + 0.6, p);
}
float specular(vec3 n, vec3 l, vec3 e, float s) {
    return pow(max(dot(reflect(e, n), l), 0.0), s) * ((s + 8.0) / (PI * 8.0));
}

vec3 getSkyColor(vec3 e) {
    float h = max(e.y, 0.0);
    return mix(SKY_BOT, SKY_TOP, pow(h, 0.5));
}

float sea_octave(vec2 uv, float choppy) {
    uv += noise(uv);
    vec2 wv = 1.0 - abs(sin(uv));
    vec2 swv = abs(cos(uv));
    wv = mix(wv, swv, wv);
    return pow(1.0 - pow(wv.x * wv.y, 0.65), choppy);
}

float map(vec3 p) {
    float freq = SEA_FREQ;
    float amp = SEA_HEIGHT;
    float choppy = SEA_CHOPPY;
    vec2 uv = p.xz;
    uv.x *= 0.75;
    float d, h = 0.0;
    for(int i = 0; i < ITER_GEOMETRY; i++) {
        d = sea_octave((uv + SEA_TIME) * freq, choppy);
        d += sea_octave((uv - SEA_TIME) * freq, choppy);
        h += d * amp;
        uv *= octave_m;
        freq *= 1.9;
        amp *= 0.22;
        choppy = mix(choppy, 1.0, 0.2);
    }
    return p.y - h;
}

float map_detailed(vec3 p) {
    float freq = SEA_FREQ;
    float amp = SEA_HEIGHT;
    float choppy = SEA_CHOPPY;
    vec2 uv = p.xz;
    uv.x *= 0.75;
    float d, h = 0.0;
    for(int i = 0; i < ITER_FRAGMENT; i++) {
        d = sea_octave((uv + SEA_TIME) * freq, choppy);
        d += sea_octave((uv - SEA_TIME) * freq, choppy);
        h += d * amp;
        uv *= octave_m;
        freq *= 1.9;
        amp *= 0.22;
        choppy = mix(choppy, 1.0, 0.2);
    }
    return p.y - h;
}

vec3 getSeaColor(vec3 p, vec3 n, vec3 l, vec3 eye, vec3 dist) {
    float fresnel = clamp(1.0 - dot(n, -eye), 0.0, 1.0);
    fresnel = min(fresnel * fresnel * fresnel, 0.5);

    vec3 reflected = getSkyColor(reflect(eye, n));
    vec3 refracted = SEA_BASE_COLOR + diffuse(n, l, 120.0) * SEA_WATER_COLOR * 0.1;

    vec3 color = mix(refracted, reflected, fresnel);
    float atten = max(1.0 - dot(dist, dist) * 0.001, 0.0);
    color += SEA_WATER_COLOR * (p.y - SEA_HEIGHT) * 0.12 * atten;

    color += vec3(0.8, 0.9, 1.0) * specular(n, l, eye, 1200.0 * inversesqrt(dot(dist, dist))) * 0.05;

    return color;
}


vec3 getNormal(vec3 p, float eps) {
    vec3 n;
    n.y = map_detailed(p);
    n.x = map_detailed(vec3(p.x + eps, p.y, p.z)) - n.y;
    n.z = map_detailed(vec3(p.x, p.y, p.z + eps)) - n.y;
    n.y = eps;
    return normalize(n);
}

float heightMapTracing(vec3 ori, vec3 dir, out vec3 p) {
    float tm = 0.0;
    float tx = 1000.0;
    float hx = map(ori + dir * tx);
    if(hx > 0.0) {
        p = ori + dir * tx;
        return tx;
    }
    float hm = map(ori);
    for(int i = 0; i < NUM_STEPS; i++) {
        float tmid = mix(tm, tx, hm / (hm - hx));
        p = ori + dir * tmid;
        float hmid = map(p);
        if(hmid < 0.0) {
            tx = tmid;
            hx = hmid;
        } else {
            tm = tmid;
            hm = hmid;
        }
        if(abs(hmid) < EPSILON)
            break;
    }
    return mix(tm, tx, hm / (hm - hx));
}

vec3 getCameraRay(vec2 ndc) {
    vec4 viewSpacePos = U_InvProjection * vec4(ndc, 1.0, 1.0);
    viewSpacePos /= viewSpacePos.w;
    viewSpacePos.w = 0.0;
    return normalize(vec3(U_InvView * viewSpacePos));
}

void main() {
    float depth = texture(U_NormalTexture, TexCoords).a;
    vec4 sceneColor = texture(U_LightPassTexture, TexCoords);

    if(depth < 0.9999 && depth > 0.0) {
        FragColor = sceneColor;
        return;
    }

    vec2 ndc = TexCoords * 2.0 - 1.0;
    vec3 dir = getCameraRay(ndc);


    vec3 ori = vec3(U_InvView[3][0], U_InvView[3][1] + 3.0, U_InvView[3][2]);

    float aspect = (U_ScreenWidth) / (U_ScreenHeight);
    float epsilon_nrm = 0.1 / (U_ScreenWidth);


    vec3 p;
    heightMapTracing(ori, dir, p);
    vec3 dist = p - ori;
    vec3 n = getNormal(p, dot(dist, dist) * epsilon_nrm);

    vec3 light = normalize(vec3(0.3, 0.8, 0.6));

    vec3 seaColor = getSeaColor(p, n, light, dir, dist);
    vec3 skyColor = getSkyColor(dir);


    vec3 finalColor = mix(skyColor, seaColor, pow(smoothstep(0.0, -0.02, dir.y), 0.2));

    vec3 x = finalColor;
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    vec3 mappedColor = clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);

    FragColor = vec4(pow(mappedColor, vec3(1.0 / 2.2)), 1.0);
}
