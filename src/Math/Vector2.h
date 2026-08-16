struct Vector2
{
    union
    {
        struct
        {
            float x, y;
        };
        float data[2];
    };

    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2 &other) const
    {
        return {x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2 &other) const
    {
        return {x - other.x, y - other.y};
    }
};