#pragma once
#include <numbers>
#include <cmath>
#include <Windows.h>


struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}
	const float* operator[ ](int index) const noexcept {
		return matrix[index];
	}

	float matrix[4][4];
};

struct Vector3
{
	// constructor
	Vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }

	// operator overloads
	const Vector3 operator-(const Vector3& other) const noexcept
	{
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}

	const Vector3 operator+(const Vector3& other) const noexcept
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	const Vector3 operator/(const float factor) const noexcept
	{
		return Vector3{ x / factor, y / factor, z / factor };
	}

	const Vector3 operator*(const float factor) const noexcept
	{
		return Vector3{ x * factor, y * factor, z * factor };
	}

	const Vector3 W2S(view_matrix_t vm) const {
		float _x = vm[0][0] * x + vm[0][1] * y + vm[0][2] * z + vm[0][3];
		float _y = vm[1][0] * x + vm[1][1] * y + vm[1][2] * z + vm[1][3];

		float w = vm[3][0] * x + vm[3][1] * y + vm[3][2] * z + vm[3][3];
		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float x = 1920 * .5f;
		float y = 1080 * .5f;

		x += 0.5f * _x * 1920 + 0.5f;
		y -= 0.5f * _y * 1080 + 0.5f;
		return Vector3( x,y,w );
	}
	// utils
	const Vector3 ToAngle() const noexcept
	{
		return Vector3{
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f };
	}

	const bool IsZero() const noexcept
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	const float Distance(Vector3 p2) {
		float dist = std::sqrt(std::pow(p2.x - x, 2) + std::pow(p2.y - y, 2));
		return dist;
	}

	// struct data
	float x, y, z;
};