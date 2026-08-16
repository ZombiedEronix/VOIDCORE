struct Vector3Int {
    union {
        struct { int x, y, z; };
        int data[3];
    };

    Vector3Int() : x(0), y(0), z(0) {}
    Vector3Int(int x, int y, int z) : x(x), y(y), z(z) {}

    Vector3Int operator+(const Vector3Int& other) const {
        return { x + other.x, y + other.y, z + other.z};
    }
    
    Vector3Int operator-(const Vector3Int& other) const {
        return { x - other.x, y - other.y, z - other.z};
    }
};