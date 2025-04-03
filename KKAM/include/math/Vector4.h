#pragma once

#include <cmath>
#include <stdexcept>

namespace math {
    class Vector4 {
    public:
        float x, y, z, w;

        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        // Addition of two Vector4
        Vector4 operator+(const Vector4& other) const {
            return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        // Returns the negated Vector4
        Vector4 operator-() const {
            return Vector4(-x, -y, -z, -w);
        }

        // Subtraction of two Vector4
        Vector4 operator-(const Vector4& other) const {
            return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        // Multiplication of Vector4 by a scalar
        Vector4 operator*(float scalar) const {
            return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
        }

        // Division of Vector4 by a scalar
        Vector4 operator/(float scalar) const {
            return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
        }

        // Addition assignment of two Vector4
        Vector4& operator+=(const Vector4& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        // Subtraction assignment of two Vector4
        Vector4& operator-=(const Vector4& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        // Multiplication assignment of Vector4 by a scalar
        Vector4& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        // Division assignment of Vector4 by a scalar
        Vector4& operator/=(float scalar) {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;
            return *this;
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

        // Gets the length of the Vector4
        float magnitude() const {
            // |v| = √(x² + y² + z² + w²)
            return sqrt(x * x + y * y + z * z + w * w);
        }

        // Normalizes the Vector4
        Vector4 normalize() const {
            float length = magnitude();
            if (length == 0) {
                throw std::runtime_error("Cannot normalize a zero-length vector");
            }
            return Vector4(x / length, y / length, z / length, w / length);
        }

        // Calculates the dot product of this vector with another vector
        float dot(const Vector4& other) const {
            // Dot product formula: a · b = ax * bx + ay * by + az * bz + aw * bw
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        // TODO ADD MORE VECTOR4 FUNCTIONALITY
    };
}

