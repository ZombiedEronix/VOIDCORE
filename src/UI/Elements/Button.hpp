#pragma once
#include "Decorator.hpp"
#include "UI/UIRegistry.hpp"
#include "TextBlock.hpp"
#include "Assets/ResourceManager.h"
#include <functional>
#include <string>

class Button : public Decorator
{
public:
    Color NormalColor = Color::FromHex("#2A2A3EFF");
    Color HoverColor = Color::FromHex("#454566FF");
    Color PressedColor = Color::FromHex("#1B1B29FF");

    std::function<void()> Action;

    Button()
    {
        SetProperty("Material", "materials/UI/Rectangle.mat");
    }

    void SetText(const std::string &label)
    {
        auto textBlock = std::dynamic_pointer_cast<TextBlock>(child);
        if (!textBlock)
        {
            textBlock = std::make_shared<TextBlock>();
            textBlock->IsHitTestVisible = false;
            // Утечка.
            textBlock->material = ResourceManager::GetMaterial("materials/UI/Text.mat")->Clone();
            textBlock->SetProperty("Font", "fonts/Default.ttf");

            textBlock->HorizontalAlignment = HAlign::Center;
            textBlock->VerticalAlignment = VAlign::Center;
            child = textBlock;
        }
        textBlock->Text = label;
    }

    void SetProperty(const std::string &name, const std::string &value) override
    {
        if (name == "Background")
        {
            Background = Color::FromHex(value);
            material->SetColor("Background", Background);
        }
        else if (name == "Text")
        {
            SetText(value);
            return;
        }
        else
        {
            Decorator::SetProperty(name, value);
        }
    }

    UIControl *HitTest(float x, float y) override
    {
        if (!Visible || !IsHitTestVisible || !rect.IsInside(x, y))
            return nullptr;

        if (UIControl *hit = Decorator::HitTest(x, y))
            return hit;

        return this;
    }

    void OnMouseEnter() override
    {
        Background = HoverColor;
        material->SetColor("Background", Background);
    }

    void OnMouseLeave() override
    {
        Background = NormalColor;
        material->SetColor("Background", Background);
    }

    void OnMouseDown() override
    {
        Background = PressedColor;
        material->SetColor("Background", Background);
    }

    void OnMouseUp() override
    {
        Background = NormalColor;
        material->SetColor("Background", Background);
    }

    void OnClick() override
    {
        if (Action)
            Action();
    }

    void Render(UIContext &ctx) override
    {
        Decorator::Render(ctx);
    }

    std::string GetTypeName() const override { return "Button"; }
};
REGISTER_UI_CONTROL(Button);