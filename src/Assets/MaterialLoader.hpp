#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

#include "Graphics/Resources/Material.h"
#include "Assets/File.hpp"
#include "Core/Debug.h"

class MaterialLoader
{
public:
    template <typename ShaderResolver, typename TextureResolver>
    static std::shared_ptr<Material> LoadYAML(const VoidCore::Assets::File &asset, ShaderResolver &&getShader, TextureResolver &&getTexture)
    {
        try
        {
            YAML::Node root = YAML::Load(asset.AsString());

            if (!root["shader"])
            {
                Debug::LogError("MaterialLoader: Missing 'shader' field in " + asset.GetPath());
                return nullptr;
            }

            std::string shaderPath = root["shader"].as<std::string>();
            std::string pipelineType = root["pipeline"] ? root["pipeline"].as<std::string>() : "Lit";

            Shader *shader = getShader(shaderPath);
            if (!shader)
            {
                Debug::LogError("MaterialLoader: Failed to load shader -> " + shaderPath);
                return nullptr;
            }

            auto pipeline = (pipelineType == "UI")
                                ? Material::CreateUIPipeline(shader)
                                : Material::CreateLitPipeline(shader);

            auto mat = std::make_shared<Material>(shader, pipeline);

            if (auto textures = root["textures"])
            {
                for (const auto &it : textures)
                {
                    uint32_t slot = it.first.as<uint32_t>();
                    std::string texPath;
                    bool srgb = false;

                    if (it.second.IsScalar())
                    {
                        texPath = it.second.as<std::string>();
                    }
                    else if (it.second.IsMap())
                    {
                        if (it.second["path"])
                            texPath = it.second["path"].as<std::string>();
                        if (it.second["srgb"])
                            srgb = it.second["srgb"].as<bool>();
                    }

                    if (!texPath.empty())
                    {
                        auto tex = getTexture(texPath, srgb);
                        if (tex)
                        {
                            mat->SetTexture(slot, tex.get());
                        }
                    }
                }
            }

            if (auto floats = root["floats"])
            {
                for (const auto &it : floats)
                {
                    mat->SetFloat(it.first.as<std::string>(), it.second.as<float>());
                }
            }

            if (auto vectors = root["vectors"])
            {
                for (const auto &it : vectors)
                {
                    std::string name = it.first.as<std::string>();
                    const auto &seq = it.second;
                    if (seq.IsSequence() && seq.size() >= 4)
                    {
                        glm::vec4 v(
                            seq[0].as<float>(),
                            seq[1].as<float>(),
                            seq[2].as<float>(),
                            seq[3].as<float>());
                        mat->SetVector(name, v);
                    }
                }
            }

            return mat;
        }
        catch (const YAML::Exception &e)
        {
            Debug::LogError("MaterialLoader: YAML parse error in " + asset.GetPath() + " -> " + e.what());
            return nullptr;
        }
    }
};
