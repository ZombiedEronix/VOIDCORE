layout (std140, binding = 0) uniform CameraBlock
{
    mat4 U_Projection;
    mat4 U_View;
    mat4 U_InvProjection;
    mat4 U_InvView;
    mat4 U_OrthoProjection;

    float U_Time;
    float U_DeltaTime;
    float U_ScreenWidth;
    float U_ScreenHeight;
};

//IN UNIFORM
const float NEAR = 0.1;
const float FAR = 1000.0;

float GetLinearDepth(float rawDepth) {
    float z = rawDepth * 2.0 - 1.0;
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

vec3 GetWorldPosition(vec2 screenUV, float depth) {
    vec4 ndc = vec4(screenUV * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    vec4 viewPos = U_InvProjection * ndc;
    viewPos /= viewPos.w;
    vec4 worldPos = U_InvView * viewPos;
    return worldPos.xyz;
}

vec3 GetViewPosition(vec2 screenUV, float depth) {
    vec4 ndc = vec4(screenUV * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    vec4 viewPos = U_InvProjection * ndc;
    return viewPos.xyz / viewPos.w;
}

vec3 GetCameraPosition() {
    return U_InvView[3].xyz;
}

vec3 GetCameraForward() {
    return normalize(-U_InvView[2].xyz);
}

vec2 GetScreenUV(vec2 fragCoord) {
    return fragCoord.xy / vec2(U_ScreenWidth, U_ScreenHeight);
}

vec4 SampleTriplanar(sampler2D tex, vec3 worldPos, vec3 worldNormal, float scale, float sharpness) {
    vec3 blend = pow(abs(normalize(worldNormal)), vec3(sharpness));
    blend /= (blend.x + blend.y + blend.z);

    vec4 xProj = texture(tex, worldPos.zy * scale);
    vec4 yProj = texture(tex, worldPos.xz * scale);
    vec4 zProj = texture(tex, worldPos.xy * scale);

    return xProj * blend.x + yProj * blend.y + zProj * blend.z;
}