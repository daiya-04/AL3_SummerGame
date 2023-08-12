#pragma once
#include <cmath>
#include <algorithm>

class Vec3 {
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
public:

	inline Vec3(){}
	inline Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	inline Vec3(const Vec3& vector) : x(vector.x), y(vector.y), z(vector.z) {}

	inline Vec3 operator-() const { return{ -x,-y,-z }; }

	inline Vec3& operator+=(const Vec3& vector) {
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}

	inline Vec3& operator-=(const Vec3& vector) {
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}

	inline Vec3& operator*=(const float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	inline Vec3& operator/=(const Vec3& vector) {
		x /= vector.x;
		y /= vector.y;
		z /= vector.z;
		return *this;
	}
	
	friend inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
		return { v1.x + v2.x,v1.y + v2.y,v1.z + v2.z };
	}

	friend inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
		return { v1.x - v2.x,v1.y - v2.y,v1.z - v2.z };
	}

	friend inline Vec3 operator*(const Vec3& vector, const float scalar) {
		return { vector.x * scalar,vector.y * scalar,vector.z * scalar };
	}

	friend inline Vec3 operator*(const float scalar, const Vec3& vector) {
		return { scalar * vector.x,scalar * vector.y,scalar * vector.z };
	}

	friend inline Vec3 operator/(const Vec3& vector, const float scalar) {
		return { vector.x / scalar,vector.y / scalar,vector.z / scalar };
	}

	friend inline bool operator==(const Vec3& v1, const Vec3& v2) {
		return { v1.x == v2.x && v1.y == v2.y && v1.z == v2.z };
	}

	friend inline bool operator!=(const Vec3& v1, const Vec3& v2) {
		return !(v1 == v2);
	}

	friend inline float Dot(const Vec3& v1, const Vec3& v2) {
		return { v1.x * v2.x + v1.y * v2.y + v1.z * v2.z };
	}

	friend inline Vec3 Cross(const Vec3& v1, const Vec3& v2) {
		return { v1.y * v2.z - v1.z * v2.y,v1.z * v2.x-v1.x * v2.z,v1.x * v2.y - v1.y * v2.x };
	}

	inline float Length() const {
		return sqrtf(x * x + y * y + z * z);
	}

	inline Vec3 Normalize() const {
		return *this / Length();
	}

	friend inline Vec3 Lerp(float t, const Vec3& start, const Vec3& end) {
		t = std::clamp(t, 0.0f, 1.0f);
		return { (1.0f - t) * start + t * end };
	}

	friend inline Vec3 Project(const Vec3& v1, const Vec3& v2) {
		float t = Dot(v1, v2) / std::powf(v2.Length(), 2);

		t = std::clamp(t, 0.0f, 1.0f);

		return t * v2;
	}

	inline Vec3 Perpendicular() const {
		if (x != 0.0f || y != 0.0f) {
			return { -y,x,0.0f };
		}
		return { 0.0f,-z,y };
	}

	friend inline Vec3 Bazier(const Vec3& p0, const Vec3& p1, const Vec3& p2, float t) {
		Vec3 p0p1 = Lerp(t, p0, p1);
		Vec3 p1p2 = Lerp(t, p1, p2);
		Vec3 p = Lerp(t, p0p1, p1p2);

		return p;
	}
};