#include <glm/glm.hpp>
#include "Graphics/Queue/LightCommand.hpp"

struct LightsBlock
{
    LightCommand lights[64];
    int currentLights = 0;
    float padding[3];

    void AddLight(const LightCommand &info)
    {
        if (currentLights < 64)
        {
            lights[currentLights] = info;
            currentLights++;
        }
    }
    void Clear() { currentLights = 0; }
};

// Переместить в другой файл...
struct ShaderBlock
{
    glm::mat4 U_Projection;
    glm::mat4 U_View;
    glm::mat4 U_InvProj;
    glm::mat4 U_InvView;
    glm::mat4 U_OrthoProjection;
    float U_Time;
    float U_DeltaTime;
    float U_ScreenWidth;
    float U_ScreenHeight;
};
// Переместить в другой файл...