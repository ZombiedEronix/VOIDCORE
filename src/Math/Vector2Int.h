struct Vector2Int {
    union {
        struct { int x, y; };
        int data[2];
    };

    Vector2Int() : x(0), y(0) {}
    Vector2Int(int x, int y) : x(x), y(y) {}

    Vector2Int operator+(const Vector2Int& other) const {
        return { x + other.x, y + other.y};
    }
    
    Vector2Int operator-(const Vector2Int& other) const {
        return { x - other.x, y - other.y};
    }
};