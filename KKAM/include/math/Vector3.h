#pragma once

namespace math {
    class Vector3 {
    public:
        float x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        // Adds two vectors
        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        // Subtracts one vector from another
        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        // Multiplies vector by a scalar
        Vector3 operator*(float scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        // Multiplies vector by another vector
        Vector3 operator*(const Vector3& other) const {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        // Divides vector by a scalar
        Vector3 operator/(float scalar) const {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }

        // Divides vector by another vector
        Vector3 operator/(const Vector3& other) const {
            return Vector3(x / other.x, y / other.y, z / other.z);
        }

        // Adds another vector to this vector
        Vector3& operator+=(const Vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        // Subtracts another vector from this vector
        Vector3& operator-=(const Vector3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        // Multiplies this vector by a scalar
        Vector3& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        // Multiplies this vector by another vector
        Vector3& operator*=(const Vector3& other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        // Divides this vector by a scalar
        Vector3& operator/=(float scalar) {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        // Divides this vector by another vector
        Vector3& operator/=(const Vector3& other) {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            return *this;
        }

        // Normalizes this Vector3, making it a unit vector with a length of 1
        Vector3& normalize() {
            auto normalized = normalize(*this); // See static method
            x = normalized.x;
            y = normalized.y;
            z = normalized.z;
            return *this;
        }

        // Returns the normalized version of this Vector3
        Vector3 normalized() const {
            return normalize(*this); // See static method
        }

		// Returns the dot product between this and another Vector3
		float dot(const Vector3& other) const {
			return dot(*this, other); // See static method
		}

        // Gets the length of the Vector3
        float magnitude() const {
            return sqrt(x * x + y * y + z * z);
        }

        // Calculates the distance between this Vector3 and another
		float distance(const Vector3& other) const {
			return distance(*this, other); // See static method
		}

        // Normalizes a Vector3, making it a unit vector with a length of 1
        static Vector3 normalize(const Vector3& v) {
            float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
            return Vector3(v.x / length, v.y / length, v.z / length);
        }

        // Calculates the cross product of two Vector3s
        static Vector3 cross(const Vector3& a, const Vector3& b) {
            return Vector3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        // Calculates the dot product of two Vector3s
        static float dot(const Vector3& a, const Vector3& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

		// Calculates the distance between two Vector3s
		static float distance(const Vector3& a, const Vector3& b) {
			// Subtract both vectors and get the magnitude of that difference
			return (a-b).magnitude();
		}

        // TODO ADD MORE VECTOR3 FUNCTIONALITY
    };
}