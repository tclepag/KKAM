#pragma once

#include <stdexcept>

namespace math {
    class Vector4 {
    public:
        float x, y, z, w;

        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        Vector4 operator+(const Vector4& other) const {
            return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        Vector4 operator-(const Vector4& other) const {
            return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        Vector4 operator*(float scalar) const {
            return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
        }

        Vector4 operator/(float scalar) const {
            return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
        }

        float& operator[](int index) {
            if (index < 0 || index > 3) {
                throw std::out_of_range("Index out of range");
            }
            return *(&x + index);
        }

        const float& operator[](int index) const {
            if (index < 0 || index > 3) {
                throw std::out_of_range("Index out of range");
            }
            return *(&x + index);
        }

        // TODO ADD MORE VECTOR4 FUNCTIONALITY
    };
}