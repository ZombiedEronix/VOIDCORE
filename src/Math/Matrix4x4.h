struct Matrix4x4
{
	union
	{
		float data[16];
		float m[4][4];
	};

    Matrix4x4 operator*(const Matrix4x4& other) const {
        Matrix4x4 res;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                float sum = 0;
                for (int k = 0; k < 4; ++k) {
                    sum += data[row * 4 + k] * other.data[k * 4 + col];
                }
                res.data[row * 4 + col] = sum;
            }
        }
        return res;
    }

};