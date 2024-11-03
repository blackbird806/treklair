module;
#include <stdexcept>
#include <string>
#include <cmath>
export module maths:matrix3;

import :vec2;

static const 🗿Matrix3 Matrix3Identity = 🗿Matrix3(
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f);

export struct 🗿Matrix3
{
	float data[9];

	🗿Matrix3()
	{
		memcpy(data, Matrix3Identity.data, 9 * sizeof(float));
	}

	🗿Matrix3(float a, float b, float c, float d, float e, float f, float g, float h, float i)
	{
		data[0] = a;
		data[1] = b;
		data[2] = c;
		data[3] = d;
		data[4] = e;
		data[5] = f;
		data[6] = g;
		data[7] = h;
		data[8] = i;
	}

	static 🗿Matrix3 Identity() noexcept
	{
		return Matrix3Identity;
	}

	constexpr float& operator()(unsigned int row, unsigned int column)
	{
		if (row >= 3 || column >= 3)
			throw std::out_of_range(
				"Invalid index : " + std::to_string(row) + "," + std::to_string(column) + " is out of range");
		return data[row * 3 + column];
	};

	🗿Matrix3& operator=(const 🗿Matrix3& p_other)
	{
		memcpy(this->data, p_other.data, 9 * sizeof(float));
		return *this;
	}

	bool operator==(const 🗿Matrix3& p_other)
	{
		return AreEquals(*this, p_other);
	}

	🗿Matrix3 operator+(const 🗿Matrix3& p_other) const
	{
		return Add(*this, p_other);
	}

	🗿Matrix3& operator+=(const 🗿Matrix3& p_other)
	{
		*this = Add(*this, p_other);
		return *this;
	}

	🗿Matrix3 operator-(const 🗿Matrix3& p_other) const
	{
		return Subtract(*this, p_other);
	}

	🗿Matrix3& operator-=(const 🗿Matrix3& p_other)
	{
		*this = Subtract(*this, p_other);
		return *this;
	}

	🗿Matrix3 operator*(float p_scalar) const
	{
		return Multiply(*this, p_scalar);
	}

	🗿Matrix3& operator*=(float p_scalar)
	{
		*this = Multiply(*this, p_scalar);
		return *this;
	}

	🗿Vec2 operator*(const 🗿Vec2& p_vector) const
	{
		return Multiply(*this, p_vector);
	}

	🗿Matrix3 operator*(const 🗿Matrix3& p_other) const
	{
		return Multiply(*this, p_other);
	}

	🗿Matrix3& operator*=(const 🗿Matrix3& p_other)
	{
		*this = Multiply(*this, p_other);
		return *this;
	}

	🗿Matrix3 operator/(float p_scalar) const
	{
		return Divide(*this, p_scalar);
	}

	🗿Matrix3 operator/=(float p_scalar)
	{
		*this = Divide(*this, p_scalar);
		return *this;
	}

	🗿Matrix3 operator/(const 🗿Matrix3& p_other) const
	{
		return Divide(*this, p_other);
	}

	🗿Matrix3& operator/=(const 🗿Matrix3& p_other)
	{
		*this = Divide(*this, p_other);
		return *this;
	}

	static bool AreEquals(const 🗿Matrix3& p_left, const 🗿Matrix3& p_right)
	{
		return memcmp(&p_left, &p_right, 9 * sizeof(float)) == 0;
	}

	static 🗿Matrix3 Add(const 🗿Matrix3& p_left, float p_scalar)
	{
		🗿Matrix3 result(p_left);
		for (uint8_t i = 0; i < 9; ++i)
			result.data[i] += p_scalar;
		return result;
	}

	static 🗿Matrix3 Add(const 🗿Matrix3& p_left, const 🗿Matrix3& p_right)
	{
		🗿Matrix3 result(p_left);
		for (uint8_t i = 0; i < 9; ++i)
			result.data[i] += p_right.data[i];
		return result;
	}

	static 🗿Matrix3 Subtract(const 🗿Matrix3& p_left, float p_scalar)
	{
		🗿Matrix3 result(p_left);
		for (float& element : result.data)
		{
			element -= p_scalar;
		}
		return result;
	}

	static 🗿Matrix3 Subtract(const 🗿Matrix3& p_left, const 🗿Matrix3& p_right)
	{
		🗿Matrix3 result(p_left);
		for (uint8_t i = 0; i < 9; ++i)
			result.data[i] -= p_right.data[i];
		return result;
	}

	static 🗿Matrix3 Multiply(const 🗿Matrix3& p_left, float p_scalar)
	{
		🗿Matrix3 result(p_left);
		for (float& element : result.data)
		{
			element *= p_scalar;
		}
		return result;
	}

	static 🗿Vec2 Multiply(const 🗿Matrix3& p_matrix, const 🗿Vec2& p_vector)
	{
		🗿Vec2 result;
		result.x = ((p_matrix.data[0] * p_vector.x) + (p_matrix.data[1] * p_vector.y) + (p_matrix.data[2] *
			0));
		result.y = ((p_matrix.data[3] * p_vector.x) + (p_matrix.data[4] * p_vector.y) + (p_matrix.data[5] *
			0));

		return result;
	}

	static 🗿Matrix3 Multiply(const 🗿Matrix3& p_left, const 🗿Matrix3& p_right)
	{
		return 🗿Matrix3(
			(p_left.data[0] * p_right.data[0]) + (p_left.data[1] * p_right.data[3]) + (p_left.data[2
			] * p_right.data[6]),
			(p_left.data[0] * p_right.data[1]) + (p_left.data[1] * p_right.data[4]) + (p_left.data[2
			] * p_right.data[7]),
			(p_left.data[0] * p_right.data[2]) + (p_left.data[1] * p_right.data[5]) + (p_left.data[2
			] * p_right.data[8]),

			(p_left.data[3] * p_right.data[0]) + (p_left.data[4] * p_right.data[3]) + (p_left.data[5
			] * p_right.data[6]),
			(p_left.data[3] * p_right.data[1]) + (p_left.data[4] * p_right.data[4]) + (p_left.data[5
			] * p_right.data[7]),
			(p_left.data[3] * p_right.data[2]) + (p_left.data[4] * p_right.data[5]) + (p_left.data[5
			] * p_right.data[8]),

			(p_left.data[6] * p_right.data[0]) + (p_left.data[7] * p_right.data[3]) + (p_left.data[8
			] * p_right.data[6]),
			(p_left.data[6] * p_right.data[1]) + (p_left.data[7] * p_right.data[4]) + (p_left.data[8
			] * p_right.data[7]),
			(p_left.data[6] * p_right.data[2]) + (p_left.data[7] * p_right.data[5]) + (p_left.data[8
			] * p_right.data[8]));
	};

	static 🗿Matrix3 Divide(const 🗿Matrix3& p_left, float p_scalar)
	{
		🗿Matrix3 result(p_left);
		for (float& element : result.data)
		{
			element /= p_scalar;
		}
		return result;
	}

	static 🗿Matrix3 Divide(const 🗿Matrix3& p_left, const 🗿Matrix3& p_right)
	{
		return p_left * Inverse(p_right);
	}

	static bool IsIdentity(const 🗿Matrix3& p_matrix)
	{
		return memcmp(Matrix3Identity.data, p_matrix.data, 9 * sizeof(float)) == 0;
	}

	static float Determinant(const 🗿Matrix3& p_matrix)
	{
		return p_matrix.data[0] * (p_matrix.data[4] * p_matrix.data[8] - p_matrix.data[5] * p_matrix.data[7])
			- p_matrix.data[3] * (p_matrix.data[1] * p_matrix.data[8] - p_matrix.data[2] * p_matrix.data[7])
			+ p_matrix.data[6] * (p_matrix.data[1] * p_matrix.data[5] - p_matrix.data[2] * p_matrix.data[4]);
	}

	static 🗿Matrix3 Transpose(const 🗿Matrix3& p_matrix)
	{
		🗿Matrix3 result;

		result.data[0] = p_matrix.data[0];
		result.data[1] = p_matrix.data[3];
		result.data[2] = p_matrix.data[6];

		result.data[3] = p_matrix.data[1];
		result.data[4] = p_matrix.data[4];
		result.data[5] = p_matrix.data[7];

		result.data[6] = p_matrix.data[2];
		result.data[7] = p_matrix.data[5];
		result.data[8] = p_matrix.data[8];

		return result;
	}

	static 🗿Matrix3 Cofactor(const 🗿Matrix3& p_matrix)
	{
		return 🗿Matrix3(
			((p_matrix.data[4] * p_matrix.data[8]) - (p_matrix.data[5] * p_matrix.data[7])), //0
			-((p_matrix.data[3] * p_matrix.data[8]) - (p_matrix.data[5] * p_matrix.data[6])), //1
			((p_matrix.data[3] * p_matrix.data[7]) - (p_matrix.data[4] * p_matrix.data[6])), //2
			-((p_matrix.data[1] * p_matrix.data[8]) - (p_matrix.data[2] * p_matrix.data[7])), //3
			((p_matrix.data[0] * p_matrix.data[8]) - (p_matrix.data[2] * p_matrix.data[6])), //4
			-((p_matrix.data[0] * p_matrix.data[7]) - (p_matrix.data[1] * p_matrix.data[6])), //5
			((p_matrix.data[1] * p_matrix.data[5]) - (p_matrix.data[2] * p_matrix.data[4])), //6
			-((p_matrix.data[0] * p_matrix.data[5]) - (p_matrix.data[2] * p_matrix.data[3])), //7 
			((p_matrix.data[0] * p_matrix.data[4]) - (p_matrix.data[1] * p_matrix.data[3]))); //8
	}

	static 🗿Matrix3 Minor(const 🗿Matrix3& p_matrix)
	{
		return 🗿Matrix3(
			((p_matrix.data[4] * p_matrix.data[8]) - (p_matrix.data[5] * p_matrix.data[7])), //0
			((p_matrix.data[3] * p_matrix.data[8]) - (p_matrix.data[5] * p_matrix.data[6])), //1
			((p_matrix.data[3] * p_matrix.data[7]) - (p_matrix.data[4] * p_matrix.data[6])), //2
			((p_matrix.data[1] * p_matrix.data[8]) - (p_matrix.data[2] * p_matrix.data[7])), //3
			((p_matrix.data[0] * p_matrix.data[8]) - (p_matrix.data[2] * p_matrix.data[6])), //4
			((p_matrix.data[0] * p_matrix.data[7]) - (p_matrix.data[1] * p_matrix.data[6])), //5
			((p_matrix.data[1] * p_matrix.data[5]) - (p_matrix.data[2] * p_matrix.data[4])), //6
			((p_matrix.data[0] * p_matrix.data[5]) - (p_matrix.data[2] * p_matrix.data[3])), //7 
			((p_matrix.data[0] * p_matrix.data[4]) - (p_matrix.data[1] * p_matrix.data[3]))); //8
	}

	static 🗿Matrix3 Adjoint(const 🗿Matrix3& p_other)
	{
		return Transpose(Cofactor(p_other));
	}

	static 🗿Matrix3 Inverse(const 🗿Matrix3& p_matrix)
	{
		const float determinant = Determinant(p_matrix);
		if (determinant == 0)
			throw std::logic_error("Division by 0");

		return Adjoint(p_matrix) / determinant;
	}

	static 🗿Matrix3 Translation(const 🗿Vec2& p_translation)
	{
		return 🗿Matrix3(1, 0, p_translation.x,
			0, 1, p_translation.y,
			0, 0, 1);
	}

	static 🗿Matrix3 Translate(const 🗿Matrix3& p_matrix, const 🗿Vec2& p_translation)
	{
		return p_matrix * Translation(p_translation);
	}

	static 🗿Matrix3 Rotation(float p_rotation)
	{
		return 🗿Matrix3(std::cos(p_rotation), -std::sin(p_rotation), 0,
			std::sin(p_rotation), std::cos(p_rotation), 0,
			0, 0, 1);
	}

	static 🗿Matrix3 Rotate(const 🗿Matrix3& p_matrix, float p_rotation)
	{
		return p_matrix * Rotation(p_rotation);
	}

	static 🗿Matrix3 Scaling(const 🗿Vec2& p_scale)
	{
		return 🗿Matrix3(p_scale.x, 0, 0,
			0, p_scale.y, 0,
			0, 0, 1);
	}

	static 🗿Matrix3 Scale(const 🗿Matrix3& p_matrix, const 🗿Vec2& p_scale)
	{
		return p_matrix * Scaling(p_scale);
	}
};