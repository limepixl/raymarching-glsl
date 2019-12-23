#pragma once

struct Vec2
{
	float x, y;

	Vec2() = default;
	Vec2(float x, float y) : x(x), y(y)
	{}
};

struct Vec3
{
	float x, y, z;

	Vec3() = default;
	Vec3(float x, float y, float z) : x(x), y(y), z(z)
	{}

	Vec3 operator*(const float rhs) const
	{
		return Vec3(x * rhs, y * rhs, z * rhs);
	}

	void operator+=(const Vec3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}

	void operator-=(const Vec3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}
};