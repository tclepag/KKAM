#pragma once

namespace math {
    class Vector2 {
    public:
        float x, y;

        Vector2() : x(0), y(0) {}
        Vector2(float x, float y) : x(x), y(y) {}

		// Returns the sum of this Vector2 and another Vector2
        Vector2 operator+(const Vector2& other) const {
            return Vector2(x + other.x, y + other.y);
        }

		// Adds this Vector2 to another Vector2 and updates the Vector2
		Vector2& operator+=(const Vector2& other) {
			x += other.x;
			y += other.y;
			return *this;
		}

		// Returns the negation of this Vector2
		Vector2 operator-() const {
			return Vector2(-x, -y);
		}

		// Returns the difference of this Vector2 and another Vector2
        Vector2 operator-(const Vector2& other) const {
            return Vector2(x - other.x, y - other.y);
        }

		// Subtracts this Vector2 from another Vector2 and updates the Vector2
		Vector2& operator-=(const Vector2& other) {
			x -= other.x;
			y -= other.y;
			return *this;
		}

		// Returns the product of this Vector2 and a scalar
        Vector2 operator*(float scalar) const {
            return Vector2(x * scalar, y * scalar);
        }

		// Returns the product of this Vector2 and another Vector2
		Vector2 operator*(const Vector2& other) const {
			return Vector2(x * other.x, y * other.y);
		}

		// Returns the product of this Vector2 and another Vector2 and updates the Vector2
		Vector2& operator*=(const Vector2& other) {
			x *= other.x;
			y *= other.y;
			return *this;
		}

		// Multiplies this Vector2 by a scalar and updates the Vector2
		Vector2& operator*=(float scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}

		// Returns the quotient of this Vector2 and a scalar
        Vector2 operator/(float scalar) const {
            return Vector2(x / scalar, y / scalar);
        }

		// Returns the quotient of this Vector2 and another Vector2
		Vector2 operator/(const Vector2& other) const {
			return Vector2(x / other.x, y / other.y);
		}

		// Divides this Vector2 by a scalar and updates the Vector2
		Vector2& operator/=(float scalar) {
			x /= scalar;
			y /= scalar;
			return *this;
		}

		// Divides this Vector2 by another Vector2 and updates the Vector2
		Vector2& operator/=(const Vector2& other) {
			x /= other.x;
			y /= other.y;
			return *this;
		}

        // Normalizes this Vector2, making it a unit vector with a length of 1
        Vector2& normalize() {
			auto normalized = normalize(*this); // See static method
			x = normalized.x;
			y = normalized.y;
			return *this;
        }

		// Returns the normalized version of this Vector2
        Vector2 normalized() const {
			return normalize(*this); // See static method
        }

		// Gets the length of the Vector2
		float magnitude() const {
			// |v| = √(x^2 + y^2)
			return sqrt(x * x * y * y);
		}

		// Calculates the dot product of two Vector2s
		float dot(const Vector2& other) {
			// x = x1 * x2
			// y = y1 * y2
			// dot = x + y
			float x = x * other.x;
			float y = y * other.y;
			return x + y;
		}

		// Calculates the distance between this Vector2 and another
		float distance(const Vector2& other) const {
			return distance(*this, other); // See static method
		}


        // Normalizes a Vector2, making it a unit vector with a length of 1
		static Vector2 normalize(const Vector2& v) {
			// |v| = √(x^2 + y^2)
			// v' = (x / |v|, y / |v|)
			float length = sqrt(v.x * v.x + v.y * v.y);
			return Vector2(v.x / length, v.y / length);
		}

		// Calculates the distance between two Vector2s
		static float distance(const Vector2& a, const Vector2& b) {
			// Get the difference between the two vectors and get the magnitude of that difference
			return (a-b).magnitude();
		}

        // TODO ADD MORE VECTOR2 FUNCTIONALTIY
    };
}