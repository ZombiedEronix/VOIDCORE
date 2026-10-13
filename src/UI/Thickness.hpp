#pragma once

struct Thickness
{
    float Top = 0.0f;
    float Right = 0.0f;
    float Bottom = 0.0f;
    float Left = 0.0f;

    Thickness() = default;

    explicit Thickness(float all)
        : Top(all), Right(all), Bottom(all), Left(all) {}

    Thickness(float vertical, float horizontal)
        : Top(vertical), Right(horizontal), Bottom(vertical), Left(horizontal) {}

    Thickness(float t, float r, float b, float l)
        : Top(t), Right(r), Bottom(b), Left(l) {}
};