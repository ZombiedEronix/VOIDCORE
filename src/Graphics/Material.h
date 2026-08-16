#pragma once
#include <Graphics/Shader.h>
#include <Assets/Texture.h>
#include <map>

enum class RenderQueue {
	Opaque,
	Masked,
	Transparent
};

class Material
{
public:
	Shader* shader;
	RenderQueue queueType = RenderQueue::Opaque;


    std::map<std::string, Texture*> textures;
    std::map<std::string, float> floats;

    void ApplyStatic()
    {
        shader->Load();

        if (queueType == RenderQueue::Transparent) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);
        }
        else {
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
        }

        int textureUnit = 0;
        for (const auto& [name, tex] : textures) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            tex->Bind();
            shader->SetInt(name, textureUnit);
            textureUnit++;
        }

        for (const auto& [name, value] : floats) {
            shader->SetFloat(name, value);
        }
    }
};
