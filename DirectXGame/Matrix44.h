#pragma once
#include <cmath>
#include <assert.h>
#include "Vec3.h"

class Matrix44 {
public:

	static const int kMatrixNum = 4;
	float m[kMatrixNum][kMatrixNum];
public:

	inline Matrix44() {
		m[0][0] = 1.0f, m[0][1] = 0.0f, m[0][2] = 0.0f, m[0][3] = 0.0f;  //1行目
		m[1][0] = 0.0f, m[1][1] = 1.0f, m[1][2] = 0.0f, m[1][3] = 0.0f;  //2行目
		m[2][0] = 0.0f, m[2][1] = 0.0f, m[2][2] = 1.0f, m[2][3] = 0.0f;  //3行目
		m[3][0] = 0.0f, m[3][1] = 0.0f, m[3][2] = 0.0f, m[3][3] = 1.0f;  //4行目
	}

	inline Matrix44(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) {
		m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03;  //1行目
		m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13;  //2行目
		m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23;  //3行目
		m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;  //4行目
	}

	friend inline Matrix44 operator+(const Matrix44& m1, const Matrix44& m2) {
		Matrix44 result{};
		for (size_t row = 0; row < kMatrixNum; row++) {
			for (size_t column = 0; column < kMatrixNum; column++) {
				result.m[row][column] = m1.m[row][column] + m2.m[row][column];
			}
		}
		return result;
	}

	friend inline Matrix44 operator-(const Matrix44& m1, const Matrix44& m2) {
		Matrix44 result{};
		for (size_t row = 0; row < kMatrixNum; row++) {
			for (size_t column = 0; column < kMatrixNum; column++) {
				result.m[row][column] = m1.m[row][column] - m2.m[row][column];
			}
		}
		return result;
	}

	friend inline Matrix44 operator*(const Matrix44& m1, const Matrix44& m2) {
		Matrix44 result{};
		for (size_t row = 0; row < kMatrixNum; row++) {
			for (size_t column = 0; column < kMatrixNum; column++) {
				result.m[row][column] = (m1.m[row][0] * m2.m[0][column]) + (m1.m[row][1] * m2.m[1][column]) + (m1.m[row][2] * m2.m[2][column]) + (m1.m[row][3] * m2.m[3][column]);
			}
		}
		return result;
	}

	friend inline Matrix44 operator*(const float scalar, const Matrix44& matrix) {
		Matrix44 result{};
		for (size_t row = 0; row < kMatrixNum; row++) {
			for (size_t column = 0; column < kMatrixNum; column++) {
				result.m[row][column] = matrix.m[row][column] * scalar;
			}
		}
	}

	friend inline Matrix44 operator*(const Matrix44& matrix, const float scalar) {
		return scalar * matrix;
	}

	inline Matrix44 Inverse() {
		Matrix44 result{};
		float sweep[kMatrixNum][kMatrixNum * 2]{};
		for (size_t row = 0; row < kMatrixNum; row++) {
			for (size_t column = 0; column < kMatrixNum; column++) {
				sweep[row][column] = m[row][column];
				sweep[row][kMatrixNum + column] = (row == column) ? 1.0f : 0.0f;
			}
		}

		for (size_t k = 0; k < kMatrixNum; k++) {
			float a = 1 / sweep[k][k];
			for (size_t column = 0; column < (kMatrixNum * 2); column++) {
				sweep[k][column] *= a;
			}
			for (size_t row = 0; row < kMatrixNum; row++) {
				if (row == k) { continue; }
				a = -sweep[row][k];
				for (size_t column = 0; column < (kMatrixNum * 2); column++) {
					sweep[row][column] += sweep[k][column] * a;
				}
			}
		}

		for (size_t row = 0; row < kMatrixNum; row++) {
			for (size_t column = 0; column < kMatrixNum; column++) {
				result.m[row][column] = sweep[row][column + kMatrixNum];
			}
		}
		return result;
	}

	inline Matrix44 Transpose() {
		Matrix44 result{};
		for (size_t row = 0; row < kMatrixNum; row++) {
			for (size_t column = 0; column < kMatrixNum; column++) {
				result.m[row][column] = m[column][row];
			}
		}
		return result;
	}
};

    inline Matrix44 MakeIdentity44() {
		return{
			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f
		};
	}

	inline Matrix44 MakeTranslateMatrix(const Vec3& translate) {
		return {
			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			translate.x,translate.y,translate.z,1.0f
		};
	}

	inline Matrix44 MakeScaleMatrix(const Vec3& scale) {
		return {
			scale.x,0.0f,0.0f,0.0f,
			0.0f,scale.y,0.0f,0.0f,
			0.0f,0.0f,scale.z,0.0f,
			0.0f,0.0f,0.0f,1.0f
		};
	}

	inline Vec3 Transform(const Vec3& vector, const Matrix44& matrix) {
		Vec3 result{};
		result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
		result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
		result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
		float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
		assert(w != 0.0f);
		result.x /= w;
		result.y /= w;
		result.z /= w;

		return result;
	}

	inline Matrix44 MakeRotateXMatrix(float radian) {
		return {
			1.0f,0.0f,0.0f,0.0f,
			0.0f,std::cosf(radian),std::sinf(radian),0.0f,
			0.0f,-std::sinf(radian),std::cosf(radian),0.0f,
			0.0f,0.0f,0.0f,1.0f
		};
	}

	inline Matrix44 MakeRotateYMatrix(float radian) {
		return {
			std::cosf(radian),0.0f,-std::sinf(radian),0.0f,
			0.0f,1.0f,0.0f,0.0f,
			std::sinf(radian),0.0f,std::cosf(radian),0.0f,
			0.0f,0.0f,0.0f,1.0f
		};
	}

	inline Matrix44 MakeRotateZMatrix(float radian) {
		return {
			std::cosf(radian),std::sinf(radian),0.0f,0.0f,
			-std::sinf(radian),std::cosf(radian),0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f
		};
	}

	inline Matrix44 MakeAffineMatrix(const Vec3& scale, const Vec3& rotate, const Vec3& translation) {
		
		Matrix44 rotateMatrix = MakeRotateXMatrix(rotate.x) * MakeRotateYMatrix(rotate.y) * MakeRotateZMatrix(rotate.z);
		Matrix44 scaleMatrix = MakeScaleMatrix(scale);
		Matrix44 translateMatrix = MakeTranslateMatrix(translation);

		return scaleMatrix * rotateMatrix * translateMatrix;
	}

	inline Matrix44 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
		Matrix44 result{};
		result.m[0][0] = (1.0f / aspectRatio) * (1.0f / std::tanf(fovY / 2.0f));
		result.m[1][1] = (1.0f / std::tanf(fovY / 2.0f));
		result.m[2][2] = farClip / (farClip - nearClip);
		result.m[2][3] = 1.0f;
		result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

		return result;
	}

	inline Matrix44 MakeOrthographicMatrix(float left, float right, float top, float bottom, float nearClip, float farClip) {
		Matrix44 result{};
		result.m[0][0] = 2.0f / (right - left);
		result.m[1][1] = 2.0f / (top - bottom);
		result.m[2][2] = 1.0f / (farClip - nearClip);
		result.m[3][0] = (left + right) / (left - right);
		result.m[3][1] = (top + bottom) / (bottom - top);
		result.m[3][2] = nearClip / (nearClip - farClip);
		result.m[3][3] = 1.0f;

		return result;
	}

	inline Matrix44 MakeViewportMatrix(float left, float top, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f) {
		Matrix44 result{};
		float w = width / 2.0f;
		float h = height / 2.0f;

		result.m[0][0] = w;
		result.m[1][1] = -h;
		result.m[2][2] = maxDepth - minDepth;
		result.m[3][0] = left + w;
		result.m[3][1] = top + h;
		result.m[3][2] = minDepth;
		result.m[3][3] = 1.0f;

		return result;
	}

	inline Vec3 TransformNormal(const Vec3& vector, const Matrix44& matrix) {
	    return {
	        vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0],
	        vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1],
	        vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2]};
    }