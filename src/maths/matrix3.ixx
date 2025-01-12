export module treklair:matrix3;
import std;
import :transform;
import :vec2;

export struct Matrix3
{
	float data[9];

	static const Matrix3 Identity;

	Matrix3()
	{
		std::memcpy(data, Identity.data, 9 * sizeof(float));
	}

	Matrix3(float a, float b, float c, float d, float e, float f, float g, float h, float i)
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

	Matrix3& operator=(const Matrix3& other)
	{
		std::memcpy(this->data, other.data, 9 * sizeof(float));
		return *this;
	}

	bool operator==(const Matrix3& other)
	{
		return areEquals(*this, other);
	}

	Matrix3 operator+(const Matrix3& other) const
	{
		return add(*this, other);
	}

	Matrix3& operator+=(const Matrix3& other)
	{
		*this = add(*this, other);
		return *this;
	}

	Matrix3 operator-(const Matrix3& other) const
	{
		return subtract(*this, other);
	}

	Matrix3& operator-=(const Matrix3& other)
	{
		*this = subtract(*this, other);
		return *this;
	}

	Matrix3 operator*(float scalar) const
	{
		return multiply(*this, scalar);
	}

	Matrix3& operator*=(float scalar)
	{
		*this = multiply(*this, scalar);
		return *this;
	}

	Vec2 operator*(const Vec2& vector) const
	{
		return multiply(*this, vector);
	}

	Matrix3 operator*(const Matrix3& other) const
	{
		return multiply(*this, other);
	}

	Matrix3& operator*=(const Matrix3& other)
	{
		*this = multiply(*this, other);
		return *this;
	}

	Matrix3 operator/(float scalar) const
	{
		return divide(*this, scalar);
	}

	Matrix3 operator/=(float scalar)
	{
		*this = divide(*this, scalar);
		return *this;
	}

	Matrix3 operator/(const Matrix3& other) const
	{
		return divide(*this, other);
	}

	Matrix3& operator/=(const Matrix3& other)
	{
		*this = divide(*this, other);
		return *this;
	}

	static bool areEquals(const Matrix3& left, const Matrix3& right)
	{
		return std::memcmp(&left, &right, 9 * sizeof(float)) == 0;
	}

	static Matrix3 add(const Matrix3& left, float scalar)
	{
		Matrix3 result(left);
		for (uint8_t i = 0; i < 9; ++i)
			result.data[i] += scalar;
		return result;
	}

	static Matrix3 add(const Matrix3& left, const Matrix3& right)
	{
		Matrix3 result(left);
		for (uint8_t i = 0; i < 9; ++i)
			result.data[i] += right.data[i];
		return result;
	}

	static Matrix3 subtract(const Matrix3& left, float scalar)
	{
		Matrix3 result(left);
		for (float& element : result.data)
		{
			element -= scalar;
		}
		return result;
	}

	static Matrix3 subtract(const Matrix3& left, const Matrix3& right)
	{
		Matrix3 result(left);
		for (uint8_t i = 0; i < 9; ++i)
			result.data[i] -= right.data[i];
		return result;
	}

	static Matrix3 multiply(const Matrix3& left, float scalar)
	{
		Matrix3 result(left);
		for (float& element : result.data)
		{
			element *= scalar;
		}
		return result;
	}

	static Vec2 multiply(const Matrix3& matrix, const Vec2& vector)
	{
		Vec2 result;
		result.x = ((matrix.data[0] * vector.x) + (matrix.data[1] * vector.y) + (matrix.data[2] *
			1));
		result.y = ((matrix.data[3] * vector.x) + (matrix.data[4] * vector.y) + (matrix.data[5] *
			1));

		return result;
	}

	static Matrix3 multiply(const Matrix3& left, const Matrix3& right)
	{
		return Matrix3(
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

	static Matrix3 divide(const Matrix3& left, float scalar)
	{
		Matrix3 result(left);
		for (float& element : result.data)
		{
			element /= scalar;
		}
		return result;
	}

	static Matrix3 divide(const Matrix3& left, const Matrix3& right)
	{
		return left * inverse(right);
	}

	static bool isIdentity(const Matrix3& matrix)
	{
		return std::memcmp(Identity.data, matrix.data, 9 * sizeof(float)) == 0;
	}

	static float determinant(const Matrix3& matrix)
	{
		return matrix.data[0] * (matrix.data[4] * matrix.data[8] - matrix.data[5] * matrix.data[7])
			- matrix.data[3] * (matrix.data[1] * matrix.data[8] - matrix.data[2] * matrix.data[7])
			+ matrix.data[6] * (matrix.data[1] * matrix.data[5] - matrix.data[2] * matrix.data[4]);
	}

	static Matrix3 transpose(const Matrix3& matrix)
	{
		Matrix3 result;

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

	static Matrix3 cofactor(const Matrix3& matrix)
	{
		return Matrix3(
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

	static Matrix3 minor(const Matrix3& matrix)
	{
		return Matrix3(
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

	static Matrix3 adjoint(const Matrix3& other)
	{
		return transpose(cofactor(other));
	}

	static Matrix3 inverse(const Matrix3& matrix)
	{
		const float det = determinant(matrix);
		if (det == 0)
			throw std::logic_error("Division by 0");

		return adjoint(matrix) / det;
	}

	static Matrix3 translation(const Vec2& translation)
	{
		return Matrix3(
			1, 0, translation.x,
			0, 1, translation.y,
			0, 0, 1);
	}

	static Matrix3 translate(const Matrix3& matrix, const Vec2& _translation)
	{
		return matrix * translation(_translation);
	}

	static Matrix3 rotation(float rotation)
	{
		return Matrix3(
			std::cos(rotation), -std::sin(rotation), 0,
			std::sin(rotation), std::cos(rotation), 0,
			0, 0, 1);
	}

	static Matrix3 rotate(const Matrix3& matrix, float _rotation)
	{
		return matrix * rotation(_rotation);
	}

	static Matrix3 scaling(const Vec2& scale)
	{
		return Matrix3(
			scale.x, 0, 0,
			0, scale.y, 0,
			0, 0, 1);
	}

	static Matrix3 scale(const Matrix3& matrix, const Vec2& scale)
	{
		return matrix * scaling(scale);
	}

	static Matrix3 transform(const Transform& transform)
	{
		Matrix3 t = Matrix3::translation(transform.position);
		Matrix3 r = Matrix3::rotation(transform.rotation);
		Matrix3 s = Matrix3::scaling(transform.scale);
		return t * r * s;
	}

	static Matrix3 transRota(const Transform& transform)
	{
		Matrix3 t = Matrix3::translation(transform.position);
		Matrix3 r = Matrix3::rotation(transform.rotation);
		return t * r;
	}
};

const Matrix3 Matrix3::Identity = Matrix3(
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f);