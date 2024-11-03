module;
#include <stdexcept>
#include <string>
#include <cmath>
export module maths:matrix3;

import :transform;
import :vec2;

export struct 🗿Matrix3
{
	float data[9];

	static const 🗿Matrix3 Identity;

	🗿Matrix3()
	{
		memcpy(data, Identity.data, 9 * sizeof(float));
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

	constexpr float& operator()(unsigned int row, unsigned int column)
	{
		if (row >= 3 || column >= 3)
			throw std::out_of_range(
				"Invalid index : " + std::to_string(row) + "," + std::to_string(column) + " is out of range");
		return data[row * 3 + column];
	};

	🗿Matrix3& operator=(const 🗿Matrix3& other)
	{
		memcpy(this->data, other.data, 9 * sizeof(float));
		return *this;
	}

	bool operator==(const 🗿Matrix3& other)
	{
		return AreEquals(*this, other);
	}

	🗿Matrix3 operator+(const 🗿Matrix3& other) const
	{
		return Add(*this, other);
	}

	🗿Matrix3& operator+=(const 🗿Matrix3& other)
	{
		*this = Add(*this, other);
		return *this;
	}

	🗿Matrix3 operator-(const 🗿Matrix3& other) const
	{
		return Subtract(*this, other);
	}

	🗿Matrix3& operator-=(const 🗿Matrix3& other)
	{
		*this = Subtract(*this, other);
		return *this;
	}

	🗿Matrix3 operator*(float scalar) const
	{
		return Multiply(*this, scalar);
	}

	🗿Matrix3& operator*=(float scalar)
	{
		*this = Multiply(*this, scalar);
		return *this;
	}

	🗿Vec2 operator*(const 🗿Vec2& vector) const
	{
		return Multiply(*this, vector);
	}

	🗿Matrix3 operator*(const 🗿Matrix3& other) const
	{
		return Multiply(*this, other);
	}

	🗿Matrix3& operator*=(const 🗿Matrix3& other)
	{
		*this = Multiply(*this, other);
		return *this;
	}

	🗿Matrix3 operator/(float scalar) const
	{
		return Divide(*this, scalar);
	}

	🗿Matrix3 operator/=(float scalar)
	{
		*this = Divide(*this, scalar);
		return *this;
	}

	🗿Matrix3 operator/(const 🗿Matrix3& other) const
	{
		return Divide(*this, other);
	}

	🗿Matrix3& operator/=(const 🗿Matrix3& other)
	{
		*this = Divide(*this, other);
		return *this;
	}

	static bool AreEquals(const 🗿Matrix3& left, const 🗿Matrix3& right)
	{
		return memcmp(&left, &right, 9 * sizeof(float)) == 0;
	}

	static 🗿Matrix3 Add(const 🗿Matrix3& left, float scalar)
	{
		🗿Matrix3 result(left);
		for (uint8_t i = 0; i < 9; ++i)
			result.data[i] += scalar;
		return result;
	}

	static 🗿Matrix3 Add(const 🗿Matrix3& left, const 🗿Matrix3& right)
	{
		🗿Matrix3 result(left);
		for (uint8_t i = 0; i < 9; ++i)
			result.data[i] += right.data[i];
		return result;
	}

	static 🗿Matrix3 Subtract(const 🗿Matrix3& left, float scalar)
	{
		🗿Matrix3 result(left);
		for (float& element : result.data)
		{
			element -= scalar;
		}
		return result;
	}

	static 🗿Matrix3 Subtract(const 🗿Matrix3& left, const 🗿Matrix3& right)
	{
		🗿Matrix3 result(left);
		for (uint8_t i = 0; i < 9; ++i)
			result.data[i] -= right.data[i];
		return result;
	}

	static 🗿Matrix3 Multiply(const 🗿Matrix3& left, float scalar)
	{
		🗿Matrix3 result(left);
		for (float& element : result.data)
		{
			element *= scalar;
		}
		return result;
	}

	static 🗿Vec2 Multiply(const 🗿Matrix3& matrix, const 🗿Vec2& vector)
	{
		🗿Vec2 result;
		result.x = ((matrix.data[0] * vector.x) + (matrix.data[1] * vector.y) + (matrix.data[2] *
			1));
		result.y = ((matrix.data[3] * vector.x) + (matrix.data[4] * vector.y) + (matrix.data[5] *
			1));

		return result;
	}

	static 🗿Matrix3 Multiply(const 🗿Matrix3& left, const 🗿Matrix3& right)
	{
		return 🗿Matrix3(
			(left.data[0] * right.data[0]) + (left.data[1] * right.data[3]) + (left.data[2
			] * right.data[6]),
			(left.data[0] * right.data[1]) + (left.data[1] * right.data[4]) + (left.data[2
			] * right.data[7]),
			(left.data[0] * right.data[2]) + (left.data[1] * right.data[5]) + (left.data[2
			] * right.data[8]),

			(left.data[3] * right.data[0]) + (left.data[4] * right.data[3]) + (left.data[5
			] * right.data[6]),
			(left.data[3] * right.data[1]) + (left.data[4] * right.data[4]) + (left.data[5
			] * right.data[7]),
			(left.data[3] * right.data[2]) + (left.data[4] * right.data[5]) + (left.data[5
			] * right.data[8]),

			(left.data[6] * right.data[0]) + (left.data[7] * right.data[3]) + (left.data[8
			] * right.data[6]),
			(left.data[6] * right.data[1]) + (left.data[7] * right.data[4]) + (left.data[8
			] * right.data[7]),
			(left.data[6] * right.data[2]) + (left.data[7] * right.data[5]) + (left.data[8
			] * right.data[8]));
	};

	static 🗿Matrix3 Divide(const 🗿Matrix3& left, float scalar)
	{
		🗿Matrix3 result(left);
		for (float& element : result.data)
		{
			element /= scalar;
		}
		return result;
	}

	static 🗿Matrix3 Divide(const 🗿Matrix3& left, const 🗿Matrix3& right)
	{
		return left * Inverse(right);
	}

	static bool IsIdentity(const 🗿Matrix3& matrix)
	{
		return memcmp(Identity.data, matrix.data, 9 * sizeof(float)) == 0;
	}

	static float Determinant(const 🗿Matrix3& matrix)
	{
		return matrix.data[0] * (matrix.data[4] * matrix.data[8] - matrix.data[5] * matrix.data[7])
			- matrix.data[3] * (matrix.data[1] * matrix.data[8] - matrix.data[2] * matrix.data[7])
			+ matrix.data[6] * (matrix.data[1] * matrix.data[5] - matrix.data[2] * matrix.data[4]);
	}

	static 🗿Matrix3 Transpose(const 🗿Matrix3& matrix)
	{
		🗿Matrix3 result;

		result.data[0] = matrix.data[0];
		result.data[1] = matrix.data[3];
		result.data[2] = matrix.data[6];

		result.data[3] = matrix.data[1];
		result.data[4] = matrix.data[4];
		result.data[5] = matrix.data[7];

		result.data[6] = matrix.data[2];
		result.data[7] = matrix.data[5];
		result.data[8] = matrix.data[8];

		return result;
	}

	static 🗿Matrix3 Cofactor(const 🗿Matrix3& matrix)
	{
		return 🗿Matrix3(
			((matrix.data[4] * matrix.data[8]) - (matrix.data[5] * matrix.data[7])), //0
			-((matrix.data[3] * matrix.data[8]) - (matrix.data[5] * matrix.data[6])), //1
			((matrix.data[3] * matrix.data[7]) - (matrix.data[4] * matrix.data[6])), //2
			-((matrix.data[1] * matrix.data[8]) - (matrix.data[2] * matrix.data[7])), //3
			((matrix.data[0] * matrix.data[8]) - (matrix.data[2] * matrix.data[6])), //4
			-((matrix.data[0] * matrix.data[7]) - (matrix.data[1] * matrix.data[6])), //5
			((matrix.data[1] * matrix.data[5]) - (matrix.data[2] * matrix.data[4])), //6
			-((matrix.data[0] * matrix.data[5]) - (matrix.data[2] * matrix.data[3])), //7 
			((matrix.data[0] * matrix.data[4]) - (matrix.data[1] * matrix.data[3]))); //8
	}

	static 🗿Matrix3 Minor(const 🗿Matrix3& matrix)
	{
		return 🗿Matrix3(
			((matrix.data[4] * matrix.data[8]) - (matrix.data[5] * matrix.data[7])), //0
			((matrix.data[3] * matrix.data[8]) - (matrix.data[5] * matrix.data[6])), //1
			((matrix.data[3] * matrix.data[7]) - (matrix.data[4] * matrix.data[6])), //2
			((matrix.data[1] * matrix.data[8]) - (matrix.data[2] * matrix.data[7])), //3
			((matrix.data[0] * matrix.data[8]) - (matrix.data[2] * matrix.data[6])), //4
			((matrix.data[0] * matrix.data[7]) - (matrix.data[1] * matrix.data[6])), //5
			((matrix.data[1] * matrix.data[5]) - (matrix.data[2] * matrix.data[4])), //6
			((matrix.data[0] * matrix.data[5]) - (matrix.data[2] * matrix.data[3])), //7 
			((matrix.data[0] * matrix.data[4]) - (matrix.data[1] * matrix.data[3]))); //8
	}

	static 🗿Matrix3 Adjoint(const 🗿Matrix3& other)
	{
		return Transpose(Cofactor(other));
	}

	static 🗿Matrix3 Inverse(const 🗿Matrix3& matrix)
	{
		const float determinant = Determinant(matrix);
		if (determinant == 0)
			throw std::logic_error("Division by 0");

		return Adjoint(matrix) / determinant;
	}

	static 🗿Matrix3 Translation(const 🗿Vec2& translation)
	{
		return 🗿Matrix3(
			1, 0, translation.x,
			0, 1, translation.y,
			0, 0, 1);
	}

	static 🗿Matrix3 Translate(const 🗿Matrix3& matrix, const 🗿Vec2& translation)
	{
		return matrix * Translation(translation);
	}

	static 🗿Matrix3 Rotation(float rotation)
	{
		return 🗿Matrix3(
			std::cos(rotation), -std::sin(rotation), 0,
			std::sin(rotation), std::cos(rotation), 0,
			0, 0, 1);
	}

	static 🗿Matrix3 Rotate(const 🗿Matrix3& matrix, float rotation)
	{
		return matrix * Rotation(rotation);
	}

	static 🗿Matrix3 Scaling(const 🗿Vec2& scale)
	{
		return 🗿Matrix3(
			scale.x, 0, 0,
			0, scale.y, 0,
			0, 0, 1);
	}

	static 🗿Matrix3 Scale(const 🗿Matrix3& matrix, const 🗿Vec2& scale)
	{
		return matrix * Scaling(scale);
	}

	static 🗿Matrix3 Transform(const 🗿Transform& transform)
	{
		🗿Matrix3 t = 🗿Matrix3::Translation(transform.position);
		🗿Matrix3 r = 🗿Matrix3::Rotation(transform.rotation);
		🗿Matrix3 s = 🗿Matrix3::Scaling(transform.scale);
		return t * r * s;
	}

	static 🗿Matrix3 TransRota(const 🗿Transform& transform)
	{
		🗿Matrix3 t = 🗿Matrix3::Translation(transform.position);
		🗿Matrix3 r = 🗿Matrix3::Rotation(transform.rotation);
		return t * r;
	}
};

const 🗿Matrix3 🗿Matrix3::Identity = 🗿Matrix3(
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f);