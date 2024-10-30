#pragma once

#include <cassert>
#include <cmath>

	struct vec2
	{
		constexpr vec2() noexcept : x(0), y(0)
		{
		}

		constexpr vec2(vec2 const& v) noexcept : x(v.x), y(v.y)
		{

		}

		constexpr vec2(float v) noexcept : x(v), y(v)
		{
		}

		constexpr vec2(float vx, float vy) noexcept : x(vx), y(vy)
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

		constexpr vec2 operator+(float v) const noexcept
		{
			return vec2(x + v, y + v);
		}

		constexpr vec2 operator-(float v) const noexcept
		{
			return vec2(x - v, y - v);
		}

		constexpr vec2 operator-(vec2 v) const noexcept
		{
			return vec2(x - v.x, y - v.y);
		}
		
		constexpr vec2 operator+(vec2 v) const noexcept
		{
			return vec2(x + v.x, y + v.y);
		}

		constexpr vec2 operator*(float v) const noexcept
		{
			return vec2(x * v, y * v);
		}

		constexpr vec2 operator*(vec2 v) const noexcept
		{
			return vec2(x * v.x, y * v.y);
		}

		constexpr vec2 operator/(vec2 v) const noexcept
		{
			return vec2(x / v.x, y / v.y);
		}

		constexpr vec2 operator/(float v) const noexcept
		{
			return vec2(x / v, y / v);
		}

		constexpr vec2& operator+=(float v) noexcept
		{
			x += v;
			y += v;
			return *this;
		}

		constexpr vec2& operator+=(vec2 v) noexcept
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		constexpr vec2& operator-=(vec2 v) noexcept
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		constexpr vec2& operator-=(float v) noexcept
		{
			x -= v;
			y -= v;
			return *this;
		}

		constexpr vec2& operator*=(float v) noexcept
		{
			x *= v;
			y *= v;
			return *this;
		}

		constexpr vec2& operator/=(float v) noexcept
		{
			x /= v;
			y /= v;
			return *this;
		}

		constexpr float dot(vec2 rhs) const noexcept
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

		/*constexpr*/ vec2 getNormalized() const noexcept
		{
			vec2 v(*this);
			v.normalize();
			return v;
		}

		static bool approxEqu(vec2 v1, vec2 v2, float delta) noexcept
		{
			return (v1 - v2).length() < delta;
		}

		union
		{
			float data[2];
			struct { float x, y; };
			struct { float r, g; };
		};
	};
