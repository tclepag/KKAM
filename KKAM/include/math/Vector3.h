#pragma once

namespace math {
    class Vector3 {
    public:
        float x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator*(float scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        Vector3 operator/(float scalar) const {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }

        static Vector3 normalize(const Vector3& v) {
            float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
            return Vector3(v.x / length, v.y / length, v.z / length);
        }

        static Vector3 cross(const Vector3& a, const Vector3& b) {
            return Vector3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        static float dot(const Vector3& a, const Vector3& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        // TODO ADD MORE VECTOR3 FUNCTIONALITY
    };
}