layout (std140, binding = 0) uniform CameraBlock
{
    mat4 U_Projection;
    mat4 U_View;
    mat4 U_InvProjection;
    mat4 U_InvView;
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