#pragma once
#include <string>

#include "UI/Elements/UIControl.hpp"
#include "Entity/Material.hpp"
#include "GarbageCollector/Ref.hpp"

class TextBlock : public UIControl
{
public:
    std::string Text;
    /// @brief Background is not working now :(
    Color Foreground = Color::White();
    Ref<const Font> font = nullptr;
    Ref<Material> material;

    TextBlock() = default;
    explicit TextBlock(std::string text, Color color = Color::White())
        : Text(std::move(text)), Foreground(color) {}

    std::string GetTypeName() const override { return "TextBlock"; }

    void SetProperty(const std::string &name, const std::string &value) override
    {
        if (name == "Background")
        {
            Background = Color::FromHex(value);
            material.Get()->SetColor("Background", Background);
        }

        else if (name == "Text")
        {
            Text = value;
        }

        else if (name == "Foreground")
        {
            Foreground = Color::FromHex(value);
        }
        else if (name == "Font")
        {

            font = ResourceManager::GetFont(value);

            // I don't know how fix this situation.
            if (material == nullptr)
            {
                assert(false && "Error: material is nullptr.");
            }

            material.Get()->SetTexture(0, new Texture(font->AtlasPixels(), font->AtlasWidth(), font->AtlasHeight(), "texture_font"));
        }
        else
        {
            UIControl::SetProperty(name, value);
        }
    }

protected:
    Size MeasureOverride(Size) override
    {
        return font ? font->MeasureText(Text) : Size{};
    }

    void OnRender(UIContext &ctx) override
    {
        const Rect c = ContentRect();

        ctx.AddText(material, font, Text, c.x, c.y, Foreground);
    }
};
REGISTER_UI_CONTROL(TextBlock);