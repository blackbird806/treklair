module;
#include <cassert>
#include <cmath>
#include <algorithm>

export module maths:vec2;

export struct 🗿Vec2
{
	constexpr 🗿Vec2() noexcept : x(0), y(0)
	{
	}

	constexpr 🗿Vec2(🗿Vec2 const& v) noexcept : x(v.x), y(v.y)
	{

	}

	constexpr 🗿Vec2(float v) noexcept : x(v), y(v)
	{
	}

	constexpr 🗿Vec2(float vx, float vy) noexcept : x(vx), y(vy)
	{
	}

	constexpr float operator[](unsigned i) const noexcept
	{
		assert(i < 2);
		return data[i];
	}

	constexpr float& operator[](unsigned i) noexcept
	{
		assert(i < 2);
		return data[i];
	}

	constexpr 🗿Vec2 operator-() const noexcept
	{
		return 🗿Vec2(-x, -y);
	}

	constexpr 🗿Vec2 operator+(float v) const noexcept
	{
		return 🗿Vec2(x + v, y + v);
	}

	constexpr 🗿Vec2 operator-(float v) const noexcept
	{
		return 🗿Vec2(x - v, y - v);
	}

	constexpr 🗿Vec2 operator-(🗿Vec2 v) const noexcept
	{
		return 🗿Vec2(x - v.x, y - v.y);
	}

	constexpr 🗿Vec2 operator+(🗿Vec2 v) const noexcept
	{
		return 🗿Vec2(x + v.x, y + v.y);
	}

	constexpr 🗿Vec2 operator*(float v) const noexcept
	{
		return 🗿Vec2(x * v, y * v);
	}

	constexpr 🗿Vec2 operator*(🗿Vec2 v) const noexcept
	{
		return 🗿Vec2(x * v.x, y * v.y);
	}

	constexpr 🗿Vec2 operator/(🗿Vec2 v) const noexcept
	{
		return 🗿Vec2(x / v.x, y / v.y);
	}

	constexpr 🗿Vec2 operator/(float v) const noexcept
	{
		return 🗿Vec2(x / v, y / v);
	}

	constexpr 🗿Vec2& operator+=(float v) noexcept
	{
		x += v;
		y += v;
		return *this;
	}

	constexpr 🗿Vec2& operator+=(🗿Vec2 v) noexcept
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	constexpr 🗿Vec2& operator-=(🗿Vec2 v) noexcept
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	constexpr 🗿Vec2& operator-=(float v) noexcept
	{
		x -= v;
		y -= v;
		return *this;
	}

	constexpr 🗿Vec2& operator*=(float v) noexcept
	{
		x *= v;
		y *= v;
		return *this;
	}

	constexpr 🗿Vec2& operator/=(float v) noexcept
	{
		x /= v;
		y /= v;
		return *this;
	}

	constexpr float dot(🗿Vec2 rhs) const noexcept
	{
		return x * rhs.x + y * rhs.y;
	}

	constexpr float sqrLength() const noexcept
	{
		return x * x + y * y;
	}

	/*constexpr*/ float length() const noexcept
	{
		return sqrt(sqrLength());
	}

	/*constexpr*/ void normalize() noexcept
	{
		*this = *this / length();
	}

	/*constexpr*/ 🗿Vec2 getNormalized() const noexcept
	{
		🗿Vec2 v(*this);
		v.normalize();
		return v;
	}

	static bool approxEqu(🗿Vec2 v1, 🗿Vec2 v2, float delta) noexcept
	{
		return (v1 - v2).length() < delta;
	}

	static 🗿Vec2 clamp(🗿Vec2 test, 🗿Vec2 min, 🗿Vec2 max) noexcept
	{
		return 🗿Vec2(std::clamp(test.x, min.x, max.x), std::clamp(test.y, min.y, max.y));
	}

	union
	{
		float data[2];
		struct { float x, y; };
		struct { float r, g; };
	};
};
