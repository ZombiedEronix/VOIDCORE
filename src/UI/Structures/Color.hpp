#pragma once
#include <cstdint>

struct Color
{
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;

    constexpr Color() = default;
    constexpr Color(float r, float g, float b, float a = 1.0f)
        : r(r), g(g), b(b), a(a) {}

    static constexpr Color FromBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
    {
        return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
    }

    static constexpr Color White() { return {1.0f, 1.0f, 1.0f, 1.0f}; }
    static constexpr Color Black() { return {0.0f, 0.0f, 0.0f, 1.0f}; }
    static constexpr Color Red() { return {1.0f, 0.0f, 0.0f, 1.0f}; }
    static constexpr Color Green() { return {0.0f, 1.0f, 0.0f, 1.0f}; }
    static constexpr Color Blue() { return {0.0f, 0.0f, 1.0f, 1.0f}; }
    static constexpr Color Transparent() { return {0.0f, 0.0f, 0.0f, 0.0f}; }

    constexpr Color WithAlpha(float alpha) const { return {r, g, b, alpha}; }
    constexpr bool IsVisible() const { return a > 0.0f; }
};
