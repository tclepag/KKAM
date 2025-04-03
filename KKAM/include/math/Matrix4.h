#pragma once

#include "Vector4.h"
#include "Vector3.h"
#include <cmath>
#include <windows.h>
#include <sstream>

namespace math {
    __declspec(align(16)) class Matrix4 {
    public:
        Vector4 rows[4];

        Matrix4() {
            rows[0] = Vector4(1, 0, 0, 0);
            rows[1] = Vector4(0, 1, 0, 0);
            rows[2] = Vector4(0, 0, 1, 0);
            rows[3] = Vector4(0, 0, 0, 1);
        }

        static Matrix4 identity() {
            return Matrix4();
        }

        static Matrix4 translation(const Vector3& translation) {
            Matrix4 result = identity();
            result.rows[3] = Vector4(translation.x, translation.y, translation.z, 1);
            return result;
        }

        static Matrix4 scale(const Vector3& scale) {
            Matrix4 result = identity();
            result.rows[0].x = scale.x;
            result.rows[1].y = scale.y;
            result.rows[2].z = scale.z;
            return result;
        }

        static Matrix4 rotationX(float angle) {
            Matrix4 result = identity();
            float cosAngle = cosf(angle);
            float sinAngle = sinf(angle);
            result.rows[1].y = cosAngle;
            result.rows[1].z = sinAngle;
            result.rows[2].y = -sinAngle;
            result.rows[2].z = cosAngle;
            return result;
        }

        static Matrix4 rotationY(float angle) {
            Matrix4 result = identity();
            float cosAngle = cosf(angle);
            float sinAngle = sinf(angle);
            result.rows[0].x = cosAngle;
            result.rows[0].z = -sinAngle;
            result.rows[2].x = sinAngle;
            result.rows[2].z = cosAngle;
            return result;
        }

        static Matrix4 rotationZ(float angle) {
            Matrix4 result = identity();
            float cosAngle = cosf(angle);
            float sinAngle = sinf(angle);
            result.rows[0].x = cosAngle;
            result.rows[0].y = sinAngle;
            result.rows[1].x = -sinAngle;
            result.rows[1].y = cosAngle;
            return result;
        }

        Matrix4 transpose() const {
            Matrix4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.rows[i][j] = rows[j][i];
                }
            }
            return result;
        }

        Matrix4 operator+(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 4; ++i) {
                result.rows[i] = rows[i] + other.rows[i];
            }
            return result;
        }

        Matrix4 operator-(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 4; ++i) {
                result.rows[i] = rows[i] - other.rows[i];
            }
            return result;
        }

        Matrix4 operator-() const {
            Matrix4 result;
            for (int i = 0; i < 4; ++i) {
                result.rows[i][0] = -rows[i][0];
				result.rows[i][1] = -rows[i][1];
				result.rows[i][2] = -rows[i][2];
				result.rows[i][3] = -rows[i][3];
            }
            return result;
        }

        Matrix4 operator*(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.rows[i][j] = rows[i][0] * other.rows[0][j] +
                        rows[i][1] * other.rows[1][j] +
                        rows[i][2] * other.rows[2][j] +
                        rows[i][3] * other.rows[3][j];
                }
            }
            return result;
        }

        Vector4 operator*(const Vector4& vec) const {
            return Vector4(
                rows[0].x * vec.x + rows[0].y * vec.y + rows[0].z * vec.z + rows[0].w * vec.w,
                rows[1].x * vec.x + rows[1].y * vec.y + rows[1].z * vec.z + rows[1].w * vec.w,
                rows[2].x * vec.x + rows[2].y * vec.y + rows[2].z * vec.z + rows[2].w * vec.w,
                rows[3].x * vec.x + rows[3].y * vec.y + rows[3].z * vec.z + rows[3].w * vec.w
            );
        }

		// Prints the matrix to the output window
        void output(const char* name) const {
            std::ostringstream oss;
            oss << name << ":\n";
            for (int i = 0; i < 16; ++i) {
                oss << rows[i / 4][i % 4] << " ";
                if ((i + 1) % 4 == 0) {
                    oss << "\n";
                }
            }
            std::string str = oss.str();
            OutputDebugStringA(str.c_str());
        }

        static Matrix4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
            Vector3 zaxis = Vector3::normalize(eye - target);    // The "forward" vector.
            Vector3 xaxis = Vector3::normalize(Vector3::cross(up, zaxis));// The "right" vector.
            Vector3 yaxis = Vector3::cross(zaxis, xaxis);        // The "up" vector.

            Matrix4 view = Matrix4::identity();
            view.rows[0] = Vector4(xaxis.x, yaxis.x, zaxis.x, 0);
            view.rows[1] = Vector4(xaxis.y, yaxis.y, zaxis.y, 0);
            view.rows[2] = Vector4(xaxis.z, yaxis.z, zaxis.z, 0);
            view.rows[3] = Vector4(-Vector3::dot(xaxis, eye), -Vector3::dot(yaxis, eye), -Vector3::dot(zaxis, eye), 1);

            return view;
        }

        static Matrix4 perspective(float fovY, float aspect, float nearZ, float farZ) {
            Matrix4 result = {};
            float tanHalfFovY = tanf(fovY / 2.0f);

            result.rows[0][0] = 1.0f / (aspect * tanHalfFovY);
            result.rows[1][1] = 1.0f / tanHalfFovY;
            result.rows[2][2] = farZ / (nearZ - farZ);
            result.rows[2][3] = -1.0f;
            result.rows[3][2] = (farZ * nearZ) / (nearZ - farZ);
            result.rows[3][3] = 0.0f;

            return result;
        }


        static Matrix4 orthographic(float left, float right, float bottom, float top, float nearZ, float farZ) {
            Matrix4 result = {};

            result.rows[0][0] = 2.0f / (right - left);
            result.rows[1][1] = 2.0f / (top - bottom);
            result.rows[2][2] = -2.0f / (farZ - nearZ);
            result.rows[3][0] = -(right + left) / (right - left);
            result.rows[3][1] = -(top + bottom) / (top - bottom);
            result.rows[3][2] = -(farZ + nearZ) / (farZ - nearZ);
            result.rows[3][3] = 1.0f;

            return result;
        }

        // TODO ADD MORE MATRIX4 FUNCTIONALITY
    };
}