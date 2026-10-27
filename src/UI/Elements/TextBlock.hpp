#pragma once
#include <string>

#include "UI/Elements/UIControl.hpp"

// Текст (UTF-8, '\n' — перенос). Автоматических переносов по ширине пока нет.
class TextBlock : public UIControl
{
public:
    std::string Text;
    Color Foreground = Color::White();
    Font *font = nullptr; // nullptr => Font::GetDefault()

    TextBlock() = default;
    explicit TextBlock(std::string text, Color color = Color::White())
        : Text(std::move(text)), Foreground(color) {}

protected:
    Size MeasureOverride(Size) override
    {
        const Font *f = font ? font : Font::GetDefault();
        return f ? f->MeasureText(Text) : Size{};
    }

    void OnRender(UIContext &ctx) override
    {
        const Font *f = font ? font : Font::GetDefault();
        const Rect c = ContentRect();
        ctx.AddText(f, Text, c.x, c.y, Foreground);
    }
};
