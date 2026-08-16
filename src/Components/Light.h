#include "Components/Component.h"
#include "Graphics/RenderPipeline/LightCommand.h"
#include "Core/GameObject.h"
#include "glm.hpp"

class Light : public Component
{
public:
    glm::vec3 color = glm::vec3(1,1,1);
    float radius = 16;
    float intensity = 1;

    LightCommand GetData();
    void Update() override;
};